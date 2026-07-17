#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;

struct Settings;
struct SharedState;

struct ToolItem {
    std::wstring id;
    std::wstring name;
    std::wstring icon;
    std::wstring cmd;
};

const ToolItem* FindToolById(const std::wstring& toolId);
extern std::vector<std::wstring> g_activeTools;
extern bool g_toolsEditMode;

class Renderer {
   public:
    bool Initialize(HWND hwnd) {
        hwnd_ = hwnd;

        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
                                       __uuidof(ID2D1Factory),
                                       reinterpret_cast<void**>(d2dFactory_.GetAddressOf()));
        if (FAILED(hr)) {
            return false;
        }

        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                                 reinterpret_cast<IUnknown**>(dwriteFactory_.GetAddressOf()));
        if (FAILED(hr)) {
            return false;
        }

        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            0.0f, 0.0f,
            D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

        hr = d2dFactory_->CreateDCRenderTarget(&props, &target_);
        if (FAILED(hr)) {
            return false;
        }

        // Apple SF Pro-like: use Segoe UI Variable Display for clean modern look.
        dwriteFactory_->CreateTextFormat(L"Segoe UI Variable Display", nullptr,
                                         DWRITE_FONT_WEIGHT_SEMI_BOLD,
                                         DWRITE_FONT_STYLE_NORMAL,
                                         DWRITE_FONT_STRETCH_NORMAL,
                                         13.5f, L"", &textFormat_);
        dwriteFactory_->CreateTextFormat(L"Segoe UI Variable Small", nullptr,
                                         DWRITE_FONT_WEIGHT_NORMAL,
                                         DWRITE_FONT_STYLE_NORMAL,
                                         DWRITE_FONT_STRETCH_NORMAL,
                                         11.0f, L"", &smallTextFormat_);
        dwriteFactory_->CreateTextFormat(L"Segoe UI Variable Display", nullptr,
                                         DWRITE_FONT_WEIGHT_BOLD,
                                         DWRITE_FONT_STYLE_NORMAL,
                                         DWRITE_FONT_STRETCH_NORMAL,
                                         18.0f, L"", &clockFormat_);
        dwriteFactory_->CreateTextFormat(L"Segoe Fluent Icons", nullptr,
                                         DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                         DWRITE_FONT_STRETCH_NORMAL,
                                         16.0f, L"", &iconFormat_);

        if (textFormat_) {
            textFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }
        if (smallTextFormat_) {
            smallTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }
        if (clockFormat_) {
            clockFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
            clockFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            clockFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
        if (iconFormat_) {
            iconFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }

        return CreateBackingBitmap(520, 140);
    }

    bool Render(const SharedState& state, const Settings& settings, const Activity& primary,
                const std::optional<Activity>& secondary, float width, float height,
                float nudge, bool hover, bool pinned, double now) {
        EnsureTextFormats(settings.sizeScale);
        const int pixelWidth = std::max(1, static_cast<int>(std::ceil(width + kRenderPadX * 2.0f)));
        const int pixelHeight = std::max(1, static_cast<int>(std::ceil(height + kRenderPadY * 2.0f)));

        if (pixelWidth != bitmapWidth_ || pixelHeight != bitmapHeight_) {
            if (!CreateBackingBitmap(pixelWidth, pixelHeight)) {
                return false;
            }
            PositionOverlayWindow(hwnd_, pixelWidth, pixelHeight);
        } else if (g_layoutDirty.exchange(false)) {
            PositionOverlayWindow(hwnd_, pixelWidth, pixelHeight);
        }

        RECT rc = {0, 0, bitmapWidth_, bitmapHeight_};
        HRESULT hr = target_->BindDC(memDc_, &rc);
        if (FAILED(hr)) {
            return false;
        }

        target_->BeginDraw();
        target_->Clear(D2D1::ColorF(0, 0.0f));

        EnsureBrushes(settings, state);
        settingsOpacity_ = settings.pillOpacity;

        const float hoverScale = hover || pinned ? 1.025f : 1.0f;
        const float scale = hoverScale;

        const float top = kRenderPadY + nudge;
        const float left = kRenderPadX;
        
        if (width >= 2.0f && height >= 2.0f) {
            if (secondary) {
                const float gap = 12.0f * settings.sizeScale;
                const float maxH = std::max(primary.height, secondary->height);
                const float pTop = top + (maxH - primary.height) * 0.5f;
                const float sTop = top + (maxH - secondary->height) * 0.5f;

                DrawPill(state, settings, primary,
                         D2D1::RectF(left, pTop, left + primary.width, pTop + primary.height),
                         scale, now);
                DrawPill(state, settings, *secondary,
                         D2D1::RectF(left + primary.width + gap, sTop,
                                      left + primary.width + gap + secondary->width,
                                      sTop + secondary->height),
                         scale, now);
            } else {
                DrawPill(state, settings, primary,
                         D2D1::RectF(left, top, left + width, top + height), scale, now);
            }
        }

        hr = target_->EndDraw();
        if (FAILED(hr)) {
            return false;
        }

        POINT src = {0, 0};
        SIZE size = {bitmapWidth_, bitmapHeight_};
        POINT dst = {};
        RECT winRect = {};
        GetWindowRect(hwnd_, &winRect);
        dst.x = winRect.left;
        dst.y = winRect.top;

        BLENDFUNCTION blend = {};
        blend.BlendOp = AC_SRC_OVER;
        float windowAlpha = settings.pillOpacity;
        if (settings.colorTheme == 1 || settings.colorTheme == 10 || settings.colorTheme == 7) {
            windowAlpha = 1.0f;
        }
        blend.SourceConstantAlpha = static_cast<BYTE>(windowAlpha * 255.0f);
        blend.AlphaFormat = AC_SRC_ALPHA;

        return UpdateLayeredWindow(hwnd_, nullptr, &dst, &size, memDc_, &src, 0, &blend,
                                   ULW_ALPHA) != FALSE;
    }

    void Shutdown() {
        artBitmap_.Reset();
        notificationIconBitmap_.Reset();
        mediaSourceIconBitmap_.Reset();
        clipboardIconBitmap_.Reset();
        accentBrush_.Reset();
        redBrush_.Reset();
        textBrush_.Reset();
        mutedBrush_.Reset();
        tintBrush_.Reset();
        shadowBrush_.Reset();
        target_.Reset();
        textFormat_.Reset();
        smallTextFormat_.Reset();
        boldTextFormat_.Reset();
        hugeTextFormat_.Reset();
        clockFormat_.Reset();
        iconFormat_.Reset();
        dwriteFactory_.Reset();
        d2dFactory_.Reset();

        if (oldBitmap_) {
            SelectObject(memDc_, oldBitmap_);
            oldBitmap_ = nullptr;
        }
        if (dib_) {
            DeleteObject(dib_);
            dib_ = nullptr;
        }
        if (memDc_) {
            DeleteDC(memDc_);
            memDc_ = nullptr;
        }
    }

   private:
    bool CreateBackingBitmap(int width, int height) {
        if (oldBitmap_) {
            SelectObject(memDc_, oldBitmap_);
            oldBitmap_ = nullptr;
        }
        if (dib_) {
            DeleteObject(dib_);
            dib_ = nullptr;
        }
        if (!memDc_) {
            HDC screen = GetDC(nullptr);
            memDc_ = CreateCompatibleDC(screen);
            ReleaseDC(nullptr, screen);
            if (!memDc_) {
                return false;
            }
        }

        BITMAPINFO bi = {};
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = width;
        bi.bmiHeader.biHeight = -height;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;

        void* bits = nullptr;
        dib_ = CreateDIBSection(memDc_, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
        if (!dib_) {
            return false;
        }

        oldBitmap_ = static_cast<HBITMAP>(SelectObject(memDc_, dib_));
        bitmapWidth_ = width;
        bitmapHeight_ = height;
        return true;
    }

    float lastFontScale_ = 0.0f;
    void EnsureTextFormats(float scale) {
        if (std::abs(scale - lastFontScale_) < 0.001f) {
            return;
        }

        textFormat_ = nullptr;
        smallTextFormat_ = nullptr;
        clockFormat_ = nullptr;
        iconFormat_ = nullptr;
        boldTextFormat_ = nullptr;
        hugeTextFormat_ = nullptr;

        auto CreateFormat = [&](const wchar_t* family, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, float size, IDWriteTextFormat** outFormat) {
            HRESULT hr = dwriteFactory_->CreateTextFormat(family, nullptr, weight, style, stretch, size, L"", outFormat);
            if (FAILED(hr)) {
                Wh_Log(L"CreateTextFormat failed for '%ls' (size %.1f), hr=0x%08X. Falling back to Segoe UI.", family, size, hr);
                hr = dwriteFactory_->CreateTextFormat(L"Segoe UI", nullptr, weight, style, stretch, size, L"", outFormat);
                if (FAILED(hr)) {
                    Wh_Log(L"CreateTextFormat failed for Segoe UI fallback, hr=0x%08X", hr);
                }
            }
        };

        CreateFormat(L"Segoe UI Variable Display", DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 13.5f * scale, &textFormat_);
        CreateFormat(L"Segoe UI Variable Small", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 11.0f * scale, &smallTextFormat_);
        CreateFormat(L"Segoe UI Variable Display", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 26.0f * scale, &clockFormat_);
        CreateFormat(L"Segoe UI Variable Display", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f * scale, &boldTextFormat_);
        CreateFormat(L"Segoe UI Variable Display", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 42.0f * scale, &hugeTextFormat_);
        CreateFormat(L"Segoe Fluent Icons", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f * scale, &iconFormat_);

        if (!iconFormat_) {
            CreateFormat(L"Segoe MDL2 Assets", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f * scale, &iconFormat_);
        }

        if (textFormat_) {
            textFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }
        if (smallTextFormat_) {
            smallTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }
        if (boldTextFormat_) {
            boldTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
            boldTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        }
        if (hugeTextFormat_) {
            hugeTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
            hugeTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        }
        if (clockFormat_) {
            clockFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
            clockFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            clockFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
        if (iconFormat_) {
            iconFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }

        lastFontScale_ = scale;
    }

    void EnsureBrushes(const Settings& settings, const SharedState& state) {
        D2D1_COLOR_F accentTarget = settings.customAccent;
        if (settings.accentMode == AccentMode::System) {
            accentTarget = GetSystemAccentColor();
        } else if (settings.accentMode == AccentMode::Auto && !state.media.art.bgra.empty()) {
            accentTarget = state.media.art.sampledAccent;
        }

        // Theme-specific accent color overrides
        if (settings.colorTheme == 6) {
            accentTarget = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f); // Nothing OS (White)
        } else if (settings.colorTheme == 8) {
            accentTarget = D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f); // Cyberpunk (Neon Cyan)
        } else if (settings.colorTheme == 11) {
            accentTarget = D2D1::ColorF(0.0f, 0.898f, 1.0f, 1.0f); // Holographic AI (Jarvis Cyan)
        }

        // Q3: Smooth accent color transition â€” lerp toward the target at ~3% per frame
        // (at 60fps this gives a ~600ms transition). Reset instantly on first frame.
        const float lerpSpeed = 0.035f;
        if (!accentBrush_) {
            lerpedAccent_ = accentTarget;
            target_->CreateSolidColorBrush(lerpedAccent_, &accentBrush_);
        } else {
            lerpedAccent_.r += (accentTarget.r - lerpedAccent_.r) * lerpSpeed;
            lerpedAccent_.g += (accentTarget.g - lerpedAccent_.g) * lerpSpeed;
            lerpedAccent_.b += (accentTarget.b - lerpedAccent_.b) * lerpSpeed;
            lerpedAccent_.a = accentTarget.a;
            accentBrush_->SetColor(lerpedAccent_);
        }

        if (!redBrush_) target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.27f, 0.27f, 1.0f), &redBrush_);

        // Use user-configured text colors.
        D2D1_COLOR_F primary = settings.textPrimaryColor;
        primary.a = 0.96f;
        if (!textBrush_) target_->CreateSolidColorBrush(primary, &textBrush_);
        else textBrush_->SetColor(primary);

        D2D1_COLOR_F secondary = settings.textSecondaryColor;
        secondary.a = 0.72f;
        if (!mutedBrush_) target_->CreateSolidColorBrush(secondary, &mutedBrush_);
        else mutedBrush_->SetColor(secondary);

        // Cache pill bg color with opacity applied.
        pillBgColor_ = settings.pillBgColor;
        pillBgColor_.a = 1.0f;
        
        settingsOpacity_ = settings.pillOpacity;
        if (settings.colorTheme == 7) {
            settingsOpacity_ = 0.32f; // Glassmorphism theme background transparency
        } else if (settings.colorTheme == 1 || settings.colorTheme == 10) {
            settingsOpacity_ = 1.0f;  // Force 100% solid opaque background (no bleed-through)
        }
        
        D2D1_COLOR_F tintColor = D2D1::ColorF(0.010f, 0.010f, 0.012f, settings.tintOpacity);
        if (!tintBrush_) target_->CreateSolidColorBrush(tintColor, &tintBrush_);
        else tintBrush_->SetColor(tintColor);

        if (!shadowBrush_) target_->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0.70f), &shadowBrush_);
    }

    void DrawPill(const SharedState& state, const Settings& settings, const Activity& activity,
                  D2D1_RECT_F rect, float scale, double now) {
        const float cx = (rect.left + rect.right) * 0.5f;
        const float cy = (rect.top + rect.bottom) * 0.5f;
        const float w = (rect.right - rect.left) * scale;
        const float h = (rect.bottom - rect.top) * scale;
        rect = D2D1::RectF(cx - w * 0.5f, cy - h * 0.5f, cx + w * 0.5f, cy + h * 0.5f);

        D2D1_COLOR_F originalAccent = lerpedAccent_;
        if (settings.colorfulModules) {
            D2D1_COLOR_F accentColor = lerpedAccent_;
            if (activity.kind == IslandKind::Idle) {
                if (settings.gameOverlay || Wh_GetIntValue(L"GameOverlayPinned", 0) != 0) {
                    accentColor = D2D1::ColorF(0.749f, 0.353f, 0.949f, 1.0f); // Neon purple (Game Overlay)
                } else {
                    int tab = g_idleTab % 4;
                    if (tab < 0) tab += 4;
                    if (tab == 0) {
                        accentColor = D2D1::ColorF(0.957f, 0.263f, 0.212f, 1.0f); // Cherry red (Calendar)
                    } else if (tab == 1) {
                        accentColor = D2D1::ColorF(1.0f, 0.757f, 0.027f, 1.0f); // Sunny gold (Weather)
                    } else if (tab == 2) {
                        accentColor = D2D1::ColorF(0.18f, 0.8f, 0.443f, 1.0f); // Neon green (System Stats)
                    } else {
                        accentColor = D2D1::ColorF(0.00f, 0.65f, 1.00f, 1.0f); // Cool blue (Tools Tray)
                    }
                }
            } else if (activity.kind == IslandKind::Media) {
                // Keep original/sampled accent
            } else if (activity.kind == IslandKind::Progress) {
                accentColor = D2D1::ColorF(0.0f, 0.737f, 0.827f, 1.0f); // Cyan
            } else if (activity.kind == IslandKind::Clipboard) {
                accentColor = D2D1::ColorF(1.0f, 0.596f, 0.0f, 1.0f); // Orange
            } else if (activity.kind == IslandKind::Notification) {
                accentColor = D2D1::ColorF(0.122f, 0.533f, 0.898f, 1.0f); // Royal blue
            }
            lerpedAccent_ = accentColor;
            if (accentBrush_) {
                accentBrush_->SetColor(lerpedAccent_);
            }
        }

        float radius = settings.w11Style ? 8.0f * settings.sizeScale : (rect.bottom - rect.top) * 0.5f;
        if (!settings.w11Style) {
            radius = std::min(radius, 44.0f * settings.sizeScale);
        }
        DrawSoftShadow(rect, radius);

        D2D1_ROUNDED_RECT pill = D2D1::RoundedRect(rect, radius, radius);
        DrawPillSurface(rect, radius, activity.kind, settings.w11Style, settings.colorTheme);

        if (activity.kind == IslandKind::Progress) {
            DrawProgressRing(rect, state.progress.percent);
        }

        if (activity.kind == IslandKind::BatteryLow) {
            const float pulse = 0.5f + 0.5f * std::sin(static_cast<float>(now * 2.0 * 3.14159265 * 2.1));
            redBrush_->SetOpacity(0.45f + 0.45f * pulse);
            target_->DrawRoundedRectangle(pill, redBrush_.Get(), 2.0f);
            redBrush_->SetOpacity(1.0f);
        } else {
            accentBrush_->SetOpacity(activity.kind == IslandKind::Idle ? 0.18f : 0.34f);
            target_->DrawRoundedRectangle(pill, accentBrush_.Get(), 1.0f);
            accentBrush_->SetOpacity(1.0f);
        }

        if (!settings.w11Style) {
            ComPtr<ID2D1SolidColorBrush> highlight;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.10f * settingsOpacity_), &highlight);
            target_->DrawRoundedRectangle(
                D2D1::RoundedRect(D2D1::RectF(rect.left + 1, rect.top + 1, rect.right - 1,
                                              rect.bottom - 1),
                                  radius - 1, radius - 1),
                highlight.Get(), 1.0f);
        }

        D2D1_MATRIX_3X2_F oldTransform;
        target_->GetTransform(&oldTransform);
        D2D1_POINT_2F pillCenter = D2D1::Point2F((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f);
        target_->SetTransform(D2D1::Matrix3x2F::Scale(settings.sizeScale, settings.sizeScale, pillCenter) * oldTransform);

        float invScale = 1.0f / settings.sizeScale;
        float unW = (rect.right - rect.left) * invScale;
        float unH = (rect.bottom - rect.top) * invScale;
        D2D1_RECT_F unscaledRect = D2D1::RectF(pillCenter.x - unW * 0.5f, pillCenter.y - unH * 0.5f, pillCenter.x + unW * 0.5f, pillCenter.y + unH * 0.5f);

        switch (activity.kind) {
            case IslandKind::Media:
                DrawMedia(state, unscaledRect, now, scale);
                break;
            case IslandKind::Clipboard:
                DrawClipboard(state, unscaledRect);
                break;
            case IslandKind::Notification:
                DrawNotification(state, unscaledRect);
                break;
            case IslandKind::Volume:
                DrawVolume(state, unscaledRect);
                break;
            case IslandKind::CapsLock:
                DrawCapsLock(state, unscaledRect);
                break;
            case IslandKind::Device:
                DrawDevice(state, unscaledRect);
                break;
            case IslandKind::BatteryLow:
                DrawBattery(state, unscaledRect);
                break;
            case IslandKind::Progress:
                DrawProgress(state, unscaledRect);
                break;
            case IslandKind::Timer:
                DrawTimer(state, unscaledRect, scale);
                break;
            case IslandKind::Idle:
            default:
                DrawIdleDashboard(state, unscaledRect, settings, now, scale);
                break;
        }

        // Draw Settings gear icon if expanded
        if (unH > 50.0f) {
            bool gearHover = false;
            POINT ptCursor;
            if (GetCursorPos(&ptCursor)) {
                ScreenToClient(hwnd_, &ptCursor);
                float pillRight = bitmapWidth_ - 28.0f;
                float pillTop = 22.0f;
                float scale = settings.sizeScale;

                float gearLeft = pillRight - 44.0f * scale;
                float gearRight = pillRight - 8.0f * scale;
                float gearTop = pillTop + 6.0f * scale;
                float gearBottom = pillTop + 42.0f * scale;
                
                if (ptCursor.x >= gearLeft && ptCursor.x <= gearRight && 
                    ptCursor.y >= gearTop && ptCursor.y <= gearBottom) {
                    gearHover = true;
                }
            }

            ComPtr<ID2D1SolidColorBrush> gearBrush;
            D2D1_COLOR_F gearColor = settings.textPrimaryColor;
            gearColor.a = (gearHover ? 0.85f : 0.35f) * settingsOpacity_;
            target_->CreateSolidColorBrush(gearColor, &gearBrush);

            if (gearBrush && iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                D2D1_RECT_F gearRect = D2D1::RectF(unscaledRect.right - 44.0f, unscaledRect.top + 6.0f,
                                                   unscaledRect.right - 8.0f, unscaledRect.top + 42.0f);
                target_->DrawTextW(L"\uE713", 1, iconFormat_.Get(), &gearRect, gearBrush.Get());
            }
        }

        // â”€â”€ Apple-style privacy indicator dots â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        // Green dot = camera in use, Orange dot = mic in use.
        // Drawn in top-right corner of pill, outside content area.
        DrawPrivacyDots(state, unscaledRect, now);

        if (settings.colorfulModules) {
            lerpedAccent_ = originalAccent;
            if (accentBrush_) {
                accentBrush_->SetColor(lerpedAccent_);
            }
        }

        target_->SetTransform(oldTransform);
    }

    void DrawSoftShadow(D2D1_RECT_F rect, float radius) {
        // Draw nested layers to create a soft, high-quality drop shadow
        // combined with a subtle ambient glow reflecting the current accent color.
        const int steps = 5;
        const float maxOffset = 8.0f * g_settings.sizeScale;
        
        for (int i = 1; i <= steps; ++i) {
            float offset = (static_cast<float>(i) / steps) * maxOffset;
            float factor = (1.0f - (static_cast<float>(i - 1) / steps));
            
            // Base shadow (black, offset slightly down for Win11 elevation look)
            float baseOpacity = factor * 0.07f * settingsOpacity_;
            D2D1_RECT_F shadowRect = D2D1::RectF(rect.left - offset, rect.top - offset + 1.5f * g_settings.sizeScale,
                                                 rect.right + offset, rect.bottom + offset + 1.5f * g_settings.sizeScale);
            
            ComPtr<ID2D1SolidColorBrush> shadowBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, baseOpacity), &shadowBrush);
            if (shadowBrush) {
                target_->FillRoundedRectangle(D2D1::RoundedRect(shadowRect, radius + offset, radius + offset), shadowBrush.Get());
            }

            // Ambient accent glow
            float glowOpacity = factor * 0.035f * settingsOpacity_;
            if (glowOpacity > 0.001f) {
                ComPtr<ID2D1SolidColorBrush> glowBrush;
                D2D1_COLOR_F glowColor = lerpedAccent_;
                glowColor.a = glowOpacity;
                target_->CreateSolidColorBrush(glowColor, &glowBrush);
                if (glowBrush) {
                    target_->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect.left - offset, rect.top - offset,
                                                                               rect.right + offset, rect.bottom + offset),
                                                                   radius + offset, radius + offset), glowBrush.Get());
                }
            }
        }
    }

    // Apple Dynamic Island privacy dots.
    // Placed inside the pill near the top-right edge â€” pulsing glow like iPhone.
    void DrawPrivacyDots(const SharedState& state, D2D1_RECT_F rect, double now) {
        const bool mic = state.system.micActive;
        const bool cam = state.system.cameraActive;
        if (!mic && !cam) return;

        // Soft breathing pulse (0.75 Hz like iPhone).
        const float pulse = 0.72f + 0.28f * std::sin(static_cast<float>(now * 2.0 * 3.14159265 * 0.75));

        const float dotR   = 4.5f;   // dot radius
        const float gap    = 5.5f;   // gap between dots
        const float margin = 10.0f;  // from right edge
        const float dotY   = rect.top + (rect.bottom - rect.top) * 0.5f;

        float x = rect.right - margin - dotR;

        // Green = camera (rightmost when both active).
        if (cam) {
            ComPtr<ID2D1SolidColorBrush> camBrush;
            // Bright iOS camera green.
            target_->CreateSolidColorBrush(
                D2D1::ColorF(0.133f, 0.776f, 0.239f, pulse * settingsOpacity_), &camBrush);
            if (camBrush) {
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, dotY), dotR, dotR),
                                     camBrush.Get());
                // Glow halo.
                ComPtr<ID2D1SolidColorBrush> glow;
                target_->CreateSolidColorBrush(
                    D2D1::ColorF(0.133f, 0.776f, 0.239f, 0.18f * pulse * settingsOpacity_), &glow);
                if (glow) {
                    target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, dotY),
                                                        dotR * 2.2f, dotR * 2.2f), glow.Get());
                }
            }
            x -= (dotR * 2.0f + gap);
        }

        // Orange = microphone.
        if (mic) {
            ComPtr<ID2D1SolidColorBrush> micBrush;
            // iOS orange.
            target_->CreateSolidColorBrush(
                D2D1::ColorF(1.0f, 0.584f, 0.0f, pulse * settingsOpacity_), &micBrush);
            if (micBrush) {
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, dotY), dotR, dotR),
                                     micBrush.Get());
                ComPtr<ID2D1SolidColorBrush> glow;
                target_->CreateSolidColorBrush(
                    D2D1::ColorF(1.0f, 0.584f, 0.0f, 0.18f * pulse * settingsOpacity_), &glow);
                if (glow) {
                    target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, dotY),
                                                        dotR * 2.2f, dotR * 2.2f), glow.Get());
                }
            }
        }
    }

    void DrawPillSurface(D2D1_RECT_F rect, float radius, IslandKind kind, bool w11Style, int colorTheme) {
        if (tintBrush_) {
            target_->FillRoundedRectangle(D2D1::RoundedRect(rect, radius, radius),
                                          tintBrush_.Get());
        }

        // Draw pill background (OLED/Theme colors)
        ComPtr<ID2D1SolidColorBrush> bgBrush;
        D2D1_COLOR_F bg = pillBgColor_;
        bg.a = settingsOpacity_;
        target_->CreateSolidColorBrush(bg, &bgBrush);
        if (bgBrush) {
            target_->FillRoundedRectangle(D2D1::RoundedRect(rect, radius, radius), bgBrush.Get());
        }

        // Theme-specific borders
        if (colorTheme == 7) {
            // Glassmorphism: dual-layer bright glowing border for premium glass look
            ComPtr<ID2D1SolidColorBrush> border;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.38f), &border);
            ComPtr<ID2D1SolidColorBrush> glowBorder;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.12f), &glowBorder);
            if (border) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 0.5f, rect.top + 0.5f, rect.right - 0.5f, rect.bottom - 0.5f),
                    radius, radius), border.Get(), 1.0f);
            }
            if (glowBorder) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left - 0.5f, rect.top - 0.5f, rect.right + 0.5f, rect.bottom + 0.5f),
                    radius, radius), glowBorder.Get(), 2.0f);
            }
        } else if (colorTheme == 1 || colorTheme == 10) {
            // OLED / AMOLED Black: Crisp solid dark grey border outline (no transparency)
            ComPtr<ID2D1SolidColorBrush> border;
            target_->CreateSolidColorBrush(D2D1::ColorF(0.180f, 0.180f, 0.200f, 1.0f), &border); // #2E2E33
            if (border) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 0.5f, rect.top + 0.5f, rect.right - 0.5f, rect.bottom - 0.5f),
                    radius, radius), border.Get(), 1.0f);
            }
        } else if (colorTheme == 8) {
            // Cyberpunk: dual-tone Neon Cyan and Neon Purple outline glow
            ComPtr<ID2D1SolidColorBrush> borderCyan;
            target_->CreateSolidColorBrush(D2D1::ColorF(0.0f, 1.0f, 1.0f, 0.70f), &borderCyan);
            ComPtr<ID2D1SolidColorBrush> borderPurple;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 1.0f, 0.70f), &borderPurple);
            
            if (borderCyan && borderPurple) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 0.5f, rect.top + 0.5f, rect.right - 0.5f, rect.bottom - 0.5f),
                    radius, radius), borderCyan.Get(), 1.2f);
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 1.2f, rect.top + 1.2f, rect.right - 1.2f, rect.bottom - 1.2f),
                    radius, radius), borderPurple.Get(), 0.8f);
            }
        } else if (colorTheme == 11) {
            // Holographic AI: tech Neon Cyan outline
            ComPtr<ID2D1SolidColorBrush> borderCyan;
            target_->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.898f, 1.0f, 0.50f), &borderCyan);
            if (borderCyan) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 0.5f, rect.top + 0.5f, rect.right - 0.5f, rect.bottom - 0.5f),
                    radius, radius), borderCyan.Get(), 1.0f);
            }
        } else if (w11Style || colorTheme == 5) {
            // Windows 11 Fluent style: white 15% opacity subtle border
            ComPtr<ID2D1SolidColorBrush> border;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.15f * settingsOpacity_), &border);
            if (border) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 0.5f, rect.top + 0.5f, rect.right - 0.5f, rect.bottom - 0.5f),
                    radius, radius), border.Get(), 1.0f);
            }
        } else {
            // Default Apple Notch Glass style: Top gloss line and thin 8% white outline
            ComPtr<ID2D1SolidColorBrush> gloss;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.055f * settingsOpacity_), &gloss);
            if (gloss) {
                D2D1_RECT_F glossLine = D2D1::RectF(rect.left + radius, rect.top + 0.5f,
                                                    rect.right - radius, rect.top + 1.2f);
                target_->FillRectangle(glossLine, gloss.Get());
            }

            ComPtr<ID2D1SolidColorBrush> border;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.08f * settingsOpacity_), &border);
            if (border) {
                target_->DrawRoundedRectangle(D2D1::RoundedRect(
                    D2D1::RectF(rect.left + 0.5f, rect.top + 0.5f, rect.right - 0.5f, rect.bottom - 0.5f),
                    radius, radius), border.Get(), 0.8f);
            }
        }
    }

    void DrawAccentGlow(D2D1_RECT_F rect, const Activity& activity, double now) {
        float opacity = activity.kind == IslandKind::Media ? 0.23f : 0.12f;
        if (activity.kind == IslandKind::BatteryLow) {
            redBrush_->SetOpacity(0.18f);
            target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.right - 38, rect.top + 20), 56, 36),
                                 redBrush_.Get());
            redBrush_->SetOpacity(1.0f);
            return;
        }

        opacity += 0.05f * (0.5f + 0.5f * std::sin(static_cast<float>(now * 1.7)));
        accentBrush_->SetOpacity(opacity);
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.left + 48, rect.top + 10), 70, 42),
                             accentBrush_.Get());
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.right - 58, rect.bottom - 8), 76, 42),
                             accentBrush_.Get());
        accentBrush_->SetOpacity(1.0f);
    }

    static void GetWeatherIconAndText(int code, std::wstring& icon, std::wstring& text) {
        switch (code) {
            case 113: icon = L"\x2600\xFE0F"; break; // Sun
            case 116: icon = L"\x26C5"; break;        // Cloud/Sun
            case 119: case 122: icon = L"\x2601\xFE0F"; break; // Cloud
            case 143: case 248: case 260: icon = L"\xD83C\xDF2B\xFE0F"; break; // Fog
            case 200: case 386: case 389: case 392: case 395: icon = L"\x26C8\xFE0F"; break; // Thunder
            case 176: case 263: case 266: case 281: case 284: case 293: case 296: case 299: case 302: case 305: case 308: case 311: case 314: case 353: case 356: case 359: icon = L"\xD83C\xDF27\xFE0F"; break; // Rain
            case 179: case 182: case 185: case 227: case 230: case 317: case 320: case 323: case 326: case 329: case 332: case 335: case 338: case 350: case 362: case 365: case 368: case 371: icon = L"\x2744\xFE0F"; break; // Snow
            default: icon = L"\xD83C\xDF21\xFE0F"; break; // Thermometer
        }
    }

    static int GetDaysInMonth(int year, int month) {
        if (month == 2) {
            bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            return leap ? 29 : 28;
        }
        if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
        return 31;
    }

    static int GetDayOfWeek(int year, int month, int day) {
        if (month < 3) { month += 12; year -= 1; }
        int k = year % 100;
        int j = year / 100;
        int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
        return (h + 6) % 7;
    }

    void DrawCard(D2D1_RECT_F rect, float scale, D2D1_COLOR_F baseColor, int colorTheme, float bgOpacity = 0.05f, float borderOpacity = 0.12f) {
        // 1. Shadows (to create a floating appearance)
        ComPtr<ID2D1SolidColorBrush> shadowBrush;
        int shadowLayers = (colorTheme == 1 || colorTheme == 10 || colorTheme == 7) ? 5 : 3;
        float shadowAlphaBase = (colorTheme == 7) ? 0.05f : 0.04f;
        for (int i = 1; i <= shadowLayers; ++i) {
            float offset = static_cast<float>(i) * 0.9f * scale;
            D2D1_RECT_F shadowRect = D2D1::RectF(rect.left - offset, rect.top - offset + 1.2f * scale, rect.right + offset, rect.bottom + offset + 1.2f * scale);
            target_->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, shadowAlphaBase / static_cast<float>(i)), &shadowBrush);
            if (shadowBrush) {
                target_->FillRoundedRectangle(D2D1::RoundedRect(shadowRect, 10.0f * scale, 10.0f * scale), shadowBrush.Get());
            }
        }

        // 2. Card Background
        ComPtr<ID2D1SolidColorBrush> cardBg;
        D2D1_COLOR_F bgCol = baseColor;
        if (colorTheme == 1 || colorTheme == 10) {
            // OLED/AMOLED: Solid charcoal grey for cards against pure opaque black background
            bgCol = D2D1::ColorF(0.082f, 0.082f, 0.090f, 1.0f); // #151517
        } else if (colorTheme == 7) {
            // Glassmorphism: Frosted white card base
            bgCol = D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.12f);
        } else {
            bgCol.a = bgOpacity * settingsOpacity_;
        }
        target_->CreateSolidColorBrush(bgCol, &cardBg);
        if (cardBg) {
            target_->FillRoundedRectangle(D2D1::RoundedRect(rect, 10.0f * scale, 10.0f * scale), cardBg.Get());
        }

        // 3. Card Border / Separators
        ComPtr<ID2D1SolidColorBrush> cardBorder;
        D2D1_COLOR_F borderCol = baseColor;
        if (colorTheme == 1 || colorTheme == 10) {
            // OLED/AMOLED: Crisp solid gray separator border
            borderCol = D2D1::ColorF(0.180f, 0.180f, 0.200f, 1.0f); // #2E2E33
        } else if (colorTheme == 7) {
            // Glassmorphism: Soft glowing borders
            borderCol = D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.28f);
        } else {
            borderCol.a = borderOpacity * settingsOpacity_;
        }
        target_->CreateSolidColorBrush(borderCol, &cardBorder);
        if (cardBorder) {
            target_->DrawRoundedRectangle(D2D1::RoundedRect(rect, 10.0f * scale, 10.0f * scale), cardBorder.Get(), 1.0f * scale);
        }

        // 4. Subtle reflections and top-light highlights for Glassmorphism
        if (colorTheme == 7) {
            ComPtr<ID2D1SolidColorBrush> highlightBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.25f), &highlightBrush);
            if (highlightBrush) {
                D2D1_RECT_F highlightLine = D2D1::RectF(rect.left + 8.0f * scale, rect.top + 0.5f * scale,
                                                        rect.right - 8.0f * scale, rect.top + 1.2f * scale);
                target_->FillRectangle(highlightLine, highlightBrush.Get());
            }
        }
    }

    void DrawSystemCard(D2D1_RECT_F cr, const wchar_t* title, const std::wstring& valueStr, int progressPercent, const wchar_t* emoji, D2D1_COLOR_F baseColor, float scale, int colorTheme) {
        DrawCard(cr, scale, baseColor, colorTheme, 0.05f, 0.12f);

        float iconCX = cr.left + 20.0f * scale;
        float iconCY = cr.top + 28.0f * scale;
        float r = 12.0f * scale;
        ComPtr<ID2D1SolidColorBrush> circleBrush;
        D2D1_COLOR_F circCol = baseColor; circCol.a = 0.12f * settingsOpacity_;
        target_->CreateSolidColorBrush(circCol, &circleBrush);
        if (circleBrush) {
            target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(iconCX, iconCY), r, r), circleBrush.Get());
        }

        D2D1_RECT_F emojiRect = D2D1::RectF(iconCX - 8.0f * scale, iconCY - 8.0f * scale, iconCX + 8.0f * scale, iconCY + 8.0f * scale);
        target_->DrawTextW(emoji, static_cast<UINT32>(wcslen(emoji)), smallTextFormat_.Get(), emojiRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

        D2D1_RECT_F titleRect = D2D1::RectF(cr.left + 40.0f * scale, cr.top + 8.0f * scale, cr.right - 6.0f * scale, cr.top + 22.0f * scale);
        mutedBrush_->SetOpacity(0.6f);
        target_->DrawTextW(title, static_cast<UINT32>(wcslen(title)), smallTextFormat_.Get(), titleRect, mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        mutedBrush_->SetOpacity(0.9f);

        D2D1_RECT_F valueRect = D2D1::RectF(cr.left + 40.0f * scale, cr.top + 22.0f * scale, cr.right - 6.0f * scale, cr.bottom - 16.0f * scale);
        textBrush_->SetOpacity(0.95f);
        target_->DrawTextW(valueStr.c_str(), static_cast<UINT32>(valueStr.length()), textFormat_.Get(), valueRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        textBrush_->SetOpacity(1.0f);

        D2D1_RECT_F meterTrack = D2D1::RectF(cr.left + 10.0f * scale, cr.bottom - 10.0f * scale, cr.right - 10.0f * scale, cr.bottom - 7.0f * scale);
        ComPtr<ID2D1SolidColorBrush> trackBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.1f), &trackBg);
        if (trackBg) {
            target_->FillRoundedRectangle(D2D1::RoundedRect(meterTrack, 1.5f * scale, 1.5f * scale), trackBg.Get());
        }

        if (progressPercent >= 0) {
            float clamped = Clamp(progressPercent / 100.0f, 0.0f, 1.0f);
            D2D1_RECT_F meterFill = D2D1::RectF(meterTrack.left, meterTrack.top, meterTrack.left + (meterTrack.right - meterTrack.left) * clamped, meterTrack.bottom);
            ComPtr<ID2D1SolidColorBrush> fillBrush;
            target_->CreateSolidColorBrush(baseColor, &fillBrush);
            if (fillBrush) {
                target_->FillRoundedRectangle(D2D1::RoundedRect(meterFill, 1.5f * scale, 1.5f * scale), fillBrush.Get());
            }
        }
    }

    void DrawSystemDashboard(const SharedState& state, D2D1_RECT_F rect, const Settings& settings, double now, float scale) {
        textBrush_->SetOpacity(0.96f);
        D2D1_RECT_F titleRect = D2D1::RectF(rect.left + 24.0f * scale, rect.top + 12.0f * scale, rect.right, rect.top + 32.0f * scale);
        target_->DrawTextW(L"System Status", 13, boldTextFormat_.Get(), titleRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        const float cardW = 118.0f * scale;
        const float cardH = 72.0f * scale;
        const float gapX = 12.0f * scale;
        const float gapY = 10.0f * scale;
        const float startX = rect.left + ((rect.right - rect.left) - (cardW * 3.0f + gapX * 2.0f)) * 0.5f;

        const float row1Top = rect.top + 42.0f * scale;
        const float row2Top = row1Top + cardH + gapY;

        // Card 1: CPU
        DrawSystemCard(D2D1::RectF(startX, row1Top, startX + cardW, row1Top + cardH),
                       L"CPU", std::to_wstring(state.system.cpuPercent) + L"%", state.system.cpuPercent,
                       L"\x2699\xFE0F", D2D1::ColorF(1.0f, 0.45f, 0.0f, 1.0f), scale, settings.colorTheme);

        // Card 2: RAM
        DrawSystemCard(D2D1::RectF(startX + cardW + gapX, row1Top, startX + cardW + gapX + cardW, row1Top + cardH),
                       L"RAM", std::to_wstring(state.system.memoryPercent) + L"%", state.system.memoryPercent,
                       L"\xD83D\xDCBE", D2D1::ColorF(0.0f, 0.55f, 1.0f, 1.0f), scale, settings.colorTheme);

        // Card 3: GPU
        int gpuVal = state.system.gpuPercent >= 0 ? state.system.gpuPercent : 0;
        DrawSystemCard(D2D1::RectF(startX + (cardW + gapX) * 2.0f, row1Top, startX + (cardW + gapX) * 2.0f + cardW, row1Top + cardH),
                       L"GPU", std::to_wstring(gpuVal) + L"%", gpuVal,
                       L"\xD83C\xDFAE", D2D1::ColorF(0.7f, 0.35f, 1.0f, 1.0f), scale, settings.colorTheme);

        // Card 4: Battery
        D2D1_COLOR_F batColor = D2D1::ColorF(0.15f, 0.82f, 0.35f, 1.0f);
        if (state.battery.low) {
            batColor = D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f);
        } else if (state.battery.charging) {
            batColor = D2D1::ColorF(0.18f, 0.8f, 0.44f, 1.0f);
        }
        std::wstring batVal = state.battery.percent >= 0 ? std::to_wstring(state.battery.percent) + L"%" : L"N/A";
        DrawSystemCard(D2D1::RectF(startX, row2Top, startX + cardW, row2Top + cardH),
                       L"BAT", batVal, state.battery.percent >= 0 ? state.battery.percent : 0,
                       L"\xD83D\xDD0B", batColor, scale, settings.colorTheme);

        // Card 5: Network Status
        std::wstring netVal = state.system.internetConnected ? L"Connected" : L"Offline";
        D2D1_COLOR_F netColor = state.system.internetConnected ? D2D1::ColorF(0.0f, 0.78f, 0.82f, 1.0f) : D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f);
        DrawSystemCard(D2D1::RectF(startX + cardW + gapX, row2Top, startX + cardW + gapX + cardW, row2Top + cardH),
                       L"NET", netVal, state.system.internetConnected ? 100 : 0,
                       L"\xD83C\xDF10", netColor, scale, settings.colorTheme);

        // Card 6: Bluetooth Status
        std::wstring btVal = state.system.bluetoothOn ? L"Enabled" : L"Disabled";
        D2D1_COLOR_F btColor = state.system.bluetoothOn ? D2D1::ColorF(1.0f, 0.2f, 0.45f, 1.0f) : D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f);
        DrawSystemCard(D2D1::RectF(startX + (cardW + gapX) * 2.0f, row2Top, startX + (cardW + gapX) * 2.0f + cardW, row2Top + cardH),
                       L"BT", btVal, state.system.bluetoothOn ? 100 : 0,
                       L"\xD83D\xDCF6", btColor, scale, settings.colorTheme);
    }

    void DrawCalendarDashboard(const SharedState& state, D2D1_RECT_F rect, const Settings& settings, double now, float scale, SYSTEMTIME& local) {
        textBrush_->SetOpacity(0.96f);
        D2D1_RECT_F titleRect = D2D1::RectF(rect.left + 24.0f * scale, rect.top + 12.0f * scale, rect.right, rect.top + 32.0f * scale);
        target_->DrawTextW(L"Calendar & Date", 15, boldTextFormat_.Get(), titleRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Left Card: Date and Time Card
        D2D1_RECT_F leftCard = D2D1::RectF(rect.left + 20.0f * scale, rect.top + 38.0f * scale, rect.left + 192.0f * scale, rect.bottom - 16.0f * scale);
        DrawCard(leftCard, scale, D2D1::ColorF(0.92f, 0.16f, 0.16f, 1.0f), settings.colorTheme, 0.05f, 0.12f);

        // Calendar Icon (colored font)
        D2D1_RECT_F calIconRect = D2D1::RectF(leftCard.left + 14.0f * scale, leftCard.top + 14.0f * scale, leftCard.left + 44.0f * scale, leftCard.top + 44.0f * scale);
        target_->DrawTextW(L"\xD83D\xDCC5", 2, clockFormat_.Get(), calIconRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

        // Current time format
        wchar_t timeBuf[32] = {};
        GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, TIME_NOSECONDS, &local, nullptr, timeBuf, ARRAYSIZE(timeBuf));

        D2D1_RECT_F timeRect = D2D1::RectF(leftCard.left + 12.0f * scale, leftCard.top + 46.0f * scale, leftCard.right - 12.0f * scale, leftCard.top + 82.0f * scale);
        target_->DrawTextW(timeBuf, static_cast<UINT32>(wcslen(timeBuf)), clockFormat_.Get(), timeRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Day and Month/Date text
        wchar_t dateBuf[64] = {};
        GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_LONGDATE, &local, nullptr, dateBuf, ARRAYSIZE(dateBuf), nullptr);
        
        D2D1_RECT_F dateRect = D2D1::RectF(leftCard.left + 12.0f * scale, leftCard.top + 88.0f * scale, leftCard.right - 12.0f * scale, leftCard.bottom - 12.0f * scale);
        // Left-aligned date text below the time
        target_->DrawTextW(dateBuf, static_cast<UINT32>(wcslen(dateBuf)), textFormat_.Get(), dateRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Right Card: Month Grid
        D2D1_RECT_F rightCard = D2D1::RectF(rect.left + 204.0f * scale, rect.top + 38.0f * scale, rect.right - 20.0f * scale, rect.bottom - 16.0f * scale);
        DrawCard(rightCard, scale, D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), settings.colorTheme, 0.04f, 0.1f);

        const float gridStart = rightCard.left + 12.0f * scale;
        const float gridTop = rightCard.top + 16.0f * scale;
        const float colW = 24.0f * scale;
        const float rowH = 21.0f * scale;
        const wchar_t* days[] = {L"S", L"M", L"T", L"W", L"T", L"F", L"S"};

        ComPtr<ID2D1SolidColorBrush> redBanner;
        target_->CreateSolidColorBrush(D2D1::ColorF(0.92f, 0.16f, 0.16f, 1.0f), &redBanner);

        // Draw weekdays header
        for (int i = 0; i < 7; ++i) {
            D2D1_RECT_F cell = D2D1::RectF(gridStart + i * colW, gridTop, gridStart + (i+1)*colW, gridTop + rowH);
            ComPtr<ID2D1SolidColorBrush> brush = (i == 0 || i == 6) ? redBanner : mutedBrush_;
            target_->DrawTextW(days[i], 1, boldTextFormat_.Get(), cell, brush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        }

        int startDay = GetDayOfWeek(local.wYear, local.wMonth, 1);
        int totalDays = GetDaysInMonth(local.wYear, local.wMonth);
        
        int row = 1;
        int col = startDay;
        textBrush_->SetOpacity(0.95f);
        for (int d = 1; d <= totalDays; ++d) {
            D2D1_RECT_F cell = D2D1::RectF(gridStart + col * colW, gridTop + row * rowH + 2.0f * scale, 
                                           gridStart + (col+1)*colW, gridTop + (row+1)*rowH + 2.0f * scale);
            
            if (d == local.wDay) {
                // Today highlight circle
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cell.left + colW*0.5f, cell.top + rowH*0.5f), 9.0f*scale, 9.0f*scale), redBanner.Get());
                
                ComPtr<ID2D1SolidColorBrush> whiteDayBrush;
                target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 1), &whiteDayBrush);
                target_->DrawTextW(std::to_wstring(d).c_str(), static_cast<UINT32>(std::to_wstring(d).length()), boldTextFormat_.Get(), cell, whiteDayBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
            } else {
                ComPtr<ID2D1SolidColorBrush> dBrush = (col == 0 || col == 6) ? redBanner : textBrush_;
                if (col == 0 || col == 6) {
                    dBrush->SetOpacity(0.55f);
                } else {
                    dBrush->SetOpacity(0.90f);
                }
                target_->DrawTextW(std::to_wstring(d).c_str(), static_cast<UINT32>(std::to_wstring(d).length()), boldTextFormat_.Get(), cell, dBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
            }
            
            col++;
            if (col > 6) { col = 0; row++; }
        }
        textBrush_->SetOpacity(1.0f);
    }

    void DrawWeatherDashboard(const SharedState& state, D2D1_RECT_F rect, const Settings& settings, double now, float scale, bool hasWeather, const std::wstring& wIcon, const std::wstring& wText) {
        textBrush_->SetOpacity(0.96f);
        D2D1_RECT_F titleRect = D2D1::RectF(rect.left + 24.0f * scale, rect.top + 12.0f * scale, rect.right, rect.top + 32.0f * scale);
        target_->DrawTextW(L"Weather Forecast", 16, boldTextFormat_.Get(), titleRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        wchar_t wTemp[32] = {};
        if (hasWeather) swprintf_s(wTemp, L"%.0f\x00B0", state.weather.temperature);
        else wcscpy_s(wTemp, L"--\x00B0");

        std::wstring city = hasWeather ? state.weather.city : L"Locating...";
        std::wstring desc = wText;

        // Left Card: Temp, City, Condition, High/Low
        D2D1_RECT_F leftCard = D2D1::RectF(rect.left + 20.0f * scale, rect.top + 38.0f * scale, rect.left + 192.0f * scale, rect.bottom - 16.0f * scale);
        DrawCard(leftCard, scale, D2D1::ColorF(1.0f, 0.76f, 0.03f, 1.0f), settings.colorTheme, 0.05f, 0.12f);

        // Location City Name
        D2D1_RECT_F cityRect = D2D1::RectF(leftCard.left + 12.0f * scale, leftCard.top + 10.0f * scale, leftCard.right - 12.0f * scale, leftCard.top + 28.0f * scale);
        target_->DrawTextW(city.c_str(), static_cast<UINT32>(city.length()), textFormat_.Get(), cityRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Temperature (very large)
        D2D1_RECT_F tempRect = D2D1::RectF(leftCard.left + 12.0f * scale, leftCard.top + 28.0f * scale, leftCard.left + 110.0f * scale, leftCard.top + 98.0f * scale);
        target_->DrawTextW(wTemp, static_cast<UINT32>(wcslen(wTemp)), hugeTextFormat_.Get(), tempRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Weather Icon (dynamic, aligned to right of temperature)
        D2D1_RECT_F iconRect = D2D1::RectF(leftCard.left + 116.0f * scale, leftCard.top + 34.0f * scale, leftCard.right - 12.0f * scale, leftCard.top + 84.0f * scale);
        target_->DrawTextW(wIcon.c_str(), static_cast<UINT32>(wIcon.length()), clockFormat_.Get(), iconRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

        // Condition Description
        D2D1_RECT_F descRect = D2D1::RectF(leftCard.left + 12.0f * scale, leftCard.bottom - 46.0f * scale, leftCard.right - 12.0f * scale, leftCard.bottom - 28.0f * scale);
        target_->DrawTextW(desc.c_str(), static_cast<UINT32>(desc.length()), smallTextFormat_.Get(), descRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // High / Low temp range
        std::wstring highLowStr = L"--\x00B0 / --\x00B0";
        if (hasWeather && !state.weather.tempMax.empty() && !state.weather.tempMin.empty()) {
            highLowStr = L"H: " + state.weather.tempMax + L"\x00B0  L: " + state.weather.tempMin + L"\x00B0";
        }
        D2D1_RECT_F rangeRect = D2D1::RectF(leftCard.left + 12.0f * scale, leftCard.bottom - 26.0f * scale, leftCard.right - 12.0f * scale, leftCard.bottom - 8.0f * scale);
        mutedBrush_->SetOpacity(0.7f);
        target_->DrawTextW(highLowStr.c_str(), static_cast<UINT32>(highLowStr.length()), smallTextFormat_.Get(), rangeRect, mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        mutedBrush_->SetOpacity(0.9f);

        // Right Column: 4 Mini detail cards
        float tx = rect.left + 204.0f * scale;
        float cardW = 94.0f * scale;
        float cardH = 72.0f * scale;
        float gapX = 10.0f * scale;
        float gapY = 10.0f * scale;

        auto DrawWeatherMiniCard = [&](D2D1_RECT_F cr, const wchar_t* title, const std::wstring& value, const wchar_t* emoji, D2D1_COLOR_F baseColor) {
            DrawCard(cr, scale, baseColor, settings.colorTheme, 0.05f, 0.12f);

            D2D1_RECT_F tr = D2D1::RectF(cr.left + 8.0f * scale, cr.top + 8.0f * scale, cr.right - 8.0f * scale, cr.top + 24.0f * scale);
            std::wstring fullTitle = std::wstring(emoji) + L" " + title;
            mutedBrush_->SetOpacity(0.6f);
            target_->DrawTextW(fullTitle.c_str(), static_cast<UINT32>(fullTitle.size()), smallTextFormat_.Get(), tr, mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
            mutedBrush_->SetOpacity(0.9f);

            D2D1_RECT_F vr = D2D1::RectF(cr.left + 8.0f * scale, cr.top + 28.0f * scale, cr.right - 8.0f * scale, cr.bottom - 8.0f * scale);
            textBrush_->SetOpacity(0.95f);
            target_->DrawTextW(value.c_str(), static_cast<UINT32>(value.size()), textFormat_.Get(), vr, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
            textBrush_->SetOpacity(1.0f);
        };

        // Wind Speed Card
        D2D1_RECT_F windCard = D2D1::RectF(tx, rect.top + 38.0f * scale, tx + cardW, rect.top + 38.0f * scale + cardH);
        std::wstring windVal = hasWeather ? state.weather.windSpeed + (settings.weatherFahrenheit ? L" mph" : L" km/h") : L"--";
        DrawWeatherMiniCard(windCard, L"WIND", windVal, L"\xD83D\xDCA8", D2D1::ColorF(0.0f, 0.7f, 1.0f, 1.0f));

        // Feels Like Card
        float tx2 = tx + cardW + gapX;
        D2D1_RECT_F feelsCard = D2D1::RectF(tx2, rect.top + 38.0f * scale, tx2 + cardW, rect.top + 38.0f * scale + cardH);
        std::wstring feelsVal = hasWeather ? state.weather.feelsLike + L"\x00B0" : L"--";
        DrawWeatherMiniCard(feelsCard, L"FEELS", feelsVal, L"\xD83C\xDF21\xFE0F", D2D1::ColorF(1.0f, 0.5f, 0.0f, 1.0f));

        // Humidity Card
        D2D1_RECT_F humCard = D2D1::RectF(tx, rect.top + 38.0f * scale + cardH + gapY, tx + cardW, rect.top + 38.0f * scale + cardH * 2.0f + gapY);
        std::wstring humVal = hasWeather ? state.weather.humidity + L"%" : L"--";
        DrawWeatherMiniCard(humCard, L"HUMID", humVal, L"\xD83D\xDCA7", D2D1::ColorF(0.0f, 0.8f, 0.4f, 1.0f));

        // UV Index Card
        D2D1_RECT_F uvCard = D2D1::RectF(tx2, rect.top + 38.0f * scale + cardH + gapY, tx2 + cardW, rect.top + 38.0f * scale + cardH * 2.0f + gapY);
        std::wstring uvVal = hasWeather ? (state.weather.uvIndex.empty() ? L"0" : state.weather.uvIndex) : L"--";
        DrawWeatherMiniCard(uvCard, L"UV IDX", uvVal, L"\x2600\xFE0F", D2D1::ColorF(1.0f, 0.8f, 0.0f, 1.0f));
    }

    void DrawToolsTrayDashboard(const SharedState& state, D2D1_RECT_F rect, const Settings& settings, double now, float scale) {
        textBrush_->SetOpacity(0.96f);

        // Get relative mouse position in client coordinates
        POINT cursor = {};
        GetCursorPos(&cursor);
        ScreenToClient(hwnd_, &cursor);

        // Calculate top-right toolbar button rectangles
        float topY = rect.top + 12.0f * scale;
        float btnSize = 22.0f * scale;
        float gap = 8.0f * scale;
        float startX = rect.right - 24.0f * scale;

        // Settings Button
        D2D1_RECT_F gearRect = D2D1::RectF(startX - btnSize, topY, startX, topY + btnSize);
        // Edit Button
        D2D1_RECT_F editRect = D2D1::RectF(startX - btnSize - (btnSize + gap), topY, startX - (btnSize + gap), topY + btnSize);
        // Add Button
        D2D1_RECT_F addRect = D2D1::RectF(startX - btnSize - 2 * (btnSize + gap), topY, startX - 2 * (btnSize + gap), topY + btnSize);

        bool hoverGear = (cursor.x >= gearRect.left && cursor.x <= gearRect.right && cursor.y >= gearRect.top && cursor.y <= gearRect.bottom);
        bool hoverEdit = (cursor.x >= editRect.left && cursor.x <= editRect.right && cursor.y >= editRect.top && cursor.y <= editRect.bottom);
        bool hoverAdd = (cursor.x >= addRect.left && cursor.x <= addRect.right && cursor.y >= addRect.top && cursor.y <= addRect.bottom);

        // Tooltip description based on hovered toolbar item
        std::wstring subtitle = L"";
        if (hoverGear) subtitle = L" \u2022 Settings";
        else if (hoverEdit) subtitle = g_toolsEditMode ? L" \u2022 Exit Edit Mode" : L" \u2022 Edit/Remove Utilities";
        else if (hoverAdd) subtitle = L" \u2022 Add a Utility";
        else if (g_toolsEditMode) subtitle = L" \u2022 Drag to Reorder";

        std::wstring titleText = L"Tools & Utilities" + subtitle;
        D2D1_RECT_F titleRect = D2D1::RectF(rect.left + 24.0f * scale, rect.top + 12.0f * scale, rect.right, rect.top + 32.0f * scale);
        
        target_->DrawTextW(titleText.c_str(), static_cast<UINT32>(titleText.length()), boldTextFormat_.Get(), titleRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Helper to draw circular button with hover highlight
        auto DrawToolbarIcon = [&](D2D1_RECT_F btnRect, const wchar_t* iconText, bool isHovered, bool isActive = false) {
            ComPtr<ID2D1SolidColorBrush> highlightBrush;
            D2D1_COLOR_F hCol = lerpedAccent_;
            if (isActive) {
                hCol.a = 0.25f;
            } else if (isHovered) {
                hCol.a = 0.15f;
            } else {
                hCol.a = 0.05f;
            }
            target_->CreateSolidColorBrush(hCol, &highlightBrush);
            if (highlightBrush) {
                float cx = (btnRect.left + btnRect.right) * 0.5f;
                float cy = (btnRect.top + btnRect.bottom) * 0.5f;
                float r = (btnRect.right - btnRect.left) * 0.5f;
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r, r), highlightBrush.Get());
            }

            D2D1_RECT_F iconTextRect = D2D1::RectF(btnRect.left, btnRect.top + 2.0f * scale, btnRect.right, btnRect.bottom);
            target_->DrawTextW(iconText, 1, smallTextFormat_.Get(), iconTextRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        };

        // Draw Toolbar Buttons
        DrawToolbarIcon(addRect, L"\x2795", hoverAdd); // ➕ Add
        DrawToolbarIcon(editRect, g_toolsEditMode ? L"\x2713" : L"\x2796", hoverEdit, g_toolsEditMode); // ➖/✓ Edit
        DrawToolbarIcon(gearRect, L"\x2699", hoverGear); // ⚙️ Settings

        // Grid layout calculation
        float startGridY = rect.top + 46.0f * scale;
        float cardW = 90.0f * scale;
        float cardH = 72.0f * scale;
        float gapX = 12.0f * scale;
        float gapY = 12.0f * scale;
        float marginX = 12.0f * scale;

        size_t totalItems = g_activeTools.size();

        // Lambda to draw a single card
        auto DrawSingleCard = [&](const std::wstring& toolId, D2D1_RECT_F cardRect, bool isHovered, bool isPlaceholder) {
            const ToolItem* item = FindToolById(toolId);
            if (!item) return;

            if (isPlaceholder) {
                ComPtr<ID2D1SolidColorBrush> placeholderBrush;
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.08f), &placeholderBrush);
                if (placeholderBrush) {
                    target_->DrawRoundedRectangle(D2D1::RoundedRect(cardRect, 12.0f * scale, 12.0f * scale), placeholderBrush.Get(), 1.0f * scale);
                }
                return;
            }

            // Apply hover lift
            if (isHovered) {
                cardRect.top -= 3.0f * scale;
                cardRect.bottom -= 3.0f * scale;
            }

            // Hover outer glow highlight
            if (isHovered) {
                ComPtr<ID2D1SolidColorBrush> glowBrush;
                D2D1_COLOR_F glowCol = lerpedAccent_; glowCol.a = 0.35f;
                target_->CreateSolidColorBrush(glowCol, &glowBrush);
                if (glowBrush) {
                    D2D1_RECT_F glowRect = D2D1::RectF(cardRect.left - 2.0f * scale, cardRect.top - 2.0f * scale, cardRect.right + 2.0f * scale, cardRect.bottom + 2.0f * scale);
                    target_->DrawRoundedRectangle(D2D1::RoundedRect(glowRect, 14.0f * scale, 14.0f * scale), glowBrush.Get(), 2.0f * scale);
                }
            }

            DrawCard(cardRect, scale, lerpedAccent_, settings.colorTheme, isHovered ? 0.08f : 0.05f, isHovered ? 0.22f : 0.12f);

            // Large colorful icon
            D2D1_RECT_F iconRect = D2D1::RectF(cardRect.left, cardRect.top + 8.0f * scale, cardRect.right, cardRect.top + 38.0f * scale);
            target_->DrawTextW(item->icon.c_str(), static_cast<UINT32>(item->icon.length()), clockFormat_.Get(), iconRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

            // Bigger text (14-16px) centered below
            D2D1_RECT_F labelRect = D2D1::RectF(cardRect.left + 4.0f * scale, cardRect.top + 40.0f * scale, cardRect.right - 4.0f * scale, cardRect.bottom - 6.0f * scale);
            target_->DrawTextW(item->name.c_str(), static_cast<UINT32>(item->name.length()), boldTextFormat_.Get(), labelRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

            // Deletion badge (❌) in Edit Mode
            if (g_toolsEditMode) {
                float bx = cardRect.right - 10.0f * scale;
                float by = cardRect.top + 10.0f * scale;
                float br = 7.0f * scale;

                ComPtr<ID2D1SolidColorBrush> redBrush;
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &redBrush);
                if (redBrush) {
                    target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(bx, by), br, br), redBrush.Get());
                }

                ComPtr<ID2D1SolidColorBrush> whiteBrush;
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &whiteBrush);
                if (whiteBrush) {
                    D2D1_RECT_F crossRect = D2D1::RectF(bx - 3.5f * scale, by - 3.5f * scale, bx + 3.5f * scale, by + 3.5f * scale);
                    target_->DrawTextW(L"\x2715", 1, smallTextFormat_.Get(), crossRect, whiteBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
                }
            }
        };

        // Draw active cards
        for (size_t i = 0; i < totalItems; ++i) {
            size_t r = i / 4;
            size_t c = i % 4;
            float x = rect.left + marginX + c * (cardW + gapX);
            float y = startGridY + r * (cardH + gapY);
            D2D1_RECT_F cardRect = D2D1::RectF(x, y, x + cardW, y + cardH);

            bool isCardHovered = (cursor.x >= cardRect.left && cursor.x <= cardRect.right && cursor.y >= cardRect.top && cursor.y <= cardRect.bottom);
            bool isDragged = false;

            DrawSingleCard(g_activeTools[i], cardRect, isCardHovered, isDragged);
        }
    }

    void DrawIdleDashboard(const SharedState& state, D2D1_RECT_F rect, const Settings& settings,
                           double now, float scale) {
        if (settings.gameOverlay || Wh_GetIntValue(L"GameOverlayPinned", 0) != 0) {
            DrawGameOverlay(state, rect, scale);
            return;
        }
        target_->PushAxisAlignedClip(rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        
        bool privacyActive = state.system.micActive || state.system.cameraActive;
        if (!clockFormat_) return;

        SYSTEMTIME local = {};
        GetLocalTime(&local);
        wchar_t timeBuf[32] = {};
        GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, TIME_NOSECONDS, &local, nullptr, timeBuf, ARRAYSIZE(timeBuf));

        const float width = rect.right - rect.left;
        
        bool hasWeather = state.weather.hasData && (now - state.weather.lastUpdated < 3600.0);
        std::wstring wIcon = L"\xD83C\xDF21\xFE0F"; // Thermometer
        std::wstring wText = L"Loading...";
        if (hasWeather) {
            wText = state.weather.weatherDesc;
            GetWeatherIconAndText(state.weather.weatherCode, wIcon, wText);
        }

        ComPtr<ID2D1SolidColorBrush> whiteTextBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.96f), &whiteTextBrush);

        if (width / scale < 220.0f) {
            // Collapsed Mode
            D2D1_RECT_F timeRect = D2D1::RectF(rect.left + 20.0f * scale, rect.top + 7.0f * scale,
                                               rect.left + 80.0f * scale, rect.bottom - 7.0f * scale);
            target_->DrawTextW(timeBuf, static_cast<UINT32>(wcslen(timeBuf)), smallTextFormat_.Get(),
                               timeRect, whiteTextBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
            
            ComPtr<ID2D1SolidColorBrush> divider;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.12f * settingsOpacity_), &divider);
            target_->FillRoundedRectangle(
                D2D1::RoundedRect(D2D1::RectF(rect.left + 82.0f * scale, rect.top + 10.0f * scale,
                                               rect.left + 83.5f * scale, rect.bottom - 10.0f * scale),
                                  0.5f * scale, 0.5f * scale), divider.Get());

            std::wstring rightLabel;
            bool showWeather = true;
            if (!state.system.foregroundTitle.empty()) {
                showWeather = (static_cast<int>(now) % 10 < 5);
            }

            if (showWeather) {
                wchar_t weatherLabel[32] = {};
                if (hasWeather) swprintf_s(weatherLabel, L"%s %.0f\x00B0", wIcon.c_str(), state.weather.temperature);
                else wcscpy_s(weatherLabel, ARRAYSIZE(weatherLabel), L"\xD83C\xDF21\xFE0F --\x00B0");
                rightLabel = weatherLabel;
            } else {
                rightLabel = L"\xD83D\xDCBB " + state.system.foregroundTitle; // Laptop emoji + window title
            }

            D2D1_RECT_F wRect = D2D1::RectF(rect.left + 94.0f * scale, rect.top + 7.0f * scale,
                                            rect.right - 10.0f * scale, rect.bottom - 7.0f * scale);
            target_->DrawTextW(rightLabel.c_str(), static_cast<UINT32>(rightLabel.length()), smallTextFormat_.Get(),
                               wRect, whiteTextBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT | D2D1_DRAW_TEXT_OPTIONS_CLIP);
            target_->PopAxisAlignedClip();
            return;
        }

        // Expanded Mode
        int tabCount = 4;
        int tab = g_idleTab % tabCount;
        if (tab < 0) tab += tabCount;

        if (tab == 0) DrawCalendarDashboard(state, rect, settings, now, scale, local);
        else if (tab == 1) DrawWeatherDashboard(state, rect, settings, now, scale, hasWeather, wIcon, wText);
        else if (tab == 2) DrawSystemDashboard(state, rect, settings, now, scale);
        else DrawToolsTrayDashboard(state, rect, settings, now, scale);

        // Pagination dots (Vertical on the right edge)
        float shiftX = 0.0f;
        if (state.system.micActive && state.system.cameraActive) {
            shiftX = 30.0f * scale;
        } else if (state.system.micActive || state.system.cameraActive) {
            shiftX = 16.0f * scale;
        }
        const float dotX = rect.right - 10.0f * scale - shiftX;
        const float dotY = (rect.top + rect.bottom) * 0.5f;
        const float spacing = 8.0f * scale;
        const float r = 2.5f * scale;
        
        ComPtr<ID2D1SolidColorBrush> activeDot, inactiveDot;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.85f * settingsOpacity_), &activeDot);
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.25f * settingsOpacity_), &inactiveDot);

        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY - spacing * 1.5f), r, r), tab == 0 ? activeDot.Get() : inactiveDot.Get());
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY - spacing * 0.5f), r, r), tab == 1 ? activeDot.Get() : inactiveDot.Get());
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY + spacing * 0.5f), r, r), tab == 2 ? activeDot.Get() : inactiveDot.Get());
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY + spacing * 1.5f), r, r), tab == 3 ? activeDot.Get() : inactiveDot.Get());

        target_->PopAxisAlignedClip();
    }

    void DrawGameOverlay(const SharedState& state, D2D1_RECT_F rect, float unused_scale) {
        const float scale = 1.0f;
        ComPtr<ID2D1SolidColorBrush> panelBrush;
        D2D1_COLOR_F panelBgCol = lerpedAccent_; panelBgCol.a = 0.060f * settingsOpacity_;
        target_->CreateSolidColorBrush(panelBgCol, &panelBrush);

        D2D1_RECT_F fpsPanel = D2D1::RectF(rect.left + 10.0f * scale, rect.top + 10.0f * scale,
                                          rect.left + 84.0f * scale, rect.bottom - 10.0f * scale);
        target_->FillRoundedRectangle(D2D1::RoundedRect(fpsPanel, 15.0f * scale, 15.0f * scale), panelBrush.Get());

        ComPtr<ID2D1SolidColorBrush> fpsIconBrush;
        target_->CreateSolidColorBrush(lerpedAccent_, &fpsIconBrush);
        D2D1_POINT_2F fpsCenter = D2D1::Point2F(fpsPanel.left + 18.0f * scale, fpsPanel.top + 17.0f * scale);
        DrawGameIcon(fpsCenter, 7.0f * scale, 0, fpsIconBrush.Get(), scale);

        if (g_settings.showMetricText) {
            mutedBrush_->SetOpacity(0.44f);
            target_->DrawTextW(L"FPS", 3, smallTextFormat_.Get(),
                               D2D1::RectF(fpsPanel.left + 31.0f * scale, fpsPanel.top + 6.0f * scale,
                                           fpsPanel.right - 10.0f * scale, fpsPanel.top + 24.0f * scale),
                               mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        }

        wchar_t fpsValue[16] = {};
        swprintf_s(fpsValue, L"%d", state.system.renderFps);
        textBrush_->SetOpacity(0.96f);
        D2D1_RECT_F fpsValueRect = g_settings.showMetricText 
            ? D2D1::RectF(fpsPanel.left + 16.0f * scale, fpsPanel.top + 23.0f * scale, fpsPanel.right - 10.0f * scale, fpsPanel.bottom - 4.0f * scale)
            : D2D1::RectF(fpsPanel.left + 31.0f * scale, fpsPanel.top + 10.0f * scale, fpsPanel.right - 10.0f * scale, fpsPanel.bottom - 4.0f * scale);
        target_->DrawTextW(fpsValue, static_cast<UINT32>(wcslen(fpsValue)), textFormat_.Get(),
                           fpsValueRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        const float cardTop = rect.top + 10.0f * scale;
        const float cardW = 62.0f * scale;
        const float gap = 6.0f * scale;
        const float start = fpsPanel.right + 8.0f * scale;
        DrawGameMetricCard(D2D1::RectF(start, cardTop, start + cardW, rect.bottom - 10.0f * scale),
                           L"CPU", state.system.cpuPercent, 1, scale);
        DrawGameMetricCard(D2D1::RectF(start + cardW + gap, cardTop,
                                       start + cardW * 2.0f + gap, rect.bottom - 10.0f * scale),
                           L"RAM", state.system.memoryPercent, 2, scale);
        DrawGameMetricCard(D2D1::RectF(start + cardW * 2.0f + gap * 2.0f, cardTop,
                                       start + cardW * 3.0f + gap * 2.0f, rect.bottom - 10.0f * scale),
                           L"GPU", state.system.gpuPercent, 3, scale);
        DrawGameMetricCard(D2D1::RectF(start + cardW * 3.0f + gap * 3.0f, cardTop,
                                       start + cardW * 4.0f + gap * 3.0f, rect.bottom - 10.0f * scale),
                           L"DSK", 100 - state.system.diskFreePercent, 4, scale);

        textBrush_->SetOpacity(0.90f);
        mutedBrush_->SetOpacity(0.58f);
    }

    void DrawGameMetricCard(D2D1_RECT_F rect, const wchar_t* label, int percent, int iconKind, float unused_scale) {
        const float scale = 1.0f;
        D2D1_COLOR_F metricColor = lerpedAccent_;

        ComPtr<ID2D1SolidColorBrush> metricBrush;
        target_->CreateSolidColorBrush(metricColor, &metricBrush);

        ComPtr<ID2D1SolidColorBrush> cardBrush;
        ComPtr<ID2D1SolidColorBrush> borderBrush;
        D2D1_COLOR_F bgCol = metricColor; bgCol.a = 0.070f * settingsOpacity_;
        D2D1_COLOR_F borderCol = metricColor; borderCol.a = 0.080f * settingsOpacity_;
        target_->CreateSolidColorBrush(bgCol, &cardBrush);
        target_->CreateSolidColorBrush(borderCol, &borderBrush);
        target_->FillRoundedRectangle(D2D1::RoundedRect(rect, 16.0f * scale, 16.0f * scale), cardBrush.Get());
        target_->DrawRoundedRectangle(D2D1::RoundedRect(rect, 16.0f * scale, 16.0f * scale), borderBrush.Get(), 1.0f * scale);

        metricBrush->SetOpacity(0.24f * settingsOpacity_);
        target_->DrawRoundedRectangle(D2D1::RoundedRect(rect, 16.0f * scale, 16.0f * scale), metricBrush.Get(), 1.2f * scale);
        metricBrush->SetOpacity(1.0f);

        DrawGameIcon(D2D1::Point2F(rect.left + 18.0f * scale, rect.top + 18.0f * scale), 8.0f * scale, iconKind, metricBrush.Get(), scale);

        if (g_settings.showMetricText) {
            mutedBrush_->SetOpacity(0.56f);
            target_->DrawTextW(label, static_cast<UINT32>(wcslen(label)), smallTextFormat_.Get(),
                               D2D1::RectF(rect.left + 31.0f * scale, rect.top + 6.0f * scale,
                                           rect.right - 5.0f * scale, rect.top + 24.0f * scale),
                               mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        }

        wchar_t value[16] = {};
        if (percent < 0) {
            wcscpy_s(value, ARRAYSIZE(value), L"--");
        } else {
            swprintf_s(value, L"%d%%", percent);
        }
        textBrush_->SetOpacity(0.90f);
        D2D1_RECT_F valueRect = g_settings.showMetricText 
            ? D2D1::RectF(rect.left + 10.0f * scale, rect.top + 22.0f * scale, rect.right - 8.0f * scale, rect.bottom - 8.0f * scale)
            : D2D1::RectF(rect.left + 31.0f * scale, rect.top + 10.0f * scale, rect.right - 5.0f * scale, rect.bottom - 8.0f * scale);
        target_->DrawTextW(value, static_cast<UINT32>(wcslen(value)), textFormat_.Get(),
                           valueRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        const float pct = percent < 0 ? 0.0f : Clamp(percent / 100.0f, 0.0f, 1.0f);
        D2D1_RECT_F track = D2D1::RectF(rect.left + 10.0f * scale, rect.bottom - 6.0f * scale,
                                       rect.right - 10.0f * scale, rect.bottom - 3.0f * scale);
        borderBrush->SetOpacity(0.12f * settingsOpacity_);
        target_->FillRoundedRectangle(D2D1::RoundedRect(track, 1.5f * scale, 1.5f * scale), borderBrush.Get());
        D2D1_RECT_F fillRect = D2D1::RectF(track.left, track.top,
                                          track.left + (track.right - track.left) * pct,
                                          track.bottom);
        metricBrush->SetOpacity(0.85f);
        target_->FillRoundedRectangle(D2D1::RoundedRect(fillRect, 1.5f * scale, 1.5f * scale), metricBrush.Get());
        metricBrush->SetOpacity(1.0f);
    }

    void DrawGameIcon(D2D1_POINT_2F center, float radius, int kind, ID2D1SolidColorBrush* customBrush = nullptr, float unused_scale = 1.0f) {
        const float scale = 1.0f;
        ID2D1SolidColorBrush* brush = customBrush ? customBrush : accentBrush_.Get();
        brush->SetOpacity(0.88f);
        switch (kind) {
            case 1: {
                const float size = radius * 0.82f;
                D2D1_RECT_F outerRect = D2D1::RectF(center.x - size, center.y - size, center.x + size, center.y + size);
                target_->DrawRoundedRectangle(D2D1::RoundedRect(outerRect, 2.0f * scale, 2.0f * scale), brush, 1.4f * scale);
                const float dieSize = size * 0.45f;
                D2D1_RECT_F dieRect = D2D1::RectF(center.x - dieSize, center.y - dieSize, center.x + dieSize, center.y + dieSize);
                target_->FillRoundedRectangle(D2D1::RoundedRect(dieRect, 1.0f * scale, 1.0f * scale), brush);
                const float pinLength = 2.0f * scale;
                const float pinSpacing = size * 0.45f;
                for (float offset = -pinSpacing; offset <= pinSpacing + 0.1f * scale; offset += pinSpacing) {
                    target_->DrawLine(D2D1::Point2F(center.x + offset, center.y - size),
                                      D2D1::Point2F(center.x + offset, center.y - size - pinLength),
                                      brush, 1.0f * scale);
                    target_->DrawLine(D2D1::Point2F(center.x + offset, center.y + size),
                                      D2D1::Point2F(center.x + offset, center.y + size + pinLength),
                                      brush, 1.0f * scale);
                    target_->DrawLine(D2D1::Point2F(center.x - size, center.y + offset),
                                      D2D1::Point2F(center.x - size - pinLength, center.y + offset),
                                      brush, 1.0f * scale);
                    target_->DrawLine(D2D1::Point2F(center.x + size, center.y + offset),
                                      D2D1::Point2F(center.x + size + pinLength, center.y + offset),
                                      brush, 1.0f * scale);
                }
                break;
            }
            case 2: {
                const float w = radius * 1.15f;
                const float h = radius * 0.45f;
                D2D1_RECT_F pcb = D2D1::RectF(center.x - w, center.y - h, center.x + w, center.y + h);
                target_->DrawRoundedRectangle(D2D1::RoundedRect(pcb, 1.0f * scale, 1.0f * scale), brush, 1.3f * scale);
                const float chipW = (w * 2.0f - 6.0f * scale) / 3.0f;
                const float chipH = h * 0.65f;
                for (int i = 0; i < 3; ++i) {
                    float cx = center.x - w + 2.0f * scale + i * (chipW + 1.0f * scale);
                    D2D1_RECT_F chip = D2D1::RectF(cx, center.y - chipH, cx + chipW, center.y + chipH);
                    target_->FillRectangle(chip, brush);
                }
                const float pinY = center.y + h;
                for (float px = center.x - w + 2.0f * scale; px <= center.x + w - 2.0f * scale; px += 2.2f * scale) {
                    target_->DrawLine(D2D1::Point2F(px, pinY),
                                      D2D1::Point2F(px, pinY + 1.2f * scale),
                                      brush, 0.9f * scale);
                }
                break;
            }
            case 3: {
                const float w = radius * 1.1f;
                const float h = radius * 0.7f;
                D2D1_RECT_F shroud = D2D1::RectF(center.x - w, center.y - h, center.x + w, center.y + h);
                target_->DrawRoundedRectangle(D2D1::RoundedRect(shroud, 1.5f * scale, 1.5f * scale), brush, 1.3f * scale);
                target_->DrawLine(D2D1::Point2F(center.x - w - 1.5f * scale, center.y - h - 1.0f * scale),
                                  D2D1::Point2F(center.x - w - 1.5f * scale, center.y + h + 1.0f * scale),
                                  brush, 1.4f * scale);
                const float fanR = h * 0.75f;
                target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), fanR, fanR), brush, 1.2f * scale);
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), 1.5f * scale, 1.5f * scale), brush);
                for (int i = 0; i < 4; ++i) {
                    float angle = i * 3.14159f / 4.0f;
                    float dx = std::cos(angle) * fanR;
                    float dy = std::sin(angle) * fanR;
                    target_->DrawLine(D2D1::Point2F(center.x - dx, center.y - dy),
                                      D2D1::Point2F(center.x + dx, center.y + dy),
                                      brush, 0.8f * scale);
                }
                target_->DrawLine(D2D1::Point2F(center.x - w + 3.0f * scale, center.y + h + 1.2f * scale),
                                  D2D1::Point2F(center.x + w - 2.0f * scale, center.y + h + 1.2f * scale),
                                  brush, 1.0f * scale);
                break;
            }
            case 4: {
                const float w = radius * 0.85f;
                const float h = radius * 1.05f;
                D2D1_RECT_F enc = D2D1::RectF(center.x - w, center.y - h, center.x + w, center.y + h);
                target_->DrawRoundedRectangle(D2D1::RoundedRect(enc, 2.0f * scale, 2.0f * scale), brush, 1.3f * scale);
                const float platR = w * 0.82f;
                const float platY = center.y + h - platR - 2.0f * scale;
                target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, platY), platR, platR), brush, 1.2f * scale);
                target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, platY), 2.0f * scale, 2.0f * scale), brush, 1.0f * scale);
                const float pivotX = center.x - w + 3.0f * scale;
                const float pivotY = center.y - h + 3.5f * scale;
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(pivotX, pivotY), 1.5f * scale, 1.5f * scale), brush);
                target_->DrawLine(D2D1::Point2F(pivotX, pivotY),
                                  D2D1::Point2F(center.x + 1.0f * scale, platY - 1.0f * scale),
                                  brush, 1.1f * scale);
                break;
            }
            case 0:
            default: {
                const float dialR = radius * 1.05f;
                target_->DrawEllipse(D2D1::Ellipse(center, dialR, dialR), brush, 1.3f * scale);
                for (int i = 0; i < 5; ++i) {
                    float angle = -3.14159f * 0.8f + i * 3.14159f * 0.4f;
                    float dx1 = std::cos(angle) * dialR;
                    float dy1 = std::sin(angle) * dialR;
                    float dx2 = std::cos(angle) * (dialR - 2.0f * scale);
                    float dy2 = std::sin(angle) * (dialR - 2.0f * scale);
                    target_->DrawLine(D2D1::Point2F(center.x + dx1, center.y + dy1),
                                      D2D1::Point2F(center.x + dx2, center.y + dy2),
                                      brush, 0.9f * scale);
                }
                target_->FillEllipse(D2D1::Ellipse(center, 1.6f * scale, 1.6f * scale), brush);
                const float needleAngle = -3.14159f * 0.25f;
                const float needleLen = dialR * 0.85f;
                target_->DrawLine(center,
                                  D2D1::Point2F(center.x + std::cos(needleAngle) * needleLen,
                                                center.y + std::sin(needleAngle) * needleLen),
                                  brush, 1.4f * scale);
                break;
            }
        }
        brush->SetOpacity(1.0f);
    }

    void DrawPageDots(D2D1_RECT_F rect, int active, int count, float unused_scale) {
        const float scale = 1.0f;
        const float gap = 6.0f * scale;
        const float total = count * 3.0f * scale + (count - 1) * gap;
        const float start = (rect.left + rect.right - total) * 0.5f;
        const float y = rect.bottom - 7.0f * scale;
        for (int i = 0; i < count; ++i) {
            mutedBrush_->SetOpacity(i == active ? 0.45f : 0.16f);
            target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(start + i * (3.0f * scale + gap), y),
                                               (i == active ? 2.0f : 1.5f) * scale,
                                               (i == active ? 2.0f : 1.5f) * scale),
                                 mutedBrush_.Get());
        }
        mutedBrush_->SetOpacity(0.58f);
    }

    void DrawMetricChip(const SharedState& state, D2D1_RECT_F rect, const wchar_t* label, int percent, int iconKind, float scale) {
        UNREFERENCED_PARAMETER(iconKind);
        D2D1_COLOR_F componentColor = lerpedAccent_;
        std::wstring emojiStr = L"âš™ï¸";

        if (wcscmp(label, L"CPU") == 0) {
            componentColor = D2D1::ColorF(1.0f, 0.35f, 0.0f, 1.0f); // CPU: Orange-Red
            emojiStr = L"âš™ï¸";
        } else if (wcscmp(label, L"RAM") == 0) {
            componentColor = D2D1::ColorF(0.0f, 0.48f, 1.0f, 1.0f); // RAM: Royal Blue
            emojiStr = L"ðŸ’¾";
        } else if (wcscmp(label, L"GPU") == 0) {
            componentColor = D2D1::ColorF(0.68f, 0.32f, 0.95f, 1.0f); // GPU: Purple
            emojiStr = L"ðŸŽ®";
        } else if (wcscmp(label, L"BAT") == 0) {
            if (state.battery.low) {
                componentColor = D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f); // Low: Red
            } else if (state.battery.charging) {
                componentColor = D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f); // Charging: Green
            } else {
                componentColor = D2D1::ColorF(0.20f, 0.78f, 0.35f, 1.0f); // Normal: Green
            }
            emojiStr = L"ðŸ”‹";
        } else if (wcscmp(label, L"VOL") == 0) {
            componentColor = D2D1::ColorF(0.0f, 0.75f, 0.85f, 1.0f); // VOL: Cyan/Teal
            emojiStr = L"ðŸ”Š";
        } else if (wcscmp(label, L"DSK") == 0) {
            componentColor = D2D1::ColorF(1.0f, 0.18f, 0.33f, 1.0f); // DSK: Pink
            emojiStr = L"ðŸ’½";
        }

        ComPtr<ID2D1SolidColorBrush> componentBrush;
        target_->CreateSolidColorBrush(componentColor, &componentBrush);

        // Draw Card Background
        ComPtr<ID2D1SolidColorBrush> chipBg;
        ComPtr<ID2D1SolidColorBrush> chipBorder;
        D2D1_COLOR_F bgCol = componentColor; bgCol.a = 0.05f * settingsOpacity_;
        D2D1_COLOR_F borderCol = componentColor; borderCol.a = 0.10f * settingsOpacity_;
        target_->CreateSolidColorBrush(bgCol, &chipBg);
        target_->CreateSolidColorBrush(borderCol, &chipBorder);
        target_->FillRoundedRectangle(D2D1::RoundedRect(rect, 10.0f * scale, 10.0f * scale), chipBg.Get());
        target_->DrawRoundedRectangle(D2D1::RoundedRect(rect, 10.0f * scale, 10.0f * scale), chipBorder.Get(), 1.0f * scale);

        // Draw Circle Background for Emoji on Left
        float iconCX = rect.left + 16.0f * scale;
        float iconCY = rect.top + 16.0f * scale;
        float r = 10.0f * scale;
        ComPtr<ID2D1SolidColorBrush> circleBg;
        D2D1_COLOR_F circCol = componentColor; circCol.a = 0.12f * settingsOpacity_;
        target_->CreateSolidColorBrush(circCol, &circleBg);
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(iconCX, iconCY), r, r), circleBg.Get());

        // Draw Emoji Icon inside the circle
        D2D1_RECT_F emojiRect = D2D1::RectF(iconCX - 8.0f * scale, iconCY - 8.0f * scale, iconCX + 8.0f * scale, iconCY + 8.0f * scale);
        target_->DrawTextW(emojiStr.c_str(), static_cast<UINT32>(emojiStr.length()), smallTextFormat_.Get(), emojiRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

        // Draw Label Text on Right
        D2D1_RECT_F labelRect = D2D1::RectF(rect.left + 32.0f * scale, rect.top + 4.0f * scale, rect.right - 4.0f * scale, rect.top + 18.0f * scale);
        mutedBrush_->SetOpacity(0.55f);
        target_->DrawTextW(label, static_cast<UINT32>(wcslen(label)), smallTextFormat_.Get(), labelRect, mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        mutedBrush_->SetOpacity(0.90f);

        // Draw Percentage Value on Right
        wchar_t value[16] = {};
        if (percent < 0) {
            wcscpy_s(value, ARRAYSIZE(value), L"--");
        } else {
            swprintf_s(value, L"%d%%", percent);
        }
        textBrush_->SetOpacity(0.90f);
        D2D1_RECT_F valueRect = D2D1::RectF(rect.left + 32.0f * scale, rect.top + 17.0f * scale, rect.right - 4.0f * scale, rect.bottom - 8.0f * scale);
        target_->DrawTextW(value, static_cast<UINT32>(wcslen(value)), boldTextFormat_.Get(), valueRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

        // Draw Meter Track / Fill at Bottom
        const float clamped = percent < 0 ? 0.0f : Clamp(percent / 100.0f, 0.0f, 1.0f);
        D2D1_RECT_F meterTrack = D2D1::RectF(rect.left + 8.0f * scale, rect.bottom - 6.0f * scale, rect.right - 8.0f * scale, rect.bottom - 4.0f * scale);
        
        ComPtr<ID2D1SolidColorBrush> trackBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.12f), &trackBg);
        target_->FillRoundedRectangle(D2D1::RoundedRect(meterTrack, 1.0f * scale, 1.0f * scale), trackBg.Get());

        D2D1_RECT_F meterFill = D2D1::RectF(meterTrack.left, meterTrack.top,
                                           meterTrack.left + (meterTrack.right - meterTrack.left) * clamped,
                                           meterTrack.bottom);
        componentBrush->SetOpacity(0.85f);
        target_->FillRoundedRectangle(D2D1::RoundedRect(meterFill, 1.0f * scale, 1.0f * scale), componentBrush.Get());
        
        textBrush_->SetOpacity(0.90f);
        mutedBrush_->SetOpacity(0.58f);
    }

    void DrawMiniUsageBars(D2D1_RECT_F rect, int memory, int cpu) {
        const int bars = 18;
        const float gap = 3.0f;
        const float barWidth = ((rect.right - rect.left) - gap * (bars - 1)) / bars;
        for (int i = 0; i < bars; ++i) {
            const float mix = i / static_cast<float>(bars - 1);
            const float value = (memory * (1.0f - mix) + cpu * mix) / 100.0f;
            const float x = rect.left + i * (barWidth + gap);
            const float h = 2.0f + value * (rect.bottom - rect.top);
            accentBrush_->SetOpacity(0.18f + 0.38f * value);
            target_->FillRoundedRectangle(
                D2D1::RoundedRect(D2D1::RectF(x, rect.bottom - h, x + barWidth, rect.bottom),
                                  barWidth * 0.5f, barWidth * 0.5f),
                accentBrush_.Get());
        }
        accentBrush_->SetOpacity(1.0f);
    }

    void DrawMedia(const SharedState& state, D2D1_RECT_F rect, double now, float scale) {
        const float height = rect.bottom - rect.top;

        float radius = g_settings.w11Style ? 8.0f * g_settings.sizeScale : height * 0.5f;
        if (!g_settings.w11Style) radius = std::min(radius, 44.0f * g_settings.sizeScale);
        ComPtr<ID2D1RoundedRectangleGeometry> mask;
        d2dFactory_->CreateRoundedRectangleGeometry(D2D1::RoundedRect(rect, radius, radius), &mask);
        ComPtr<ID2D1Layer> layer;
        target_->CreateLayer(&layer);

        float expandedAlpha = std::clamp((height - 60.0f) / 60.0f, 0.0f, 1.0f);
        float collapsedAlpha = std::clamp((80.0f - height) / 30.0f, 0.0f, 1.0f);

        // Expanded UI
        if (expandedAlpha > 0.01f && mask && layer) {
            target_->PushLayer(D2D1::LayerParameters(rect, mask.Get(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(), expandedAlpha, nullptr, D2D1_LAYER_OPTIONS_NONE), layer.Get());
            
            int tab = g_idleTab % 5;
            if (tab < 0) tab += 5;

            if (tab == 0) {
                // Expanded media: album art left, title+artist center, waveform right
                const float artSize = 64.0f;
                D2D1_RECT_F artRect = D2D1::RectF(rect.left + 20.0f, rect.top + 20.0f,
                                                  rect.left + 20.0f + artSize, rect.top + 20.0f + artSize);
                DrawAlbumArt(state.media, artRect, now, 10.0f, true);

                // Subtle border around album art
                ComPtr<ID2D1SolidColorBrush> artBorder;
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.10f), &artBorder);
                target_->DrawRoundedRectangle(D2D1::RoundedRect(artRect, 10.0f, 10.0f), artBorder.Get(), 1.0f);

                float shiftX = 0.0f;
                if (state.system.micActive && state.system.cameraActive) shiftX = 30.0f;
                else if (state.system.micActive || state.system.cameraActive) shiftX = 16.0f;

                // Waveform on the right
                const float waveW = 34.0f;
                const float waveH = 24.0f;
                const float waveCY = rect.top + 20.0f + artSize * 0.5f;
                D2D1_RECT_F waveRect = D2D1::RectF(rect.right - 20.0f - shiftX - waveW,
                                                   waveCY - waveH * 0.5f,
                                                   rect.right - 20.0f - shiftX,
                                                   waveCY + waveH * 0.5f);

                const float textLeft = artRect.right + 14.0f;
                const float textRight = waveRect.left - 10.0f;

                // Title â€” bold, large, prominent
                D2D1_RECT_F titleRect = D2D1::RectF(textLeft, rect.top + 24.0f, textRight, rect.top + 46.0f);
                textBrush_->SetOpacity(1.0f);
                DrawMarqueeText(state.media.title.empty() ? L"Unknown" : state.media.title,
                                titleRect, textFormat_.Get(), textBrush_.Get(), now, 42.0f);

                // Artist â€” smaller, muted
                D2D1_RECT_F artistRect = D2D1::RectF(textLeft, rect.top + 48.0f, textRight, rect.top + 66.0f);
                mutedBrush_->SetOpacity(0.55f);
                DrawMarqueeText(state.media.artist.empty() ? L"" : state.media.artist,
                                artistRect, smallTextFormat_.Get(), mutedBrush_.Get(), now, 30.0f);
                mutedBrush_->SetOpacity(0.50f);

                // Waveform bars
                if (state.media.playing) {
                    DrawWaveform(state, waveRect);
                } else {
                    mutedBrush_->SetOpacity(0.4f);
                    for (int i = 0; i < 4; ++i)
                        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(waveRect.left + i * 5.5f + 2.0f, waveCY), 1.3f, 1.3f), mutedBrush_.Get());
                }

                // --- Scrubber (full width, below art block) ---
                const float scrubberY = rect.top + 103.0f;
                double currentPosition = state.media.positionTicks / 10000000.0;
                double duration = state.media.endTicks / 10000000.0;
                if (state.media.playing && state.media.lastUpdatedTicks > 0)
                    currentPosition += (GetTickCount64() - state.media.lastUpdatedTicks) / 1000.0;
                currentPosition = std::max(0.0, std::min(currentPosition, duration));

                auto FormatTime = [](double seconds) -> std::wstring {
                    if (seconds <= 0.0 || _isnan(seconds)) return L"0:00";
                    int m = static_cast<int>(seconds) / 60;
                    int s = static_cast<int>(seconds) % 60;
                    wchar_t buf[16]; swprintf_s(buf, L"%d:%02d", m, s);
                    return buf;
                };

                std::wstring elapsedStr = FormatTime(currentPosition);
                std::wstring remainStr  = L"-" + FormatTime(duration - currentPosition);

                const float scrubLeft  = rect.left + 20.0f;
                const float scrubRight = rect.right - 20.0f;

                // Elapsed / Remaining labels
                mutedBrush_->SetOpacity(0.75f);
                target_->DrawTextW(elapsedStr.c_str(), static_cast<UINT32>(elapsedStr.size()),
                                   smallTextFormat_.Get(),
                                   D2D1::RectF(scrubLeft, scrubberY - 8.0f, scrubLeft + 40.0f, scrubberY + 8.0f),
                                   mutedBrush_.Get());
                target_->DrawTextW(remainStr.c_str(), static_cast<UINT32>(remainStr.size()),
                                   smallTextFormat_.Get(),
                                   D2D1::RectF(scrubRight - 38.0f, scrubberY - 8.0f, scrubRight, scrubberY + 8.0f),
                                   mutedBrush_.Get());

                // Bar track
                const float barLeft  = scrubLeft  + 36.0f;
                const float barRight = scrubRight - 40.0f;
                const float progress = duration > 0.0 ? static_cast<float>(currentPosition / duration) : 0.0f;
                const float scrubW   = (barRight - barLeft) * progress;

                ComPtr<ID2D1SolidColorBrush> scrubBg;
                target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.18f), &scrubBg);
                target_->FillRoundedRectangle(
                    D2D1::RoundedRect(D2D1::RectF(barLeft, scrubberY - 1.8f, barRight, scrubberY + 1.8f), 1.8f, 1.8f),
                    scrubBg.Get());

                // Bar fill (accent color)
                ComPtr<ID2D1SolidColorBrush> scrubFg;
                target_->CreateSolidColorBrush(lerpedAccent_, &scrubFg);
                if (scrubW > 0.0f)
                    target_->FillRoundedRectangle(
                        D2D1::RoundedRect(D2D1::RectF(barLeft, scrubberY - 1.8f, barLeft + scrubW, scrubberY + 1.8f), 1.8f, 1.8f),
                        scrubFg.Get());

                // Scrubber thumb
                ComPtr<ID2D1SolidColorBrush> thumbBrush;
                target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 1), &thumbBrush);
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(barLeft + scrubW, scrubberY), 4.0f, 4.0f), thumbBrush.Get());

                // --- 4 Controls: Back | Play/Pause | Forward | AirPlay ---
                // Evenly distribute: spacing = 56px, centered as a group
                const float ctrlY  = rect.top + 148.0f;
                const float ctrlCX = (rect.left + rect.right) * 0.5f;
                const float sp = 54.0f;
                DrawMediaControls(state.media.playing,
                                  D2D1::Point2F(ctrlCX - sp * 1.5f, ctrlY),   // Back
                                  D2D1::Point2F(ctrlCX - sp * 0.5f, ctrlY),   // Play
                                  D2D1::Point2F(ctrlCX + sp * 0.5f, ctrlY),   // Forward
                                  D2D1::Point2F(ctrlCX + sp * 1.5f, ctrlY));  // AirPlay
            } else if (tab == 1) {
                SYSTEMTIME local = {}; GetLocalTime(&local);
                DrawCalendarDashboard(state, rect, g_settings, now, scale, local);
            } else if (tab == 2) {
                bool hasWeather = state.weather.hasData && (now - state.weather.lastUpdated < 3600.0);
                std::wstring wIcon = L"ðŸŒ¡ï¸"; std::wstring wText = L"Loading...";
                if (hasWeather) {
                    wText = state.weather.weatherDesc;
                    GetWeatherIconAndText(state.weather.weatherCode, wIcon, wText);
                }
                DrawWeatherDashboard(state, rect, g_settings, now, scale, hasWeather, wIcon, wText);
            } else if (tab == 3) {
                DrawSystemDashboard(state, rect, g_settings, now, scale);
            } else {
                DrawToolsTrayDashboard(state, rect, g_settings, now, scale);
            }

            // Pagination dots (Vertical on the right edge)
            float shiftX = 0.0f;
            if (state.system.micActive && state.system.cameraActive) {
                shiftX = 30.0f * scale;
            } else if (state.system.micActive || state.system.cameraActive) {
                shiftX = 16.0f * scale;
            }
            const float dotX = rect.right - 10.0f * scale - shiftX;
            const float dotY = (rect.top + rect.bottom) * 0.5f;
            const float spacing = 8.0f * scale;
            const float r = 2.5f * scale;
            
            ComPtr<ID2D1SolidColorBrush> activeDot, inactiveDot;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.85f * settingsOpacity_), &activeDot);
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.25f * settingsOpacity_), &inactiveDot);

            for (int i = 0; i < 5; ++i) {
                float yOffset = (i - 2) * spacing;
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY + yOffset), r, r), 
                                     tab == i ? activeDot.Get() : inactiveDot.Get());
            }

            target_->PopLayer();
        }

        // Collapsed UI
        if (collapsedAlpha > 0.01f && mask && layer) {
            target_->PushLayer(D2D1::LayerParameters(rect, mask.Get(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(), collapsedAlpha, nullptr, D2D1_LAYER_OPTIONS_NONE), layer.Get());
            
            const float cy = (rect.top + rect.bottom) * 0.5f;
            const float artPadding = 6.0f;
            const float artSize = height - artPadding * 2.0f;
            
            D2D1_RECT_F artRect = D2D1::RectF(rect.left + 8.0f, cy - artSize * 0.5f,
                                              rect.left + 8.0f + artSize, cy + artSize * 0.5f);
            DrawAlbumArt(state.media, artRect, now, artSize * 0.5f, false);

            float shiftX = 0.0f;
            if (state.system.micActive && state.system.cameraActive) {
                shiftX = 30.0f;
            } else if (state.system.micActive || state.system.cameraActive) {
                shiftX = 16.0f;
            }

            D2D1_RECT_F waveRect = D2D1::RectF(rect.right - 42.0f - shiftX, cy - 10.0f,
                                               rect.right - 14.0f - shiftX, cy + 10.0f);
            if (state.media.playing) {
                DrawWaveform(state, waveRect);
            } else {
                mutedBrush_->SetOpacity(0.5f);
                for (int i = 0; i < 4; ++i) {
                    target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(waveRect.left + i * 6.0f + 3.0f, cy), 1.5f, 1.5f), mutedBrush_.Get());
                }
            }

            target_->PopLayer();
        }
    }

    void DrawMediaControls(bool playing, D2D1_POINT_2F prev, D2D1_POINT_2F play, D2D1_POINT_2F next, D2D1_POINT_2F airplay = {-9999.f, -9999.f}) {
        DrawMediaButton(prev,    18.0f, 0, false);
        DrawMediaButton(play,    24.0f, playing ? 1 : 2, true);
        DrawMediaButton(next,    18.0f, 3, false);
        if (airplay.x > -9000.f)
            DrawMediaButton(airplay, 18.0f, 4, false);
    }

    void DrawMediaButton(D2D1_POINT_2F center, float radius, int kind, bool primary) {
        int buttonCmd = (kind == 0) ? 0 : ((kind == 1 || kind == 2) ? 1 : 2);
        bool isPressed = (g_pressedMediaButton.load() == buttonCmd);

        if (isPressed) radius *= 0.88f;

        // Dark circle background â€” matches reference image style
        ComPtr<ID2D1SolidColorBrush> bg;
        const float bgAlpha = primary ? (isPressed ? 0.30f : 0.18f) : (isPressed ? 0.18f : 0.10f);
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, bgAlpha), &bg);
        target_->FillEllipse(D2D1::Ellipse(center, radius, radius), bg.Get());

        // All icons are WHITE (textBrush_) â€” accent color only for scrubber/waveform
        const float iconAlpha = primary ? (isPressed ? 1.0f : 0.95f) : (isPressed ? 0.90f : 0.80f);
        textBrush_->SetOpacity(iconAlpha);

        if (kind == 1) {  // pause â€” two white bars
            const float h = radius * 0.72f;
            target_->FillRoundedRectangle(D2D1::RoundedRect(
                D2D1::RectF(center.x - 4.0f, center.y - h * 0.5f, center.x - 1.5f, center.y + h * 0.5f),
                1.0f, 1.0f), textBrush_.Get());
            target_->FillRoundedRectangle(D2D1::RoundedRect(
                D2D1::RectF(center.x + 1.5f, center.y - h * 0.5f, center.x + 4.0f, center.y + h * 0.5f),
                1.0f, 1.0f), textBrush_.Get());
        } else if (kind == 4) {
            DrawAirPlayIcon(center, radius);
        } else {
            const float dir = kind == 0 ? -1.0f : 1.0f;
            const float tri = radius * (primary ? 0.70f : 0.62f);
            const float cx = (kind == 0 || kind == 3) ? center.x - dir * radius * 0.16f : center.x;
            D2D1_POINT_2F p1 = D2D1::Point2F(cx - dir * tri * 0.35f, center.y - tri * 0.58f);
            D2D1_POINT_2F p2 = D2D1::Point2F(cx - dir * tri * 0.35f, center.y + tri * 0.58f);
            D2D1_POINT_2F p3 = D2D1::Point2F(cx + dir * tri * 0.55f, center.y);
            ComPtr<ID2D1PathGeometry> geom; d2dFactory_->CreatePathGeometry(&geom);
            ComPtr<ID2D1GeometrySink> sink; geom->Open(&sink);
            sink->BeginFigure(p1, D2D1_FIGURE_BEGIN_FILLED);
            sink->AddLine(p2); sink->AddLine(p3);
            sink->EndFigure(D2D1_FIGURE_END_CLOSED); sink->Close();
            target_->FillGeometry(geom.Get(), textBrush_.Get());
            if (kind == 0 || kind == 3) {
                const float x = cx + dir * radius * 0.55f;
                target_->DrawLine(D2D1::Point2F(x, center.y - radius * 0.45f),
                                  D2D1::Point2F(x, center.y + radius * 0.45f),
                                  textBrush_.Get(), 1.8f);
            }
        }
        textBrush_->SetOpacity(1.0f);
    }

    void DrawAirPlayIcon(D2D1_POINT_2F center, float radius) {
        // All drawn in white (textBrush_) â€” already set by caller DrawMediaButton
        // Upward-pointing triangle (antenna base)
        const float triH = radius * 0.46f;
        const float triW = radius * 0.44f;
        const float triTop = center.y - triH * 0.60f;
        const float triBot = center.y + triH * 0.40f;
        ComPtr<ID2D1PathGeometry> triGeom; d2dFactory_->CreatePathGeometry(&triGeom);
        ComPtr<ID2D1GeometrySink> triSink; triGeom->Open(&triSink);
        triSink->BeginFigure(D2D1::Point2F(center.x, triTop), D2D1_FIGURE_BEGIN_FILLED);
        triSink->AddLine(D2D1::Point2F(center.x - triW, triBot));
        triSink->AddLine(D2D1::Point2F(center.x + triW, triBot));
        triSink->EndFigure(D2D1_FIGURE_END_CLOSED); triSink->Close();
        target_->FillGeometry(triGeom.Get(), textBrush_.Get());

        // Inner arc (wireless signal)
        const float arcY = triTop;
        const float r1 = radius * 0.32f;
        ComPtr<ID2D1PathGeometry> arc1; d2dFactory_->CreatePathGeometry(&arc1);
        ComPtr<ID2D1GeometrySink> s1; arc1->Open(&s1);
        s1->BeginFigure(D2D1::Point2F(center.x - r1, arcY), D2D1_FIGURE_BEGIN_HOLLOW);
        s1->AddArc(D2D1::ArcSegment(D2D1::Point2F(center.x + r1, arcY),
            D2D1::SizeF(r1, r1), 0.f, D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
        s1->EndFigure(D2D1_FIGURE_END_OPEN); s1->Close();
        target_->DrawGeometry(arc1.Get(), textBrush_.Get(), 1.3f);

        // Outer arc (wireless signal)
        const float r2 = radius * 0.56f;
        ComPtr<ID2D1PathGeometry> arc2; d2dFactory_->CreatePathGeometry(&arc2);
        ComPtr<ID2D1GeometrySink> s2; arc2->Open(&s2);
        s2->BeginFigure(D2D1::Point2F(center.x - r2 * 0.87f, arcY - r2 * 0.50f), D2D1_FIGURE_BEGIN_HOLLOW);
        s2->AddArc(D2D1::ArcSegment(D2D1::Point2F(center.x + r2 * 0.87f, arcY - r2 * 0.50f),
            D2D1::SizeF(r2, r2), 0.f, D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
        s2->EndFigure(D2D1_FIGURE_END_OPEN); s2->Close();
        target_->DrawGeometry(arc2.Get(), textBrush_.Get(), 1.3f);
    }

    void DrawAlbumArt(const MediaSnapshot& media, D2D1_RECT_F rect, double now, float radius = 9.0f, bool drawBadge = true) {
        ComPtr<ID2D1RoundedRectangleGeometry> mask;
        HRESULT hrMask = d2dFactory_->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(rect, radius, radius), &mask);
        ComPtr<ID2D1Layer> layer;
        HRESULT hrLayer = target_->CreateLayer(nullptr, &layer);
        const bool roundedClip = SUCCEEDED(hrMask) && SUCCEEDED(hrLayer) && mask && layer;
        if (roundedClip) {
            target_->PushLayer(D2D1::LayerParameters(rect, mask.Get()), layer.Get());
        } else {
            target_->PushAxisAlignedClip(rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        }

        if (!media.art.bgra.empty()) {
            if (artGeneration_ != media.art.generation || !artBitmap_) {
                D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
                    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
                target_->CreateBitmap(D2D1::SizeU(media.art.width, media.art.height),
                                      media.art.bgra.data(), media.art.width * 4,
                                      &props, &artBitmap_);
                artGeneration_ = media.art.generation;
            }

            D2D1_RECT_F dst = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);
            target_->DrawBitmap(artBitmap_.Get(), dst, 1.0f,
                                D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
        } else {
            accentBrush_->SetOpacity(0.24f);
            target_->FillRoundedRectangle(D2D1::RoundedRect(rect, radius, radius), accentBrush_.Get());
            accentBrush_->SetOpacity(1.0f);
            if (!media.sourceIcon.bgra.empty()) {
                D2D1_RECT_F iconRect = D2D1::RectF(rect.left + 11, rect.top + 11,
                                                  rect.right - 11, rect.bottom - 11);
                DrawBitmapPixels(media.sourceIcon, iconRect, mediaSourceIconBitmap_,
                                 mediaSourceIconGeneration_, 0.95f);
            } else {
                target_->DrawTextW(media.sourceBadge.empty() ? L"\u25b6" : media.sourceBadge.c_str(),
                                   static_cast<UINT32>(media.sourceBadge.empty() ? 1 : media.sourceBadge.size()),
                                   textFormat_.Get(), rect, textBrush_.Get());
            }
        }

        if (drawBadge && !media.sourceIcon.bgra.empty()) {
            D2D1_RECT_F badge = D2D1::RectF(rect.right - 24, rect.bottom - 22,
                                           rect.right - 3, rect.bottom - 3);
            DrawCircularBitmapPixels(media.sourceIcon,
                                     D2D1::Point2F((badge.left + badge.right) * 0.5f,
                                                   (badge.top + badge.bottom) * 0.5f),
                                     9.5f, mediaSourceIconBitmap_,
                                     mediaSourceIconGeneration_, 0.98f);
        }

        if (roundedClip) {
            target_->PopLayer();
        } else {
            target_->PopAxisAlignedClip();
        }
    }

    void DrawBitmapPixels(const BitmapPixels& pixels, D2D1_RECT_F rect,
                          ComPtr<ID2D1Bitmap>& cache, uint64_t& cachedGeneration,
                          float opacity = 1.0f) {
        if (pixels.bgra.empty()) {
            return;
        }

        if (cachedGeneration != pixels.generation || !cache) {
            D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
            target_->CreateBitmap(D2D1::SizeU(pixels.width, pixels.height),
                                  pixels.bgra.data(), pixels.width * 4,
                                  &props, &cache);
            cachedGeneration = pixels.generation;
        }

        if (cache) {
            target_->DrawBitmap(cache.Get(), rect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
        }
    }

    // Draws a bitmap filling a rounded-rect badge with a small inset for polish.
    void DrawRoundedBitmapPixels(const BitmapPixels& pixels, D2D1_RECT_F badge,
                                 float cornerRadius,
                                 ComPtr<ID2D1Bitmap>& cache, uint64_t& cachedGeneration,
                                 float opacity = 1.0f) {
        if (pixels.bgra.empty()) return;

        // 2px inset so the icon has clean edges inside the badge.
        const float pad = 2.0f;
        D2D1_RECT_F iconRect = D2D1::RectF(badge.left + pad, badge.top + pad,
                                           badge.right - pad, badge.bottom - pad);
        const float innerR = std::max(0.0f, cornerRadius - pad);

        ComPtr<ID2D1RoundedRectangleGeometry> mask;
        d2dFactory_->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(iconRect, innerR, innerR), &mask);
        ComPtr<ID2D1Layer> layer;
        target_->CreateLayer(nullptr, &layer);

        if (mask && layer) {
            target_->PushLayer(
                D2D1::LayerParameters(iconRect, mask.Get(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
                layer.Get());
            DrawBitmapPixels(pixels, iconRect, cache, cachedGeneration, opacity);
            target_->PopLayer();
        } else {
            DrawBitmapPixels(pixels, iconRect, cache, cachedGeneration, opacity);
        }
    }

    void DrawCircularBitmapPixels(const BitmapPixels& pixels, D2D1_POINT_2F center, float radius,
                                  ComPtr<ID2D1Bitmap>& cache, uint64_t& cachedGeneration,
                                  float opacity = 1.0f) {
        if (pixels.bgra.empty()) {
            return;
        }

        D2D1_RECT_F rect = D2D1::RectF(center.x - radius, center.y - radius,
                                      center.x + radius, center.y + radius);
        ComPtr<ID2D1EllipseGeometry> ellipse;
        d2dFactory_->CreateEllipseGeometry(D2D1::Ellipse(center, radius, radius), &ellipse);
        ComPtr<ID2D1Layer> layer;
        target_->CreateLayer(nullptr, &layer);

        if (ellipse && layer) {
            target_->PushLayer(D2D1::LayerParameters(
                                  rect, ellipse.Get(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
                              layer.Get());
            DrawBitmapPixels(pixels, rect, cache, cachedGeneration, opacity);
            target_->PopLayer();
        } else {
            DrawBitmapPixels(pixels, rect, cache, cachedGeneration, opacity);
        }

        ComPtr<ID2D1SolidColorBrush> border;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.12f), &border);
        if (border) {
            target_->DrawEllipse(D2D1::Ellipse(center, radius, radius), border.Get(), 1.0f);
        }
    }

    void DrawMarqueeText(const std::wstring& text, D2D1_RECT_F rect, IDWriteTextFormat* format,
                         ID2D1Brush* brush, double now, float speed) {
        if (!format || !brush || text.empty()) {
            return;
        }

        ComPtr<IDWriteTextLayout> layout;
        dwriteFactory_->CreateTextLayout(text.c_str(), static_cast<UINT32>(text.size()),
                                         format, 2000.0f, rect.bottom - rect.top, &layout);
        DWRITE_TEXT_METRICS metrics = {};
        if (layout) {
            layout->GetMetrics(&metrics);
        }

        const float available = rect.right - rect.left;
        
        // We only want to clip horizontally so we don't bleed out of the pill.
        // We expand the vertical bounds so descenders/ascenders aren't cut off.
        D2D1_RECT_F clipRect = rect;
        clipRect.top -= 10.0f;
        clipRect.bottom += 10.0f;
        target_->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

        if (metrics.widthIncludingTrailingWhitespace <= available) {
            target_->DrawTextLayout(D2D1::Point2F(rect.left, rect.top), layout.Get(), brush,
                                    D2D1_DRAW_TEXT_OPTIONS_NONE);
        } else {
            const float cycle = metrics.widthIncludingTrailingWhitespace + 38.0f;
            const float pauseDuration = 1.5f;
            const float totalDuration = (cycle / speed) + pauseDuration;
            const float timeInCycle = std::fmod(static_cast<float>(now), totalDuration);
            const float offset = (timeInCycle <= pauseDuration) ? 0.0f : (timeInCycle - pauseDuration) * speed;

            target_->DrawTextLayout(D2D1::Point2F(rect.left - offset, rect.top), layout.Get(),
                                    brush, D2D1_DRAW_TEXT_OPTIONS_NONE);
            target_->DrawTextLayout(D2D1::Point2F(rect.left - offset + cycle, rect.top),
                                    layout.Get(), brush, D2D1_DRAW_TEXT_OPTIONS_NONE);
        }
        target_->PopAxisAlignedClip();
    }

    void DrawWaveform(const SharedState& state, D2D1_RECT_F rect) {
        const float gap = 2.5f;
        const float minBarWidth = 2.0f;
        const float availableW = rect.right - rect.left;
        size_t count = std::max<size_t>(1, static_cast<size_t>((availableW + gap) / (minBarWidth + gap)));
        count = std::min<size_t>(count, 32);

        const float barWidth = (availableW - gap * (count - 1)) / count;
        const float centerY = (rect.top + rect.bottom) * 0.5f;
        const float maxH = (rect.bottom - rect.top) * 0.86f;

        // Use a step size of 4 samples (approx 40ms) so bars aren't identical
        const size_t step = 4;

        // Create linear gradient brush for the premium vertical gradient effect
        ComPtr<ID2D1LinearGradientBrush> gradBrush;
        ComPtr<ID2D1GradientStopCollection> stops;
        D2D1_GRADIENT_STOP stopData[2] = {};
        stopData[0].position = 0.0f;
        stopData[0].color = lerpedAccent_;
        stopData[1].position = 1.0f;
        stopData[1].color = lerpedAccent_;
        // Bottom is faded slightly for depth (Liquid Glass look)
        stopData[1].color.a = 0.35f;

        target_->CreateGradientStopCollection(stopData, 2, &stops);
        if (stops) {
            target_->CreateLinearGradientBrush(
                D2D1::LinearGradientBrushProperties(
                    D2D1::Point2F((rect.left + rect.right) * 0.5f, rect.top),
                    D2D1::Point2F((rect.left + rect.right) * 0.5f, rect.bottom)),
                stops.Get(), &gradBrush);
        }

        for (size_t i = 0; i < count; ++i) {
            const size_t offset = (count - i) * step;
            const size_t source = (state.waveformWrite + state.waveform.size() - offset) %
                                  state.waveform.size();
            const float amp = Clamp(state.waveform[source], 0.03f, 1.0f);
            const float h = std::max(3.0f, amp * maxH);
            const float x = rect.left + i * (barWidth + gap);
            D2D1_RECT_F bar = D2D1::RectF(x, centerY - h * 0.5f, x + barWidth, centerY + h * 0.5f);
            
            if (gradBrush) {
                gradBrush->SetOpacity(0.45f + 0.5f * amp);
                target_->FillRoundedRectangle(D2D1::RoundedRect(bar, barWidth * 0.5f, barWidth * 0.5f),
                                             gradBrush.Get());
            } else {
                accentBrush_->SetOpacity(0.45f + 0.5f * amp);
                target_->FillRoundedRectangle(D2D1::RoundedRect(bar, barWidth * 0.5f, barWidth * 0.5f),
                                             accentBrush_.Get());
            }
        }
        accentBrush_->SetOpacity(1.0f);
    }

    void DrawClipboard(const SharedState& state, D2D1_RECT_F rect) {
        const float width = rect.right - rect.left;
        if (width < 20.0f) return;
        const double now = NowSeconds();
        const float cy = (rect.top + rect.bottom) * 0.5f;

        ComPtr<ID2D1SolidColorBrush> whiteBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &whiteBrush);

        // ── 1. Split Circle Mode (36px x 36px) ──
        if (width < 45.0f) {
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r = 12.0f;
            D2D1_RECT_F badge = D2D1::RectF(cx - r, cy - r, cx + r, cy + r);
            if (!state.clipboard.appIcon.bgra.empty()) {
                DrawCircularBitmapPixels(state.clipboard.appIcon, D2D1::Point2F(cx, cy), r, clipboardIconBitmap_, clipboardIconGeneration_, 1.0f);
            } else {
                const wchar_t* glyph = state.clipboard.image ? L"\uE114" : L"\uE8C8";
                if (iconFormat_) {
                    iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                    iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                    target_->DrawTextW(glyph, 1, iconFormat_.Get(), badge, whiteBrush.Get());
                    iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                    iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
                }
            }
            return;
        }

        // ── 2. Split Pill Mode (62px Width) ──
        if (width < 100.0f) {
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 6.0f, cy - badgeSz * 0.5f, rect.left + 6.0f + badgeSz, cy + badgeSz * 0.5f);
            const float br = badgeSz * 0.32f;
            if (!state.clipboard.appIcon.bgra.empty()) {
                DrawRoundedBitmapPixels(state.clipboard.appIcon, badge, br, clipboardIconBitmap_, clipboardIconGeneration_, 1.0f);
            } else {
                const wchar_t* glyph = state.clipboard.image ? L"\uE114" : L"\uE8C8";
                if (iconFormat_) {
                    iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                    iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                    target_->DrawTextW(glyph, 1, iconFormat_.Get(), badge, whiteBrush.Get());
                    iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                    iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
                }
            }
            const wchar_t* glyphRight = L"\uE8A6";
            D2D1_RECT_F rightRect = D2D1::RectF(rect.right - 28.0f, cy - 10.0f, rect.right - 6.0f, cy + 10.0f);
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyphRight, 1, iconFormat_.Get(), rightRect, whiteBrush.Get());
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        if (rect.bottom - rect.top < 40.0f) {
            // Compact Clipboard View (height 36.0f)
            const float cy = (rect.top + rect.bottom) * 0.5f;
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 8.0f, cy - badgeSz * 0.5f,
                                            rect.left + 8.0f + badgeSz, cy + badgeSz * 0.5f);
            
            // Draw small badge background
            ComPtr<ID2D1SolidColorBrush> badgeBg;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.10f), &badgeBg);
            target_->FillRoundedRectangle(D2D1::RoundedRect(badge, 6.0f, 6.0f), badgeBg.Get());

            // Icon
            if (!state.clipboard.appIcon.bgra.empty()) {
                DrawRoundedBitmapPixels(state.clipboard.appIcon, badge, 5.0f,
                                        clipboardIconBitmap_, clipboardIconGeneration_, 1.0f);
            } else {
                const wchar_t* glyph = state.clipboard.image ? L"\uE114" : L"\uE8C8";
                textBrush_->SetOpacity(0.90f);
                if (smallTextFormat_ && iconFormat_) {
                    iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                    iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                    target_->DrawTextW(glyph, static_cast<UINT32>(wcslen(glyph)), iconFormat_.Get(), badge,
                                       textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                    iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                    iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
                }
            }

            // Text label
            D2D1_RECT_F textRect = D2D1::RectF(badge.right + 8.0f, rect.top + 6.0f, rect.right - 10.0f, rect.bottom - 6.0f);
            textBrush_->SetOpacity(0.92f);
            const std::wstring dispText = state.clipboard.text.empty() ? (state.clipboard.image ? L"Image copied" : L"Copied") : state.clipboard.text;
            target_->DrawTextW(dispText.c_str(), static_cast<UINT32>(dispText.size()), smallTextFormat_.Get(),
                               textRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
            textBrush_->SetOpacity(1.0f);
            return;
        }

        if (rect.bottom - rect.top < 40.0f) return; // Keep compiler analysis happy

        const float ttl = 4.0f;
        const float remaining = Clamp(static_cast<float>(state.clipboard.expiresAt - now), 0.0f, ttl);
        const float progress = remaining / ttl;

        D2D1_RECT_F badge = D2D1::RectF(rect.left + 12, rect.top + 10,
                                       rect.left + 50, rect.bottom - 10);
        ComPtr<ID2D1SolidColorBrush> badgeBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.070f), &badgeBg);
        target_->FillRoundedRectangle(D2D1::RoundedRect(badge, 13, 13), badgeBg.Get());
        target_->DrawRoundedRectangle(D2D1::RoundedRect(badge, 13, 13), accentBrush_.Get(), 1.0f);

        if (!state.clipboard.appIcon.bgra.empty()) {
            DrawRoundedBitmapPixels(state.clipboard.appIcon,
                                    badge, 10.0f,
                                    clipboardIconBitmap_,
                                    clipboardIconGeneration_, 0.96f);
        } else {
            // Fallback icon when no app icon is available.
            // Use Segoe Fluent Icons for a native Windows 11 look.
            const wchar_t* glyph = state.clipboard.image ? L"\uE114" : L"\uE8C8";
            // For image: show a bright gradient-style photo icon placeholder.
            ComPtr<ID2D1SolidColorBrush> iconBg;
            const D2D1_COLOR_F iconColor = state.clipboard.image
                ? D2D1::ColorF(0.3f, 0.3f, 0.35f, 0.90f)
                : D2D1::ColorF(1, 1, 1, 0.055f * settingsOpacity_);
            target_->CreateSolidColorBrush(iconColor, &iconBg);
            target_->FillRoundedRectangle(D2D1::RoundedRect(badge, 13, 13), iconBg.Get());
            textBrush_->SetOpacity(0.95f);
            
            // Draw glyph perfectly centered in the badge rectangle.
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph,
                                   static_cast<UINT32>(wcslen(glyph)), iconFormat_.Get(), badge,
                                   textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            
            textBrush_->SetOpacity(0.90f);
        }

        D2D1_RECT_F titleRect = D2D1::RectF(badge.right + 11, rect.top + 9,
                                           rect.right - 18, rect.top + 25);
        mutedBrush_->SetOpacity(0.48f);
        const std::wstring clipTitle =
            state.clipboard.appName.empty()
                ? (state.clipboard.image ? std::wstring(L"Image copied") : std::wstring(L"Clipboard"))
                : state.clipboard.appName + L"  \u00b7  Clipboard";
        target_->DrawTextW(clipTitle.c_str(), static_cast<UINT32>(clipTitle.size()),
                           smallTextFormat_.Get(), titleRect, mutedBrush_.Get(),
                           D2D1_DRAW_TEXT_OPTIONS_CLIP);

        D2D1_RECT_F textRect = D2D1::RectF(badge.right + 11, rect.top + 25,
                                           rect.right - 18, rect.bottom - 12);
        DrawMarqueeText(state.clipboard.text.empty() ? L"Copied" : state.clipboard.text,
                        textRect, textFormat_.Get(), textBrush_.Get(), now, 34.0f);

        D2D1_RECT_F track = D2D1::RectF(badge.right + 11, rect.bottom - 8,
                                       rect.right - 20, rect.bottom - 6);
        ComPtr<ID2D1SolidColorBrush> trackBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.08f), &trackBrush);
        target_->FillRoundedRectangle(D2D1::RoundedRect(track, 1, 1), trackBrush.Get());
        D2D1_RECT_F fill = D2D1::RectF(track.left, track.top,
                                      track.left + (track.right - track.left) * progress,
                                      track.bottom);
        accentBrush_->SetOpacity(0.55f);
        target_->FillRoundedRectangle(D2D1::RoundedRect(fill, 1, 1), accentBrush_.Get());
        accentBrush_->SetOpacity(1.0f);
        mutedBrush_->SetOpacity(0.58f);
    }

    void DrawNotification(const SharedState& state, D2D1_RECT_F rect) {
        const float width = rect.right - rect.left;
        if (width < 20.0f) return;
        const double now = NowSeconds();
        const float cy = (rect.top + rect.bottom) * 0.5f;

        ComPtr<ID2D1SolidColorBrush> whiteBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &whiteBrush);

        // ── 1. Split Circle Mode (36px x 36px) ──
        if (width < 45.0f) {
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r = 12.0f;
            if (!state.notification.icon.bgra.empty()) {
                DrawCircularBitmapPixels(state.notification.icon, D2D1::Point2F(cx, cy), r, notificationIconBitmap_, notificationIconGeneration_, 1.0f);
            } else {
                DrawNotificationFallbackIcon(D2D1::Point2F(cx, cy), r * 0.8f);
            }
            return;
        }

        // ── 2. Split Pill Mode (62px Width) ──
        if (width < 100.0f) {
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 6.0f, cy - badgeSz * 0.5f, rect.left + 6.0f + badgeSz, cy + badgeSz * 0.5f);
            const float br = badgeSz * 0.32f;
            if (!state.notification.icon.bgra.empty()) {
                DrawRoundedBitmapPixels(state.notification.icon, badge, br, notificationIconBitmap_, notificationIconGeneration_, 1.0f);
            } else {
                DrawNotificationFallbackIcon(D2D1::Point2F((badge.left + badge.right) * 0.5f, cy), badgeSz * 0.38f);
            }
            const wchar_t* glyph = L"\uE7E7";
            D2D1_RECT_F bellRect = D2D1::RectF(rect.right - 28.0f, cy - 10.0f, rect.right - 6.0f, cy + 10.0f);
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, 1, iconFormat_.Get(), bellRect, whiteBrush.Get());
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        if (rect.bottom - rect.top < 48.0f) {
            // Compact Notification View (height 36.0f)
            const float cy = (rect.top + rect.bottom) * 0.5f;
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 8.0f, cy - badgeSz * 0.5f,
                                            rect.left + 8.0f + badgeSz, cy + badgeSz * 0.5f);
            const float br = badgeSz * 0.32f;

            ComPtr<ID2D1SolidColorBrush> plateBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.10f), &plateBrush);
            target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), plateBrush.Get());

            if (!state.notification.icon.bgra.empty()) {
                DrawRoundedBitmapPixels(state.notification.icon, badge, br,
                                        notificationIconBitmap_, notificationIconGeneration_, 1.0f);
            } else {
                DrawNotificationFallbackIcon(D2D1::Point2F((badge.left + badge.right) * 0.5f, cy), badgeSz * 0.38f);
            }

            D2D1_RECT_F textRect = D2D1::RectF(badge.right + 8.0f, rect.top + 6.0f, rect.right - 10.0f, rect.bottom - 6.0f);
            textBrush_->SetOpacity(0.92f);
            std::wstring displayMsg = state.notification.title;
            if (displayMsg.empty()) displayMsg = state.notification.app.empty() ? L"Notification" : state.notification.app;
            target_->DrawTextW(displayMsg.c_str(), static_cast<UINT32>(displayMsg.size()), smallTextFormat_.Get(),
                               textRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
            textBrush_->SetOpacity(1.0f);
            return;
        }

        if (rect.bottom - rect.top < 48.0f) return; // Keep compiler analysis happy

        const float ttl = 4.0f;
        const float remaining = Clamp(static_cast<float>(state.notification.expiresAt - now), 0.0f, ttl);
        const float progress = remaining / ttl;

        const float cy2 = (rect.top + rect.bottom) * 0.5f;
        // iOS squircle app icon container
        const float iconSz = 38.0f;
        D2D1_RECT_F badge = D2D1::RectF(rect.left + 14.0f, cy2 - iconSz * 0.5f,
                                        rect.left + 14.0f + iconSz, cy2 + iconSz * 0.5f);
        const float br = iconSz * 0.32f;

        // Background plate for icon
        ComPtr<ID2D1SolidColorBrush> plateBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.10f), &plateBrush);
        target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), plateBrush.Get());

        if (!state.notification.icon.bgra.empty()) {
            DrawRoundedBitmapPixels(state.notification.icon, badge, br,
                                    notificationIconBitmap_, notificationIconGeneration_, 1.0f);

            // Red dot notification badge (top-right of app icon)
            ComPtr<ID2D1SolidColorBrush> badgeColor;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &badgeColor);
            
            const float dotR = iconSz * 0.14f;
            const float dotX = badge.right - dotR * 0.4f;
            const float dotY = badge.top + dotR * 0.4f;
            
            target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY), dotR, dotR), badgeColor.Get());

            ComPtr<ID2D1SolidColorBrush> badgeBorder;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &badgeBorder);
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(dotX, dotY), dotR, dotR), badgeBorder.Get(), 0.9f);
        } else {
            DrawNotificationFallbackIcon(
                D2D1::Point2F((badge.left + badge.right) * 0.5f, cy2), iconSz * 0.38f);
        }

        // Smart-extract Title and Body from concatenated notification string
        std::wstring rawTitle = state.notification.title;
        std::wstring titlePart = rawTitle;
        std::wstring bodyPart = state.notification.body;

        if (bodyPart.empty()) {
            size_t pos = rawTitle.find(L"  -  ");
            if (pos != std::wstring::npos) {
                titlePart = rawTitle.substr(0, pos);
                bodyPart = rawTitle.substr(pos + 5);
            } else {
                pos = rawTitle.find(L" - ");
                if (pos != std::wstring::npos) {
                    titlePart = rawTitle.substr(0, pos);
                    bodyPart = rawTitle.substr(pos + 3);
                }
            }
        }

        if (titlePart.empty()) {
            titlePart = L"Notification";
        }

        const float tx = badge.right + 12.0f;
        const float rx = rect.right - 14.0f;

        // 1. App name & timestamp "now" on the top line
        std::wstring appName = state.notification.app.empty() ? L"Notification" : state.notification.app;
        std::wstring topLabel = appName + L"  \u2022  now";
        D2D1_RECT_F topRect = D2D1::RectF(tx, rect.top + 8.0f, rx, rect.top + 22.0f);
        mutedBrush_->SetOpacity(0.52f);
        target_->DrawTextW(topLabel.c_str(), static_cast<UINT32>(topLabel.size()),
                           smallTextFormat_.Get(), topRect, mutedBrush_.Get(),
                           D2D1_DRAW_TEXT_OPTIONS_CLIP);

        // 2. Bold title (sender) in the middle line
        D2D1_RECT_F midRect = D2D1::RectF(tx, rect.top + 22.0f, rx, rect.top + 40.0f);
        textBrush_->SetOpacity(0.96f);
        DrawMarqueeText(titlePart, midRect, textFormat_.Get(), textBrush_.Get(), now, 32.0f);

        // 3. Regular body content in the bottom line
        if (!bodyPart.empty()) {
            D2D1_RECT_F botRect = D2D1::RectF(tx, rect.top + 40.0f, rx, rect.top + 56.0f);
            mutedBrush_->SetOpacity(0.72f);
            DrawMarqueeText(bodyPart, botRect, smallTextFormat_.Get(), mutedBrush_.Get(), now, 24.0f);
        }

        // 4. Clean bottom progress bar
        D2D1_RECT_F track = D2D1::RectF(tx, rect.bottom - 7.0f, rx, rect.bottom - 5.0f);
        ComPtr<ID2D1SolidColorBrush> trackBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.08f), &trackBrush);
        target_->FillRoundedRectangle(D2D1::RoundedRect(track, 1.0f, 1.0f), trackBrush.Get());

        D2D1_RECT_F fill = D2D1::RectF(track.left, track.top,
                                       track.left + (track.right - track.left) * progress, track.bottom);
        accentBrush_->SetOpacity(0.75f);
        target_->FillRoundedRectangle(D2D1::RoundedRect(fill, 1.0f, 1.0f), accentBrush_.Get());
        
        // Restore brushes opacity
        accentBrush_->SetOpacity(1.0f);
        mutedBrush_->SetOpacity(0.58f);
        textBrush_->SetOpacity(1.0f);
    }

    void DrawVolume(const SharedState& state, D2D1_RECT_F rect) {
        const float width = rect.right - rect.left;
        if (width < 20.0f) return;
        const bool muted = state.volume.muted || state.volume.percent == 0;
        const float pct   = Clamp(state.volume.percent / 100.0f, 0.0f, 1.0f);
        const float cy = (rect.top + rect.bottom) * 0.5f;

        ComPtr<ID2D1SolidColorBrush> whiteBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &whiteBrush);

        ComPtr<ID2D1SolidColorBrush> accentColorBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.48f, 1.0f, 1.0f), &accentColorBrush);

        // ── 1. Split Circle Mode (36px x 36px) ──
        if (width < 45.0f) {
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r = 12.0f;
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r, r), accentColorBrush.Get(), 1.5f);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                if (muted) {
                    const wchar_t* glyph = L"\uE74F";
                    if (iconFormat_) target_->DrawTextW(glyph, 1, iconFormat_.Get(), rect, whiteBrush.Get());
                } else {
                    wchar_t valStr[8] = {};
                    swprintf_s(valStr, L"%d", state.volume.percent);
                    target_->DrawTextW(valStr, static_cast<UINT32>(wcslen(valStr)), smallTextFormat_.Get(), rect, whiteBrush.Get());
                }
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        // ── 2. Split Pill Mode (62px Width) ──
        if (width < 100.0f) {
            const wchar_t* glyph = muted ? L"\uE74F" : (pct > 0.65f ? L"\uE767" : (pct > 0.32f ? L"\uE995" : L"\uE993"));
            D2D1_RECT_F speakerRect = D2D1::RectF(rect.left + 6.0f, cy - 10.0f, rect.left + 26.0f, cy + 10.0f);
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, static_cast<UINT32>(wcslen(glyph)), iconFormat_.Get(), speakerRect, whiteBrush.Get());
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            D2D1_RECT_F ringRect = D2D1::RectF(rect.right - 28.0f, cy - 11.0f, rect.right - 6.0f, cy + 11.0f);
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(rect.right - 17.0f, cy), 11.0f, 11.0f), accentColorBrush.Get(), 1.5f);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                wchar_t valStr[8] = {};
                swprintf_s(valStr, L"%d", state.volume.percent);
                target_->DrawTextW(valStr, static_cast<UINT32>(wcslen(valStr)), smallTextFormat_.Get(), ringRect, whiteBrush.Get());
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        if (rect.bottom - rect.top < 50.0f) {
            // Compact Volume View (native iOS style: icon left, slider bar right, height 36.0f)
            const float cy = (rect.top + rect.bottom) * 0.5f;
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 8.0f, cy - badgeSz * 0.5f,
                                            rect.left + 8.0f + badgeSz, cy + badgeSz * 0.5f);
            const float br = badgeSz * 0.30f;

            ComPtr<ID2D1SolidColorBrush> badgeBg;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.10f), &badgeBg);
            target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBg.Get());

            const wchar_t* glyph = muted ? L"\uE74F" : (pct > 0.65f ? L"\uE767" : (pct > 0.32f ? L"\uE995" : L"\uE993"));
            textBrush_->SetOpacity(0.95f);
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, static_cast<UINT32>(wcslen(glyph)),
                                   iconFormat_.Get(), badge, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            // Slider track on the right
            const float tx = badge.right + 10.0f;
            const float rx = rect.right - 12.0f;
            D2D1_RECT_F track = D2D1::RectF(tx, cy - 3.0f, rx, cy + 3.0f);

            ComPtr<ID2D1SolidColorBrush> trackBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.15f), &trackBrush);
            target_->FillRoundedRectangle(D2D1::RoundedRect(track, 3.0f, 3.0f), trackBrush.Get());

            D2D1_RECT_F fill = D2D1::RectF(track.left, track.top,
                                           track.left + (track.right - track.left) * pct, track.bottom);
            accentBrush_->SetOpacity(muted ? 0.25f : 0.85f);
            target_->FillRoundedRectangle(D2D1::RoundedRect(fill, 3.0f, 3.0f), accentBrush_.Get());
            accentBrush_->SetOpacity(1.0f);
            textBrush_->SetOpacity(1.0f);
            return;
        }

        if (rect.bottom - rect.top < 32.0f) return; // Keep compiler analysis happy

        const int   N     = 22;   // number of waveform bars
        const int   litN  = static_cast<int>(pct * N + 0.5f);

        // â”€â”€ Left: Speaker icon badge â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        const float badgeSz = 36.0f;
        const float badgeCY = rect.top + 8.0f + badgeSz * 0.5f;
        D2D1_RECT_F badge = D2D1::RectF(rect.left + 14, badgeCY - badgeSz * 0.5f,
                                         rect.left + 14 + badgeSz, badgeCY + badgeSz * 0.5f);
        const float br = badgeSz * 0.30f;
        ComPtr<ID2D1SolidColorBrush> badgeBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.10f), &badgeBg);
        target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBg.Get());

        // Speaker glyph â€” scale icon to 3 levels (Segoe MDL2 / Fluent Icons)
        const wchar_t* glyph = muted ? L"\uE74F"
                              : (pct > 0.65f ? L"\uE767" : (pct > 0.32f ? L"\uE995" : L"\uE993"));
        textBrush_->SetOpacity(0.95f);
        if (iconFormat_) {
            iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            target_->DrawTextW(glyph, static_cast<UINT32>(wcslen(glyph)),
                               iconFormat_.Get(), badge, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
            iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }

        // â”€â”€ Right of badge: labels â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        const float tx = badge.right + 12.0f;
        const float rx = rect.right - 14.0f;

        // "Volume" / device name â€” small, muted
        mutedBrush_->SetOpacity(0.50f);
        const std::wstring lbl = state.volume.deviceName.empty() ? L"Volume" : state.volume.deviceName;
        target_->DrawTextW(lbl.c_str(), static_cast<UINT32>(lbl.size()), smallTextFormat_.Get(),
                           D2D1::RectF(tx, rect.top + 10, rx - 44, badgeCY + 2),
                           mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);

        // Percentage value â€” white, right-aligned
        wchar_t valStr[16] = {};
        if (muted) wcscpy_s(valStr, ARRAYSIZE(valStr), L"Muted");
        else       swprintf_s(valStr, L"%d%%", state.volume.percent);
        textBrush_->SetOpacity(0.92f);
        target_->DrawTextW(valStr, static_cast<UINT32>(wcslen(valStr)), smallTextFormat_.Get(),
                           D2D1::RectF(rx - 44, rect.top + 10, rx, badgeCY + 2),
                           textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);

        // â”€â”€ iOS-style waveform bar chart â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        // Bars span from badge-right to rect-right, bottom of rect minus padding
        const float bLeft   = tx;
        const float bRight  = rx;
        const float bBottom = rect.bottom - 8.0f;
        const float bTop    = badgeCY + 10.0f;
        const float bH      = bBottom - bTop;     // available bar height
        const float barW    = 3.2f;
        const float gap     = (bRight - bLeft - N * barW) / std::max(N - 1, 1);

        for (int i = 0; i < N; i++) {
            const float t  = static_cast<float>(i) / (N - 1);  // 0..1
            // Sine profile â€” tall in middle, short at edges (matches the reference image)
            const float hf = 0.20f + 0.80f * std::sin(t * 3.14159f);
            const float bx = bLeft + i * (barW + gap);
            const bool  lit = (i < litN);

            if (lit) {
                accentBrush_->SetOpacity(muted ? 0.22f : 0.84f);
                target_->FillRoundedRectangle(
                    D2D1::RoundedRect(D2D1::RectF(bx, bBottom - bH * hf, bx + barW, bBottom), 1.6f, 1.6f),
                    accentBrush_.Get());
            } else {
                // Inactive position: small dim dot at bottom
                mutedBrush_->SetOpacity(0.22f);
                const float dotR = barW * 0.50f;
                target_->FillEllipse(
                    D2D1::Ellipse(D2D1::Point2F(bx + barW * 0.5f, bBottom - dotR), dotR, dotR),
                    mutedBrush_.Get());
            }
        }

        accentBrush_->SetOpacity(1.0f);
        mutedBrush_->SetOpacity(0.58f);
        textBrush_->SetOpacity(1.0f);
    }

    void DrawCapsLock(const SharedState& state, D2D1_RECT_F rect) {
        const float width = rect.right - rect.left;
        if (width < 20.0f) return;
        const float cy = (rect.top + rect.bottom) * 0.5f;
        const bool isNum = state.capsLock.isNumEvent;
        const bool isOn  = isNum ? state.capsLock.numOn : state.capsLock.capsOn;
        const wchar_t* glyph = isNum ? L"1" : L"A";
        const std::wstring label = isNum ? L"Num Lock" : L"Caps Lock";
        const std::wstring status = isOn ? L"ON" : L"OFF";

        ComPtr<ID2D1SolidColorBrush> greenBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f), &greenBrush);
        ComPtr<ID2D1SolidColorBrush> greyBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.25f), &greyBrush);
        ComPtr<ID2D1SolidColorBrush> whiteBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &whiteBrush);

        if (width < 45.0f) {
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r = 12.0f;
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r, r), isOn ? greenBrush.Get() : greyBrush.Get(), 1.5f);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, 1, smallTextFormat_.Get(), rect, isOn ? greenBrush.Get() : whiteBrush.Get());
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        if (width < 100.0f) {
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 6.0f, cy - badgeSz * 0.5f, rect.left + 6.0f + badgeSz, cy + badgeSz * 0.5f);
            const float br = badgeSz * 0.30f;
            ComPtr<ID2D1SolidColorBrush> badgeBg;
            target_->CreateSolidColorBrush(isOn ? D2D1::ColorF(0.10f, 0.55f, 0.20f, 0.28f) : D2D1::ColorF(1, 1, 1, 0.07f), &badgeBg);
            target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBg.Get());

            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, 1, smallTextFormat_.Get(), badge, isOn ? greenBrush.Get() : whiteBrush.Get());
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            D2D1_RECT_F statusRect = D2D1::RectF(rect.right - 28.0f, rect.top, rect.right - 4.0f, rect.bottom);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(status.c_str(), static_cast<UINT32>(status.size()), smallTextFormat_.Get(), statusRect, isOn ? greenBrush.Get() : whiteBrush.Get());
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        if (rect.bottom - rect.top < 40.0f) {
            // Compact Caps Lock View (height 36.0f)
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 8.0f, cy - badgeSz * 0.5f,
                                            rect.left + 8.0f + badgeSz, cy + badgeSz * 0.5f);
            const float br = badgeSz * 0.30f;

            ComPtr<ID2D1SolidColorBrush> badgeBg;
            target_->CreateSolidColorBrush(isOn ? D2D1::ColorF(0.10f, 0.55f, 0.20f, 0.28f) : D2D1::ColorF(1, 1, 1, 0.07f), &badgeBg);
            target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBg.Get());

            textBrush_->SetOpacity(isOn ? 1.0f : 0.75f);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, 1, smallTextFormat_.Get(), badge, textBrush_.Get());
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            const float tx = badge.right + 10.0f;
            D2D1_RECT_F labelRect = D2D1::RectF(tx, rect.top + 6.0f, rect.right - 54.0f, rect.bottom - 6.0f);
            mutedBrush_->SetOpacity(0.55f);
            target_->DrawTextW(label.c_str(), static_cast<UINT32>(label.size()), smallTextFormat_.Get(),
                               labelRect, mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);

            // Small glowing status indicator dot or small pill on the right
            const float pillW = isOn ? 26.0f : 30.0f;
            const float pillH = 18.0f;
            D2D1_RECT_F pill = D2D1::RectF(rect.right - 10.0f - pillW, cy - pillH * 0.5f,
                                            rect.right - 10.0f,          cy + pillH * 0.5f);
            ComPtr<ID2D1SolidColorBrush> pillBg;
            target_->CreateSolidColorBrush(isOn ? D2D1::ColorF(0.14f, 0.70f, 0.32f, 0.88f) : D2D1::ColorF(1, 1, 1, 0.10f), &pillBg);
            target_->FillRoundedRectangle(D2D1::RoundedRect(pill, 9.0f, 9.0f), pillBg.Get());

            textBrush_->SetOpacity(isOn ? 1.0f : 0.50f);
            target_->DrawTextW(status.c_str(), static_cast<UINT32>(status.size()), smallTextFormat_.Get(),
                               pill, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);

            textBrush_->SetOpacity(1.0f);
            mutedBrush_->SetOpacity(0.58f);
            return;
        }

        if (rect.bottom - rect.top < 24.0f) return; // Keep compiler analysis happy

        const float badgeSz = (rect.bottom - rect.top) - 16.0f;
        D2D1_RECT_F badge = D2D1::RectF(rect.left + 14, cy - badgeSz * 0.5f,
                                        rect.left + 14 + badgeSz, cy + badgeSz * 0.5f);
        const float br = badgeSz * 0.35f;

        // Badge background â€” green tint when ON, dim when OFF
        ComPtr<ID2D1SolidColorBrush> badgeBg;
        target_->CreateSolidColorBrush(
            isOn ? D2D1::ColorF(0.10f, 0.55f, 0.20f, 0.28f)
                 : D2D1::ColorF(1, 1, 1, 0.07f), &badgeBg);
        target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBg.Get());

        // Badge border
        ComPtr<ID2D1SolidColorBrush> badgeBorder;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, isOn ? 0.22f : 0.12f), &badgeBorder);
        target_->DrawRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBorder.Get(), 1.0f);

        // Key character centered
        textBrush_->SetOpacity(isOn ? 1.0f : 0.75f);
        target_->DrawTextW(glyph, static_cast<UINT32>(wcslen(glyph)), clockFormat_.Get(),
                           badge, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);

        // Status LED dot (bottom-right of badge)
        ComPtr<ID2D1SolidColorBrush> ledBrush;
        target_->CreateSolidColorBrush(
            isOn ? D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f)
                 : D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.18f), &ledBrush);
        target_->FillEllipse(D2D1::Ellipse(
            D2D1::Point2F(badge.right - 5.0f, badge.bottom - 5.0f), 3.0f, 3.0f), ledBrush.Get());

        // Label text (Caps Lock / Num Lock) â€” muted
        const float tx = badge.right + 14;
        mutedBrush_->SetOpacity(0.52f);
        target_->DrawTextW(label.c_str(), static_cast<UINT32>(label.size()),
                           smallTextFormat_.Get(),
                           D2D1::RectF(tx, cy - 11, rect.right - 52, cy + 5),
                           mutedBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);

        // Colored ON/OFF pill badge

        const float pillW = isOn ? 30.0f : 36.0f;
        const float pillH = 20.0f;
        D2D1_RECT_F pill = D2D1::RectF(rect.right - 14 - pillW, cy - pillH * 0.5f,
                                        rect.right - 14,          cy + pillH * 0.5f);
        ComPtr<ID2D1SolidColorBrush> pillBg;
        target_->CreateSolidColorBrush(
            isOn ? D2D1::ColorF(0.14f, 0.70f, 0.32f, 0.88f)
                 : D2D1::ColorF(1, 1, 1, 0.10f), &pillBg);
        target_->FillRoundedRectangle(D2D1::RoundedRect(pill, 10.0f, 10.0f), pillBg.Get());

        textBrush_->SetOpacity(isOn ? 1.0f : 0.50f);
        target_->DrawTextW(status.c_str(), static_cast<UINT32>(status.size()),
                           smallTextFormat_.Get(), pill, textBrush_.Get(),
                           D2D1_DRAW_TEXT_OPTIONS_CLIP);

        textBrush_->SetOpacity(1.0f);
        mutedBrush_->SetOpacity(0.58f);
    }

    // Loads airpods.png from the previews folder next to the mod's host process.
    // Returns true on first success; result is cached in airpodsPngPixels_.
    bool LoadAirPodsPng() {
        if (airpodsPngLoaded_) return !airpodsPngPixels_.bgra.empty();
        airpodsPngLoaded_ = true;

        ComPtr<IWICImagingFactory> wic;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic));
        if (FAILED(hr)) return false;

        // Try a list of candidate paths for airpods.png
        // 1. Next to the host EXE (DynamicIsland.exe or Windhawk)
        // 2. Next to the loaded DLL/mod
        // 3. Hardcoded project folder (dev machine fallback)
        // 4. Current working directory
        const std::vector<std::wstring> candidates = [&]() -> std::vector<std::wstring> {
            std::vector<std::wstring> paths;
            wchar_t buf[MAX_PATH] = {};

            // Candidate 1: next to the host EXE
            if (GetModuleFileNameW(nullptr, buf, MAX_PATH)) {
                wchar_t* sl = wcsrchr(buf, L'\\');
                if (sl) { *sl = L'\0'; paths.push_back(std::wstring(buf) + L"\\previews\\music.png"); }
            }
            // Candidate 2: next to this loaded module (the mod DLL)
            HMODULE self = nullptr;
            GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                               GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               reinterpret_cast<LPCWSTR>(&self), &self);
            if (self && GetModuleFileNameW(self, buf, MAX_PATH)) {
                wchar_t* sl = wcsrchr(buf, L'\\');
                if (sl) { *sl = L'\0'; paths.push_back(std::wstring(buf) + L"\\previews\\music.png"); }
            }
            // Candidate 3: hardcoded project directory (developer machine)
            paths.push_back(L"C:\\Users\\AONAK YADAV\\Dynamic-Island-for-Windows\\previews\\music.png");
            // Candidate 4: current working directory
            if (GetCurrentDirectoryW(MAX_PATH, buf))
                paths.push_back(std::wstring(buf) + L"\\previews\\music.png");
            return paths;
        }();

        ComPtr<IWICBitmapDecoder> decoder;
        for (const auto& candidate : candidates) {
            hr = wic->CreateDecoderFromFilename(candidate.c_str(), nullptr,
                GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
            if (SUCCEEDED(hr)) break;
        }
        if (FAILED(hr)) return false;

        ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) return false;

        ComPtr<IWICFormatConverter> conv;
        hr = wic->CreateFormatConverter(&conv);
        if (FAILED(hr)) return false;

        hr = conv->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
        if (FAILED(hr)) return false;

        UINT w = 0, h = 0;
        conv->GetSize(&w, &h);
        if (!w || !h) return false;

        airpodsPngPixels_.width = w;
        airpodsPngPixels_.height = h;
        airpodsPngPixels_.bgra.resize(static_cast<size_t>(w) * h * 4);
        hr = conv->CopyPixels(nullptr, w * 4, static_cast<UINT>(airpodsPngPixels_.bgra.size()),
                              airpodsPngPixels_.bgra.data());
        if (FAILED(hr)) {
            airpodsPngPixels_ = {};
            return false;
        }
        airpodsPngPixels_.generation = 1;
        return true;
    }

    // Draws the music.png image centered and scaled to fit `rect`,
    // with a solid white circular background to make the black PNG clearly visible.
    void DrawAirPodsIcon(D2D1_RECT_F rect, ComPtr<ID2D1SolidColorBrush> /*brush*/) {
        LoadAirPodsPng();

        // 1. Draw solid white circular background
        ComPtr<ID2D1SolidColorBrush> whiteBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &whiteBg);
        const float cx = (rect.left + rect.right) * 0.5f;
        const float cy = (rect.top + rect.bottom) * 0.5f;
        const float r = (rect.right - rect.left) * 0.5f;
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r, r), whiteBg.Get());

        // 2. Draw black PNG icon slightly inset so it looks very clean
        const float inset = r * 0.35f;
        D2D1_RECT_F pngRect = D2D1::RectF(rect.left + inset, rect.top + inset, rect.right - inset, rect.bottom - inset);

        if (!airpodsPngPixels_.bgra.empty()) {
            DrawBitmapPixels(airpodsPngPixels_, pngRect, airpodsBitmap_, airpodsBitmapGen_);
            return;
        }
        // Fallback: small black circle if PNG fails to load
        ComPtr<ID2D1SolidColorBrush> blackFb;
        target_->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.90f), &blackFb);
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r * 0.4f, r * 0.4f), blackFb.Get());
    }


    std::wstring CleanDeviceName(const std::wstring& name) {
        std::wstring s = name;
        auto IsMatch = [](const std::wstring& str, const std::wstring& pattern) {
            if (str.length() < pattern.length()) return false;
            std::wstring s_lower = str;
            for (auto& c : s_lower) c = std::towlower(c);
            std::wstring p_lower = pattern;
            for (auto& c : p_lower) c = std::towlower(c);
            return s_lower.find(p_lower) != std::wstring::npos;
        };
        
        if (IsMatch(s, L"Headphones (") || IsMatch(s, L"Headset (")) {
            size_t start = s.find(L"(");
            if (start != std::wstring::npos) {
                size_t end = s.find(L")", start);
                if (end != std::wstring::npos) {
                    return s.substr(start + 1, end - start - 1);
                } else {
                    return s.substr(start + 1);
                }
            }
        }
        
        std::vector<std::wstring> toRemove = {
            L" Headphones", L"Headphones",
            L" Headset", L"Headset",
            L" Hands-Free AG Audio", L" Hands-Free", L"Hands-Free",
            L" Stereo", L"Stereo"
        };
        
        for (const auto& pattern : toRemove) {
            size_t pos = 0;
            std::wstring s_lower = s;
            for (auto& c : s_lower) c = std::towlower(c);
            std::wstring p_lower = pattern;
            for (auto& c : p_lower) c = std::towlower(c);
            
            while ((pos = s_lower.find(p_lower)) != std::wstring::npos) {
                s.erase(pos, pattern.length());
                s_lower.erase(pos, pattern.length());
            }
        }
        
        size_t first = s.find_first_not_of(L" \t\r\n");
        if (first == std::wstring::npos) return L"";
        size_t last = s.find_last_not_of(L" \t\r\n");
        return s.substr(first, (last - first + 1));
    }

    void DrawDevice(const SharedState& state, D2D1_RECT_F rect) {
        const float width  = rect.right  - rect.left;
        const float height = rect.bottom - rect.top;
        const float cy     = (rect.top + rect.bottom) * 0.5f;
        const bool connected = (state.device.eventType == DeviceEventType::Connected);

        // Any Bluetooth-like device gets the AirPods icon treatment
        const bool isAirPods = state.device.isBluetoothLike ||
                               state.device.deviceName.find(L"AirPods")  != std::wstring::npos ||
                               state.device.deviceName.find(L"Air Pods") != std::wstring::npos ||
                               state.device.deviceName.find(L"Buds")     != std::wstring::npos;

        Wh_Log(L"DrawDevice: rect={%.1f, %.1f, %.1f, %.1f}, isAirPods=%d, connected=%d, name='%ls'", 
               rect.left, rect.top, rect.right, rect.bottom, isAirPods, connected, state.device.deviceName.c_str());

        ComPtr<ID2D1SolidColorBrush> whiteBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &whiteBrush);

        ComPtr<ID2D1SolidColorBrush> greenBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f), &greenBrush);

        // ── 1. Split Circle Mode (≤45px wide — secondary bubble) ──
        if (width < 45.0f) {
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r  = 12.0f;
            if (isAirPods) {
                D2D1_RECT_F iconRect = D2D1::RectF(cx - r, cy - r, cx + r, cy + r);
                DrawAirPodsIcon(iconRect, whiteBrush);
            } else {
                ComPtr<ID2D1SolidColorBrush> dotBrush;
                target_->CreateSolidColorBrush(
                    connected ? D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f)
                              : D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &dotBrush);
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), 5.0f, 5.0f), dotBrush.Get());
            }
            return;
        }

        // ── 2. Split Pill Mode (≤100px wide) ──
        if (width < 100.0f) {
            D2D1_RECT_F iconRect = D2D1::RectF(rect.left + 4.0f, cy - 13.0f, rect.left + 30.0f, cy + 13.0f);
            if (isAirPods) {
                DrawAirPodsIcon(iconRect, whiteBrush);
            } else {
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.left + 16.0f, cy), 5.0f, 5.0f), greenBrush.Get());
            }
            return;
        }

        // ── 3. Compact Pill (height ≤45px) ──
        if (height < 45.0f) {
            D2D1_RECT_F iconRect = D2D1::RectF(rect.left + 10.0f, cy - 13.0f, rect.left + 36.0f, cy + 13.0f);
            if (isAirPods) {
                DrawAirPodsIcon(iconRect, whiteBrush);
            } else {
                target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.left + 22.0f, cy), 5.0f, 5.0f), greenBrush.Get());
            }

            std::wstring rawName = state.device.deviceName.empty()
                ? (isAirPods ? L"AirPods" : L"Device")
                : state.device.deviceName;
            std::wstring cleanName = CleanDeviceName(rawName);
            std::wstring statusText = connected ? L"Connected: " : L"Disconnected: ";
            std::wstring dispName = statusText + cleanName;

            D2D1_RECT_F nameRect = D2D1::RectF(rect.left + 42.0f, rect.top, rect.right - 12.0f, rect.bottom);
            textBrush_->SetOpacity(0.92f);
            if (smallTextFormat_) {
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                target_->DrawTextW(dispName.c_str(), static_cast<UINT32>(dispName.size()),
                                   smallTextFormat_.Get(), nameRect, textBrush_.Get(),
                                   D2D1_DRAW_TEXT_OPTIONS_NONE);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            textBrush_->SetOpacity(1.0f);
            return;
        }

        // ——— 4. Expanded View (matches reference image) ———
        // Left: AirPods PNG icon
        const float iconSize = 40.0f;
        D2D1_RECT_F iconRect = D2D1::RectF(rect.left + 14.0f, cy - iconSize * 0.5f,
                                            rect.left + 14.0f + iconSize, cy + iconSize * 0.5f);
        if (isAirPods) {
            DrawAirPodsIcon(iconRect, whiteBrush);
        } else {
            target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.left + 34.0f, cy), 14.0f, 14.0f), whiteBrush.Get());
        }

        // Center text — two lines: small grey "Connected" / "Disconnected",
        // then bold white device name
        const float textLeft  = rect.left + 62.0f;
        const float textRight = rect.right - 20.0f;

        // Top sub-label (Connected / Disconnected)
        std::wstring subLabel = connected ? L"Connected" : L"Disconnected";
        ComPtr<ID2D1SolidColorBrush> subBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.50f), &subBrush);
        D2D1_RECT_F subRect = D2D1::RectF(textLeft, cy - 20.0f, textRight, cy - 2.0f);
        if (smallTextFormat_) {
            smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            target_->DrawTextW(subLabel.c_str(), static_cast<UINT32>(subLabel.size()),
                               smallTextFormat_.Get(), subRect, subBrush.Get(),
                               D2D1_DRAW_TEXT_OPTIONS_NONE);
            smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }

        // Bold device name
        std::wstring rawName = state.device.deviceName.empty()
            ? (isAirPods ? L"AirPods" : L"Bluetooth Device")
            : state.device.deviceName;
        std::wstring dispName = CleanDeviceName(rawName);
        D2D1_RECT_F nameRect = D2D1::RectF(textLeft, cy + 2.0f, textRight, cy + 24.0f);
        if (boldTextFormat_) {
            boldTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            boldTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            target_->DrawTextW(dispName.c_str(), static_cast<UINT32>(dispName.size()),
                               boldTextFormat_.Get(), nameRect, whiteBrush.Get(),
                               D2D1_DRAW_TEXT_OPTIONS_NONE);
            boldTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // Restore
            boldTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }
    }


    void DrawNotificationFallbackIcon(D2D1_POINT_2F center, float radius) {
        ComPtr<ID2D1SolidColorBrush> bg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.055f * settingsOpacity_), &bg);
        target_->FillEllipse(D2D1::Ellipse(center, radius, radius), bg.Get());

        accentBrush_->SetOpacity(0.92f);
        
        ComPtr<ID2D1PathGeometry> bellGeom;
        d2dFactory_->CreatePathGeometry(&bellGeom);
        ComPtr<ID2D1GeometrySink> sink;
        if (SUCCEEDED(bellGeom->Open(&sink))) {
            const float r = radius;
            sink->BeginFigure(D2D1::Point2F(center.x - r * 0.15f, center.y - r * 0.7f), D2D1_FIGURE_BEGIN_FILLED);
            sink->AddBezier(D2D1::BezierSegment(
                D2D1::Point2F(center.x - r * 0.4f, center.y - r * 0.7f),
                D2D1::Point2F(center.x - r * 0.5f, center.y - r * 0.2f),
                D2D1::Point2F(center.x - r * 0.55f, center.y + r * 0.2f)
            ));
            sink->AddBezier(D2D1::BezierSegment(
                D2D1::Point2F(center.x - r * 0.6f, center.y + r * 0.45f),
                D2D1::Point2F(center.x - r * 0.85f, center.y + r * 0.55f),
                D2D1::Point2F(center.x - r * 0.85f, center.y + r * 0.6f)
            ));
            sink->AddLine(D2D1::Point2F(center.x + r * 0.85f, center.y + r * 0.6f));
            sink->AddBezier(D2D1::BezierSegment(
                D2D1::Point2F(center.x + r * 0.85f, center.y + r * 0.55f),
                D2D1::Point2F(center.x + r * 0.6f, center.y + r * 0.45f),
                D2D1::Point2F(center.x + r * 0.55f, center.y + r * 0.2f)
            ));
            sink->AddBezier(D2D1::BezierSegment(
                D2D1::Point2F(center.x + r * 0.5f, center.y - r * 0.2f),
                D2D1::Point2F(center.x + r * 0.4f, center.y - r * 0.7f),
                D2D1::Point2F(center.x + r * 0.15f, center.y - r * 0.7f)
            ));
            sink->EndFigure(D2D1_FIGURE_END_CLOSED);
            sink->Close();

            target_->FillGeometry(bellGeom.Get(), accentBrush_.Get());
            target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y + r * 0.7f), r * 0.22f, r * 0.22f), accentBrush_.Get());
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y - r * 0.75f), r * 0.18f, r * 0.18f), accentBrush_.Get(), 1.4f);
        }

        ComPtr<ID2D1SolidColorBrush> badgeColor;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &badgeColor);
        const float badgeR = radius * 0.28f;
        const float badgeX = center.x + radius * 0.65f;
        const float badgeY = center.y - radius * 0.5f;
        target_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(badgeX, badgeY), badgeR, badgeR), badgeColor.Get());

        ComPtr<ID2D1SolidColorBrush> badgeBorder;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.95f), &badgeBorder);
        target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(badgeX, badgeY), badgeR, badgeR), badgeBorder.Get(), 0.9f);

        accentBrush_->SetOpacity(1.0f);
    }

    void DrawBattery(const SharedState& state, D2D1_RECT_F rect) {
        if (rect.right - rect.left < 45.0f) {
            // â”€â”€ Split Circle Mode (36px x 36px) â”€â”€
            const float cy = (rect.top + rect.bottom) * 0.5f;
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r = 12.0f;

            D2D1_COLOR_F color = state.battery.charging 
                ? D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f)  // Green
                : (state.battery.low ? D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f) : D2D1::ColorF(1.0f, 0.60f, 0.0f, 1.0f)); // Red/Orange

            ComPtr<ID2D1SolidColorBrush> brush;
            target_->CreateSolidColorBrush(color, &brush);

            // Draw circular progress track (thin ring)
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r, r), brush.Get(), 1.5f);

            // Centered percentage number
            wchar_t valStr[8] = {};
            swprintf_s(valStr, L"%d", state.battery.percent);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(valStr, static_cast<UINT32>(wcslen(valStr)), smallTextFormat_.Get(),
                                   rect, brush.Get());
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        const float cy = (rect.top + rect.bottom) * 0.5f;
        const float pct = Clamp(state.battery.percent / 100.0f, 0.0f, 1.0f);
        const bool low = state.battery.low;
        const bool charging = state.battery.charging;

        if (rect.right - rect.left < 100.0f) {
            // â”€â”€ Split Pill Mode (62px Width, Matches iOS Split View) â”€â”€
            // Render only Percentage and Battery Icon (NO label text)
            D2D1_COLOR_F pctColor = charging 
                ? D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f) // Green
                : (low ? D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f) : D2D1::ColorF(1.0f, 0.60f, 0.0f, 1.0f)); // Red/Orange
            ComPtr<ID2D1SolidColorBrush> pctBrush;
            target_->CreateSolidColorBrush(pctColor, &pctBrush);

            wchar_t valStr[16] = {};
            swprintf_s(valStr, L"%d%%", state.battery.percent);
            
            D2D1_RECT_F pctRect = D2D1::RectF(rect.left + 4.0f, rect.top, rect.right - 26.0f, rect.bottom);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(valStr, static_cast<UINT32>(wcslen(valStr)), smallTextFormat_.Get(),
                                   pctRect, pctBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            // Battery Icon (Right)
            const float bw = 18.0f;
            const float bh = 9.0f;
            D2D1_RECT_F batRect = D2D1::RectF(rect.right - 22.0f, cy - bh * 0.5f, rect.right - 4.0f, cy + bh * 0.5f);
            
            ComPtr<ID2D1SolidColorBrush> borderBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.20f), &borderBrush);
            target_->DrawRoundedRectangle(D2D1::RoundedRect(batRect, 1.5f, 1.5f), borderBrush.Get(), 1.0f);

            D2D1_RECT_F nub = D2D1::RectF(batRect.right, cy - 2.0f, batRect.right + 1.2f, cy + 2.0f);
            target_->FillRectangle(nub, borderBrush.Get());

            D2D1_RECT_F fill = D2D1::RectF(batRect.left + 1.2f, batRect.top + 1.2f,
                                           batRect.left + 1.2f + (bw - 2.4f) * pct, batRect.bottom - 1.2f);
            target_->FillRoundedRectangle(D2D1::RoundedRect(fill, 0.8f, 0.8f), pctBrush.Get());
            return;
        }

        if (rect.bottom - rect.top < 40.0f) {
            // â”€â”€ Compact Pill View (36px Height, Matches Figma Exactly) â”€â”€
            // Left: "Charging" or "Low Battery" (White text)
            std::wstring label = charging ? L"Charging" : L"Low Battery";
            D2D1_RECT_F labelRect = D2D1::RectF(rect.left + 12.0f, rect.top + 6.0f, rect.left + 120.0f, rect.bottom - 6.0f);
            textBrush_->SetOpacity(0.92f);
            if (smallTextFormat_) {
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(label.c_str(), static_cast<UINT32>(label.size()), smallTextFormat_.Get(),
                                   labelRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            // Right: Percentage text (Green if charging, Red/Orange if low)
            D2D1_COLOR_F pctColor = charging 
                ? D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f) // Green
                : (low ? D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f) : D2D1::ColorF(1.0f, 0.60f, 0.0f, 1.0f)); // Red/Orange
            ComPtr<ID2D1SolidColorBrush> pctBrush;
            target_->CreateSolidColorBrush(pctColor, &pctBrush);

            wchar_t valStr[16] = {};
            swprintf_s(valStr, L"%d %%", state.battery.percent);
            
            // Render percentage aligned to the right of battery icon
            D2D1_RECT_F pctRect = D2D1::RectF(rect.right - 68.0f, rect.top + 6.0f, rect.right - 34.0f, rect.bottom - 6.0f);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(valStr, static_cast<UINT32>(wcslen(valStr)), smallTextFormat_.Get(),
                                   pctRect, pctBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            // Rightmost: Battery Fill Icon (same design as Figma)
            const float bw = 22.0f;
            const float bh = 11.0f;
            D2D1_RECT_F batRect = D2D1::RectF(rect.right - 30.0f, cy - bh * 0.5f, rect.right - 8.0f, cy + bh * 0.5f);
            
            // Draw battery border (semi-translucent white/grey or colored)
            ComPtr<ID2D1SolidColorBrush> borderBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.24f), &borderBrush);
            target_->DrawRoundedRectangle(D2D1::RoundedRect(batRect, 2.0f, 2.0f), borderBrush.Get(), 1.0f);

            // Battery nub
            D2D1_RECT_F nub = D2D1::RectF(batRect.right, cy - 2.0f, batRect.right + 1.5f, cy + 2.0f);
            target_->FillRectangle(nub, borderBrush.Get());

            // Fill block
            D2D1_RECT_F fill = D2D1::RectF(batRect.left + 1.5f, batRect.top + 1.5f,
                                           batRect.left + 1.5f + (bw - 3.0f) * pct, batRect.bottom - 1.5f);
            target_->FillRoundedRectangle(D2D1::RoundedRect(fill, 1.0f, 1.0f), pctBrush.Get());
            
            textBrush_->SetOpacity(1.0f);
            return;
        }

        if (rect.bottom - rect.top < 24.0f) return; // Keep compiler analysis happy

        const float badgeSz = (rect.bottom - rect.top) - 16.0f;
        D2D1_RECT_F badge = D2D1::RectF(rect.left + 14, cy - badgeSz * 0.5f,
                                        rect.left + 14 + badgeSz, cy + badgeSz * 0.5f);
        const float br = badgeSz * 0.35f;

        ComPtr<ID2D1SolidColorBrush> badgeBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.12f), &badgeBg);
        target_->FillRoundedRectangle(D2D1::RoundedRect(badge, br, br), badgeBg.Get());

        // Draw battery vector icon
        const float bx = badge.left + badgeSz * 0.25f;
        const float by = cy - badgeSz * 0.22f;
        const float bw = badgeSz * 0.45f;
        const float bh = badgeSz * 0.44f;
        D2D1_RECT_F batRect = D2D1::RectF(bx, by, bx + bw, by + bh);
        
        ComPtr<ID2D1SolidColorBrush> batBorder;
        target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.85f), &batBorder);
        target_->DrawRoundedRectangle(D2D1::RoundedRect(batRect, 2, 2), batBorder.Get(), 1.5f);
        
        // Battery Terminal (Nub)
        D2D1_RECT_F nubRect = D2D1::RectF(batRect.right, cy - 3, batRect.right + 2.5f, cy + 3);
        target_->FillRectangle(nubRect, batBorder.Get());

        // Battery Fill

        D2D1_RECT_F fillRect = D2D1::RectF(batRect.left + 2, batRect.top + 2,
                                           batRect.left + 2 + (bw - 4) * pct, batRect.bottom - 2);
        
        ComPtr<ID2D1SolidColorBrush> batFill;
        if (state.battery.low) {
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &batFill); // Red
        } else if (state.battery.charging) {
            target_->CreateSolidColorBrush(D2D1::ColorF(0.19f, 0.83f, 0.38f, 1.0f), &batFill); // Green
        } else {
            target_->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 0.95f), &batFill); // White
        }
        target_->FillRoundedRectangle(D2D1::RoundedRect(fillRect, 1, 1), batFill.Get());

        // Text Labels
        const float tx = badge.right + 14;
        D2D1_RECT_F labelRect = D2D1::RectF(tx, cy - 22, rect.right - 14, cy - 6);
        mutedBrush_->SetOpacity(0.50f);
        std::wstring label = state.battery.charging ? L"Power Connected" : L"Battery Alert";
        target_->DrawTextW(label.c_str(), static_cast<UINT32>(label.size()),
                           smallTextFormat_.Get(), labelRect, mutedBrush_.Get(),
                           D2D1_DRAW_TEXT_OPTIONS_CLIP);

        wchar_t value[128] = {};
        if (state.battery.secondsRemaining != BATTERY_LIFE_UNKNOWN && !state.battery.charging) {
            const DWORD minutes = state.battery.secondsRemaining / 60;
            swprintf_s(value, ARRAYSIZE(value), L"%d%% \u2022 %luh %02lum left", 
                       state.battery.percent, minutes / 60, minutes % 60);
        } else {
            swprintf_s(value, ARRAYSIZE(value), L"%d%%", state.battery.percent);
        }

        D2D1_RECT_F valueRect = D2D1::RectF(tx, cy - 4, rect.right - 14, cy + 17);
        textBrush_->SetOpacity(0.95f);
        target_->DrawTextW(value, static_cast<UINT32>(wcslen(value)), textFormat_.Get(),
                           valueRect, textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
        textBrush_->SetOpacity(0.90f);
    }

    void DrawProgress(const SharedState& state, D2D1_RECT_F rect) {
        wchar_t buffer[64] = {};
        swprintf_s(buffer, L"Progress %d%%", state.progress.percent);
        const float paddingY = (rect.bottom - rect.top < 40.0f) ? 6.0f : 14.0f;
        target_->DrawTextW(buffer, static_cast<UINT32>(wcslen(buffer)), textFormat_.Get(),
                           D2D1::RectF(rect.left + 18, rect.top + paddingY, rect.right - 18,
                                       rect.bottom - (paddingY - 4.0f)),
                           textBrush_.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }

    void DrawTimer(const SharedState& state, D2D1_RECT_F rect, float scale) {
        ULONGLONG nowTick = GetTickCount64();
        ULONGLONG remainingMs = 0;
        if (g_timerActive) {
            if (g_timerPaused) {
                remainingMs = g_timerPauseRemainingMs;
            } else if (g_timerTargetTick > nowTick) {
                remainingMs = g_timerTargetTick - nowTick;
            }
        }
        int hours = static_cast<int>(remainingMs / 3600000);
        int minutes = static_cast<int>((remainingMs % 3600000) / 60000);
        int seconds = static_cast<int>((remainingMs % 60000) / 1000);
        wchar_t timeStr[64] = {};
        if (hours > 0) {
            swprintf_s(timeStr, L"%02d:%02d:%02d", hours, minutes, seconds);
        } else {
            swprintf_s(timeStr, L"%02d:%02d", minutes, seconds);
        }

        const float width = rect.right - rect.left;
        const float height = rect.bottom - rect.top;

        // â”€â”€ 1. Split Circle Mode (36px x 36px) â”€â”€
        if (width < 45.0f) {
            const float cy = (rect.top + rect.bottom) * 0.5f;
            const float cx = (rect.left + rect.right) * 0.5f;
            const float r = 12.0f * scale;

            ComPtr<ID2D1SolidColorBrush> timerAccent;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.52f, 0.0f, 1.0f), &timerAccent); // Orange

            // Circular ring outline
            target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), r, r), timerAccent.Get(), 1.5f);

            // Centered clock icon (Fluent Stopwatch glyph: L"\uE916")
            const wchar_t* glyph = L"\uE916";
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, 1, iconFormat_.Get(), rect, timerAccent.Get());
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        ComPtr<ID2D1SolidColorBrush> orangeBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.52f, 0.0f, 1.0f), &orangeBrush);

        const float cy = (rect.top + rect.bottom) * 0.5f;

        // ── 2. Split Pill Mode (62px Width) ──
        if (width < 100.0f) {
            D2D1_RECT_F iconRect = D2D1::RectF(rect.left + 4.0f, cy - 10.0f, rect.left + 24.0f, cy + 10.0f);
            const wchar_t* stopwatchGlyph = L"\uE916";
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(stopwatchGlyph, 1, iconFormat_.Get(), iconRect, orangeBrush.Get());
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            D2D1_RECT_F timeRect = D2D1::RectF(rect.left + 26.0f, rect.top, rect.right - 4.0f, rect.bottom);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(timeStr, static_cast<UINT32>(wcslen(timeStr)), smallTextFormat_.Get(),
                                   timeRect, orangeBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        // ── 3. Compact Pill Mode (36px Height, Matches Figma) ──
        if (height < 50.0f) {
            const float cyLocal = (rect.top + rect.bottom) * 0.5f;
            
            // Left: Orange Timer icon badge
            const float badgeSz = 24.0f;
            D2D1_RECT_F badge = D2D1::RectF(rect.left + 8.0f, cyLocal - badgeSz * 0.5f, rect.left + 8.0f + badgeSz, cyLocal + badgeSz * 0.5f);
            const wchar_t* glyph = L"\uE916";
            if (iconFormat_) {
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(glyph, 1, iconFormat_.Get(), badge, orangeBrush.Get());
                iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }

            // Right: Orange time string (e.g. "2:56")
            D2D1_RECT_F textRect = D2D1::RectF(badge.right + 10.0f, rect.top + 6.0f, rect.right - 12.0f, rect.bottom - 6.0f);
            if (smallTextFormat_) {
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                target_->DrawTextW(timeStr, static_cast<UINT32>(wcslen(timeStr)), smallTextFormat_.Get(),
                                   textRect, orangeBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
                smallTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                smallTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            }
            return;
        }

        // ——— 3. Expanded View (Vibrant iOS-style stacked layout) ———

        // Visual alert pulsing
        if (g_timerAlertActive) {
            float pulse = 0.5f + 0.5f * std::sin(static_cast<float>(nowTick / 100.0f));
            ComPtr<ID2D1SolidColorBrush> alertBrush;
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, pulse * 0.18f), &alertBrush);
            target_->FillRoundedRectangle(D2D1::RoundedRect(rect, 20.0f * scale, 20.0f * scale), alertBrush.Get());
        }

        // Left Side Buttons:
        // A. Pause/Resume button (Circle center x=36, y=cy, radius=22)
        D2D1_POINT_2F pauseCenter = D2D1::Point2F(rect.left + 36.0f, cy);
        ComPtr<ID2D1SolidColorBrush> pauseBg;
        ComPtr<ID2D1SolidColorBrush> pauseIconBrush;

        if (g_timerAlertActive) {
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 0.20f), &pauseBg);
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &pauseIconBrush);
        } else if (g_timerPaused) {
            // Green theme for Resume
            target_->CreateSolidColorBrush(D2D1::ColorF(0.18f, 0.8f, 0.44f, 0.18f), &pauseBg);
            target_->CreateSolidColorBrush(D2D1::ColorF(0.18f, 0.8f, 0.44f, 1.0f), &pauseIconBrush);
        } else {
            // Orange theme for Pause
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.52f, 0.0f, 0.18f), &pauseBg);
            target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.52f, 0.0f, 1.0f), &pauseIconBrush);
        }

        target_->FillEllipse(D2D1::Ellipse(pauseCenter, 22.0f, 22.0f), pauseBg.Get());

        // Center pause/resume icon glyph (Fluent pause L"\uE769" or play L"\uE768")
        const wchar_t* pGlyph = g_timerPaused ? L"\uE768" : L"\uE769";
        D2D1_RECT_F pauseBtnRect = D2D1::RectF(pauseCenter.x - 12.0f, pauseCenter.y - 12.0f, pauseCenter.x + 12.0f, pauseCenter.y + 12.0f);
        if (iconFormat_) {
            iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            target_->DrawTextW(pGlyph, 1, iconFormat_.Get(), pauseBtnRect, pauseIconBrush.Get());
        }

        // B. Cancel button (Circle center x=92, y=cy, radius=22)
        D2D1_POINT_2F cancelCenter = D2D1::Point2F(rect.left + 92.0f, cy);
        ComPtr<ID2D1SolidColorBrush> cancelBg;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.15f), &cancelBg);
        target_->FillEllipse(D2D1::Ellipse(cancelCenter, 22.0f, 22.0f), cancelBg.Get());

        // Center 'X' glyph (Fluent cancel L"\uE711" in white/gray)
        const wchar_t* cGlyph = L"\uE711";
        ComPtr<ID2D1SolidColorBrush> cancelIconBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.85f), &cancelIconBrush);
        D2D1_RECT_F cancelBtnRect = D2D1::RectF(cancelCenter.x - 12.0f, cancelCenter.y - 12.0f, cancelCenter.x + 12.0f, cancelCenter.y + 12.0f);
        if (iconFormat_) {
            target_->DrawTextW(cGlyph, 1, iconFormat_.Get(), cancelBtnRect, cancelIconBrush.Get());
            
            // Restore icon alignments
            iconFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            iconFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }

        // Right Side: Stacked labels in clean iOS style
        // "Timer" label in muted white, positioned above the big time string
        ComPtr<ID2D1SolidColorBrush> labelBrush;
        target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.50f), &labelBrush);
        D2D1_RECT_F labelRect = D2D1::RectF(rect.right - 150.0f, cy - 24.0f, rect.right - 14.0f, cy - 4.0f);
        if (boldTextFormat_) {
            boldTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
            boldTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            target_->DrawTextW(L"Timer", 5, boldTextFormat_.Get(), labelRect, labelBrush.Get());
            boldTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            boldTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }

        // Timer remaining time (large bold orange/accent or green/grey when paused)
        D2D1_RECT_F valRect = D2D1::RectF(rect.right - 150.0f, cy - 4.0f, rect.right - 14.0f, cy + 28.0f);
        if (clockFormat_) {
            clockFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
            clockFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            
            ComPtr<ID2D1SolidColorBrush> valBrush;
            if (g_timerAlertActive) {
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.23f, 0.18f, 1.0f), &valBrush); // Red
            } else if (g_timerPaused) {
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.60f), &valBrush); // Muted white
            } else {
                target_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.52f, 0.0f, 1.0f), &valBrush); // Orange
            }
            
            target_->DrawTextW(timeStr, static_cast<UINT32>(wcslen(timeStr)), clockFormat_.Get(), valRect, valBrush.Get());
            clockFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            clockFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }
    }

    void DrawProgressRing(D2D1_RECT_F rect, int percent) {
        ComPtr<ID2D1PathGeometry> geometry;
        d2dFactory_->CreatePathGeometry(&geometry);
        ComPtr<ID2D1GeometrySink> sink;
        geometry->Open(&sink);

        const float cx = (rect.left + rect.right) * 0.5f;
        const float cy = (rect.top + rect.bottom) * 0.5f;
        const float rx = (rect.right - rect.left) * 0.5f + 6.0f;
        const float ry = (rect.bottom - rect.top) * 0.5f + 6.0f;
        const float start = -3.14159265f * 0.5f;
        const float sweep = 2.0f * 3.14159265f * Clamp(percent / 100.0f, 0.0f, 1.0f);
        const int segments = std::max(2, static_cast<int>(48 * percent / 100.0f));

        auto pointAt = [&](float a) {
            return D2D1::Point2F(cx + std::cos(a) * rx, cy + std::sin(a) * ry);
        };

        sink->BeginFigure(pointAt(start), D2D1_FIGURE_BEGIN_HOLLOW);
        for (int i = 1; i <= segments; ++i) {
            const float a = start + sweep * i / segments;
            sink->AddLine(pointAt(a));
        }
        sink->EndFigure(D2D1_FIGURE_END_OPEN);
        sink->Close();

        accentBrush_->SetOpacity(0.92f);
        target_->DrawGeometry(geometry.Get(), accentBrush_.Get(), 3.0f);
        accentBrush_->SetOpacity(1.0f);
    }

    HWND hwnd_ = nullptr;
    HDC memDc_ = nullptr;
    HBITMAP dib_ = nullptr;
    HBITMAP oldBitmap_ = nullptr;
    int bitmapWidth_ = 0;
    int bitmapHeight_ = 0;

    ComPtr<ID2D1Factory> d2dFactory_;
    ComPtr<ID2D1DCRenderTarget> target_;
    ComPtr<IDWriteFactory> dwriteFactory_;
    ComPtr<IDWriteTextFormat> textFormat_;
    ComPtr<IDWriteTextFormat> smallTextFormat_;
    ComPtr<IDWriteTextFormat> boldTextFormat_;
    ComPtr<IDWriteTextFormat> hugeTextFormat_;
    ComPtr<IDWriteTextFormat> clockFormat_;
    ComPtr<IDWriteTextFormat> iconFormat_;
    ComPtr<ID2D1SolidColorBrush> accentBrush_;
    ComPtr<ID2D1SolidColorBrush> redBrush_;
    ComPtr<ID2D1SolidColorBrush> textBrush_;
    ComPtr<ID2D1SolidColorBrush> mutedBrush_;
    ComPtr<ID2D1SolidColorBrush> tintBrush_;
    ComPtr<ID2D1SolidColorBrush> shadowBrush_;
    ComPtr<ID2D1Bitmap> artBitmap_;
    ComPtr<ID2D1Bitmap> notificationIconBitmap_;
    ComPtr<ID2D1Bitmap> mediaSourceIconBitmap_;
    ComPtr<ID2D1Bitmap> clipboardIconBitmap_;
    ComPtr<ID2D1Bitmap> airpodsBitmap_;          // cache for airpods.png
    uint64_t artGeneration_ = 0;
    uint64_t notificationIconGeneration_ = 0;
    uint64_t mediaSourceIconGeneration_ = 0;
    uint64_t clipboardIconGeneration_ = 0;
    uint64_t airpodsBitmapGen_ = 0;
    BitmapPixels airpodsPngPixels_;              // decoded pixels for airpods.png
    bool airpodsPngLoaded_ = false;              // guard so we only attempt once
    float settingsOpacity_ = 0.96f;
    D2D1_COLOR_F pillBgColor_ = D2D1::ColorF(0.051f, 0.051f, 0.059f, 1.0f);
    D2D1_COLOR_F lerpedAccent_ = D2D1::ColorF(0.0f, 0.48f, 1.0f, 1.0f);  // Q3: smooth accent lerp state
};

