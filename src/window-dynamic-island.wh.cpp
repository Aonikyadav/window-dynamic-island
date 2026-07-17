// ==WindhawkMod==
// @id              window-dynamic-island
// @name            Window Dynamic Island
// @description     A premium Dynamic Island for Windows by Aonik with media, weather, clipboard, battery, notifications, and gaming overlays.
// @version         1.0.0
// @author          Zoro
// @github          https://github.com/Aonikyadav
// @include         windhawk.exe
// @compilerOptions -lole32 -loleaut32 -lshcore -ld2d1 -ldwrite -ldwmapi -lgdi32 -luser32 -lshell32 -lruntimeobject -lwindowscodecs -lavrt -lsetupapi -lwinhttp -lpdh
// @license         MIT
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Aonik Dynamic Island

A fluid, living overlay inspired by Apple's Dynamic Island, bringing a beautiful, highly-responsive UI to your Windows desktop. Built natively with hardware-accelerated Direct2D rendering for a buttery-smooth 60 FPS experience.

![Aonik Dynamic Island Preview](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/Full-preview.png)

---

## 🚀 Modules & Dashboards

The Dynamic Island intelligently expands to display context-aware dashboards. You can easily navigate between different views using your mouse scroll wheel.

| Module | Description | Preview |
| :--- | :--- | :--- |
| **Media Player** | Shows live album art, track details, audio waveforms, and full playback controls. | ![Media](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/media.png) |
| **Calendar** | A sleek, perfectly aligned monthly calendar that highlights the current date. | ![Calendar](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/calender.png) |
| **Weather** | Real-time weather stats powered by wttr.in, including wind speed, humidity, and "feels like" temperature. | ![Weather](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/weather.png) |
| **Game Overlay** | Real-time FPS, CPU, GPU, and RAM utilization overlays tailored for gamers. | ![Gamebar](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/gamebar.png) |
| **System Status** | Color-coded grid showing CPU, RAM, GPU, Battery, Volume, and Disk usage. | ![System Status](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/system-status.png) |
| **Battery Status** | Shows a fluid alert when your laptop battery is running low or is fully charged. | ![Battery](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/battery-status.png) |
| **Volume OSD** | A sleek on-screen display showing the current volume level whenever you increase or decrease system volume. | ![Volume](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/volume-inc-dec.png) |
| **Idle View** | A minimal dashboard with your battery status, digital clock, and sleek pagination dots. | ![Idle](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/idle.png) |
| **Camera Privacy** | Shows a green dot when an app is actively using your webcam. | ![Camera](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/camera-detected.png) |
| **Mic Privacy** | Shows an orange dot when an app is actively using your microphone. | ![Mic](https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/previews/mic-detected.png) |

---

## ✨ Core Features

- **Hardware Privacy Indicators:** A pulsing orange dot appears when your microphone is active, and a green dot when your camera is in use. Rate-limited polling ensures absolutely no CPU drain.
- **High-Res Clipboard & Notifications:** Instantly see what you copied or your latest Windows notifications, featuring crisp, high-fidelity 64px app icons extracted directly from system executables.
- **Dynamic Fluid Animations:** Fully smooth resizing and splitting when multiple events happen at once (e.g., media playing while you copy text or receive a notification).
- **Customizable Aesthetics:** Switch between sleek OLED Black, Dark Gray, Midnight Blue, and Deep Purple themes from the right-click menu, or use the settings to dial in your exact hex colors.

---

## ⚙️ Usage & Settings

- **Hover & Scroll:** Hover over the island to seamlessly expand it. Use your mouse scroll wheel to swipe between the Media, Calendar, and Weather tabs.
- **Right-Click Menu:** Right-click the island to access Theme presets, Transparency settings, and to pin the island open.
- **Windhawk Settings:** Visit the Mod Settings tab to change the island's Position, Size Scale, Animation Speed, and toggle specific modules. You can also perfectly align the island using the new `Offset X` and `Offset Y` settings, and even select exactly which monitor the island should appear on (including a brand new "Follow Mouse" mode!).
- **Notifications:** To use the notification module, you need to add `explorer.exe` to the process inclusion list in the Advanced tab of the mod settings and restart the mod.

---

## 📝 Feedback & Credits

### Support / Bug Reports
- Please use [window-dynamic-island issues](https://github.com/Aonikyadav/window-dynamic-island/issues) to report bugs, request features, or share feedback.  
- Clear descriptions, screenshots, or steps to reproduce help improve fixes and updates.  
- Suggestions for UI/UX or new integrations are always welcome.

### Credits
- **[Aonik Yadav @GitHub](https://github.com/Aonikyadav)**: Author and primary developer. Built and maintains the entire mod — UI, dashboards, system integrations, and all feature work.
- **AI Assistance**: Contributed Classic Pastel Green & Mint dashboards, auto-hide engine enhancements, and UV Index layout optimizations.

*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- Appearance:
  - Position: top-center
    $name: Position
    $description: Where the island should appear on your screen.
    $options:
      - top-center: Top Center
      - top-left: Top Left
      - top-right: Top Right
      - bottom-center: Bottom Center
  - TargetMonitor: primary
    $name: Target Monitor
    $description: Select the screen to display the island. If a display isn't found, it safely falls back to the Primary Monitor.(Extra Displays are given even if they don't exist because windhawk settings are static)
    $options:
      - 'primary': Primary Monitor
      - '1': Display 1
      - '2': Display 2
      - '3': Display 3
      - '4': Display 4
      - '5': Display 5
      - 'follow': Follow Mouse (Active Monitor)
  - OffsetX: 0
    $name: Offset X
    $description: Adjust the horizontal position (in pixels). Positive values move it right, negative values move it left.
  - OffsetY: 0
    $name: Offset Y
    $description: Adjust the vertical position (in pixels). Positive values move it down, negative values move it up.
  - SizeScale: '1.0'
    $name: Size scale
    $description: Makes the entire island and its contents larger or smaller.
    $options:
      - '0.8': 0.8x
      - '1.0': 1.0x
      - '1.2': 1.2x
      - '1.5': 1.5x
      - '1.8': 1.8x
      - '2.0': 2.0x
      - '2.5': 2.5x
  - AutoHideIdleSeconds: '0'
    $name: Auto-hide idle island
    $description: Hide the idle pill after this many seconds of inactivity. 0 to disable.
    $options:
      - '-1': Hide instantly
      - '0': Never hide (default)
      - '5': Hide after 5 seconds
      - '10': Hide after 10 seconds
      - '30': Hide after 30 seconds
      - '60': Hide after 60 seconds
  - UnhideOnHover: true
    $name: Unhide on hover
    $description: Allow the hidden island to reappear when you hover your mouse over it.
  - AutoDpiScale: true
    $name: Auto DPI scaling
    $description: Automatically scales the island to match your monitor's DPI. Recommended for 4K screens.
  - W11Style: false
    $name: Native Windows 11 style
    $description: Changes the shape from an Apple pill to a Windows 11 rounded box.
  - AlwaysOnTop: true
    $name: Always on top
    $description: Keeps the island above all other windows. Turn this off if it blocks other apps.
  - ExpandOnHover: true
    $name: Expand on hover
    $description: Expand the island automatically when hovered. If disabled, click to expand.
  - AnimationSpeed: normal
    $name: Animation speed
    $description: How fast the island expands and collapses.
    $options:
      - slow: Slow
      - normal: Normal
      - fast: Fast
  $name: Appearance & Behavior
- Themes:
  - AccentColorMode: auto
    $name: Accent color mode
    $description: How the glowing accent color is chosen. Auto extracts it from album art.
    $options:
      - auto: Auto, from album art
      - system: System
      - custom: Custom hex
  - CustomAccentHex: "#4cc9f0"
    $name: Custom accent hex
    $description: The hex color to use when the accent mode is set to Custom.
  - PillBgColor: "#0D0D0F"
    $name: Pill background color
    $description: Hex color for the background. Presets are available in the right-click menu.
  - TextPrimaryColor: "#F7F7F7"
    $name: Primary text color
    $description: Hex color for titles and main text.
  - TextSecondaryColor: "#888888"
    $name: Secondary text color
    $description: Hex color for artist names and muted labels.
  - TintIntensity: 72
    $name: Background tint intensity
    $description: 0 to 100. Controls how dark the background tint behind the island is.
  - PillOpacity: 96
    $name: Pill transparency
    $description: 35 to 100. Lower values make the island more see-through.
  $name: Colors & Theming
- Modules:
  - Media: true
    $name: Media module
    $description: Shows album art, song info, and playback controls when music is playing.
  - Clipboard: true
    $name: Clipboard module
    $description: Shows a quick preview of the text or images you just copied.
  - Battery: true
    $name: Battery module
    $description: Shows an alert when your laptop battery is running low.
  - Progress: true
    $name: Progress module
    $description: Shows a progress ring around the island for downloads or file copies.
  - GameOverlay: false
    $name: Enable game overlay mode
    $description: Replaces the clock with live stats like FPS, CPU, and RAM usage.
  - ShowMetricText: false
    $name: Show labels in metric chips
    $description: Adds text labels (like "CPU") inside the game overlay bars.
  - WeatherCity: ""
    $name: Weather City (Optional)
    $description: Enter your city (e.g. London). Leave blank to use auto IP geolocation.
  - WeatherFahrenheit: false
    $name: Use Fahrenheit
    $description: Display weather temperature and wind speed in imperial units.
  $name: Modules & Features
*/
// ==/WindhawkModSettings==

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <unknwn.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <setupapi.h>
#include <dbt.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <shcore.h>
#include <windowsx.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <mmreg.h>
#include <objbase.h>
#include <wrl/client.h>
#include <uiautomation.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#include <pdh.h>
#include <pdhmsg.h>
#pragma comment(lib, "pdh.lib")

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cwchar>
#include <cstring>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#if __has_include(<winrt/Windows.UI.Notifications.Management.h>) && \
    __has_include(<winrt/Windows.UI.Notifications.h>)
#define DYNAMIC_ISLAND_HAS_USER_NOTIFICATION_LISTENER 1
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.UI.Notifications.Management.h>
#else
#define DYNAMIC_ISLAND_HAS_USER_NOTIFICATION_LISTENER 0
#endif

using Microsoft::WRL::ComPtr;
using namespace std::chrono_literals;

#ifdef STANDALONE_APP
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "shcore.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "avrt.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "advapi32.lib")

void OpenSettingsDialog(HWND parent);
void SetStartupRegistry(bool enable);
bool IsStartupRegistryEnabled();

// Helper to get absolute path to settings.ini next to the executable
std::wstring GetIniPath() {
    wchar_t path[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    wchar_t* lastSlash = wcsrchr(path, L'\\');
    if (lastSlash) {
        *(lastSlash + 1) = L'\0';
        wcscat_s(path, L"settings.ini");
    } else {
        wcscpy_s(path, L"settings.ini");
    }
    return path;
}

// Windhawk API stubs for standalone mode
extern "C" {
    void Wh_Log(const wchar_t* format, ...) {
        va_list args;
        va_start(args, format);
        wchar_t buffer[2048];
        vswprintf_s(buffer, format, args);
        va_end(args);
        OutputDebugStringW(L"[DynamicIsland] ");
        OutputDebugStringW(buffer);
        OutputDebugStringW(L"\n");

        wchar_t logPath[MAX_PATH] = {};
        GetModuleFileNameW(nullptr, logPath, MAX_PATH);
        wchar_t* lastSlash = wcsrchr(logPath, L'\\');
        if (lastSlash) {
            *(lastSlash + 1) = L'\0';
            wcscat_s(logPath, L"debug.log");
        } else {
            wcscpy_s(logPath, L"debug.log");
        }

        FILE* f = nullptr;
        if (_wfopen_s(&f, logPath, L"a, ccs=UTF-8") == 0) {
            fwprintf(f, L"[DynamicIsland] %ls\n", buffer);
            fclose(f);
        }
    }

    PCWSTR Wh_GetStringSetting(PCWSTR name) {
        wchar_t* buffer = new wchar_t[512];
        std::wstring def = L"";
        if (wcscmp(name, L"Appearance.Position") == 0) def = L"top-center";
        else if (wcscmp(name, L"Appearance.SizeScale") == 0) def = L"1.0";
        else if (wcscmp(name, L"Themes.AccentColorMode") == 0) def = L"auto";
        else if (wcscmp(name, L"Themes.CustomAccentHex") == 0) def = L"#4cc9f0";
        else if (wcscmp(name, L"Appearance.AnimationSpeed") == 0) def = L"normal";
        else if (wcscmp(name, L"Modules.WeatherCity") == 0) def = L"";
        else if (wcscmp(name, L"Appearance.AutoHideIdleSeconds") == 0) def = L"0";
        else if (wcscmp(name, L"Appearance.TargetMonitor") == 0) def = L"primary";
        else if (wcscmp(name, L"Themes.PillBgColor") == 0) def = L"#0D0D0F";
        else if (wcscmp(name, L"Themes.TextPrimaryColor") == 0) def = L"#F7F7F7";
        else if (wcscmp(name, L"Themes.TextSecondaryColor") == 0) def = L"#888888";

        GetPrivateProfileStringW(L"Settings", name, def.c_str(), buffer, 512, GetIniPath().c_str());
        return buffer;
    }

    void Wh_FreeStringSetting(PCWSTR value) {
        delete[] value;
    }

    int Wh_GetIntSetting(PCWSTR name) {
        int def = 1;
        if (wcscmp(name, L"Appearance.AutoDpiScale") == 0) def = 1;
        else if (wcscmp(name, L"Modules.Media") == 0) def = 1;
        else if (wcscmp(name, L"Modules.Clipboard") == 0) def = 1;
        else if (wcscmp(name, L"Modules.Battery") == 0) def = 1;
        else if (wcscmp(name, L"Modules.Progress") == 0) def = 1;
        else if (wcscmp(name, L"Themes.TintIntensity") == 0) def = 72;
        else if (wcscmp(name, L"Themes.PillOpacity") == 0) def = 96;
        else if (wcscmp(name, L"Modules.GameOverlay") == 0) def = 0;
        else if (wcscmp(name, L"Modules.ShowMetricText") == 0) def = 0;
        else if (wcscmp(name, L"Modules.WeatherFahrenheit") == 0) def = 0;
        else if (wcscmp(name, L"Appearance.UnhideOnHover") == 0) def = 1;
        else if (wcscmp(name, L"Appearance.AlwaysOnTop") == 0) def = 1;
        else if (wcscmp(name, L"Appearance.ExpandOnHover") == 0) def = 1;
        else if (wcscmp(name, L"Appearance.OffsetX") == 0) def = 0;
        else if (wcscmp(name, L"Appearance.OffsetY") == 0) def = 0;
        else if (wcscmp(name, L"Appearance.W11Style") == 0) def = 0;
        else if (wcscmp(name, L"Themes.ColorfulModules") == 0) def = 0;
        else if (wcscmp(name, L"Appearance.ExpandOnTrackChange") == 0) def = 1;
        else if (wcscmp(name, L"Appearance.StartWithWindows") == 0) def = 0;
        else if (wcscmp(name, L"Appearance.SplitMode") == 0) def = 1;

        return GetPrivateProfileIntW(L"Settings", name, def, GetIniPath().c_str());
    }

    int Wh_GetIntValue(PCWSTR name, int fallback) {
        return GetPrivateProfileIntW(L"State", name, fallback, GetIniPath().c_str());
    }

    void Wh_SetIntValue(PCWSTR name, int value) {
        wchar_t valStr[32];
        swprintf_s(valStr, L"%d", value);
        WritePrivateProfileStringW(L"State", name, valStr, GetIniPath().c_str());
        
        void WhTool_ModSettingsChanged();
        WhTool_ModSettingsChanged();
    }
}
#endif

HWND g_settingsHwnd = nullptr;
HFONT g_hFont = nullptr;

namespace {

constexpr wchar_t kWindowClass[] = L"Windhawk.DynamicIslandForWindows";
constexpr UINT WM_APP_LAYOUT_CHANGED = WM_APP + 0x442;
constexpr UINT WM_APP_NEW_EVENT = WM_APP + 0x443;
constexpr float kRenderPadX = 28.0f;
constexpr float kRenderPadY = 22.0f;

enum class IslandKind {
    Idle,
    Media,
    Progress,
    Clipboard,
    Notification,
    Volume,
    BatteryLow,
    CapsLock,
    Device,
    Split,
    Timer,
};

enum class Position {
    TopCenter,
    TopLeft,
    TopRight,
    BottomCenter,
};

enum class AccentMode {
    Auto,
    System,
    Custom,
};

enum class AirPodsMode {
    Both = 0,
    SmallOnly = 1,
    BiggerOnly = 2
};

struct Settings {
    Position position = Position::TopCenter;
    int targetMonitor = 0;
    int offsetX = 0;
    int offsetY = 0;
    float sizeScale = 1.0f;
    AccentMode accentMode = AccentMode::Auto;
    D2D1_COLOR_F customAccent = D2D1::ColorF(0x4cc9f0);
    float animationSpeed = 1.0f;
    bool media = true;
    bool clipboard = true;
    bool battery = true;
    bool progress = true;
    float tintOpacity = 0.72f;
    float pillOpacity = 0.96f;
    bool gameOverlay = false;
    bool showMetricText = true;
    std::wstring weatherCity;
    bool weatherFahrenheit = false;
    int autoHideIdleSeconds = 0;
    bool unhideOnHover = true;
    bool alwaysOnTop = true;
    bool expandOnHover = true;
    bool autoDpiScale = true;
    bool w11Style = false;
    bool colorfulModules = false;
    bool expandOnTrackChange = true;
    bool startWithWindows = false;
    bool splitMode = true;
    AirPodsMode airpodsMode = AirPodsMode::Both;
    // Color customization
    D2D1_COLOR_F pillBgColor = D2D1::ColorF(0.051f, 0.051f, 0.059f, 1.0f); // #0D0D0F
    D2D1_COLOR_F textPrimaryColor = D2D1::ColorF(0.969f, 0.969f, 0.969f, 1.0f); // #F7F7F7
    D2D1_COLOR_F textSecondaryColor = D2D1::ColorF(0.533f, 0.533f, 0.533f, 1.0f); // #888888
    int colorTheme = 0;
};

struct BitmapPixels {
    std::vector<uint8_t> bgra;
    UINT width = 0;
    UINT height = 0;
    uint64_t generation = 0;
    D2D1_COLOR_F sampledAccent = D2D1::ColorF(0x4cc9f0);
};

struct MediaSnapshot {
    bool available = false;
    bool playing = false;
    std::wstring title;
    std::wstring artist;
    std::wstring sourceAppUserModelId;
    std::wstring sourceName;
    std::wstring sourceBadge;
    BitmapPixels art;
    BitmapPixels sourceIcon;
    uint64_t artGeneration = 0;
    uint64_t sourceIconGeneration = 0;
    double artChangedAt = 0.0;
    int64_t positionTicks = 0;
    int64_t endTicks = 0;
    int64_t lastUpdatedTicks = 0;
};

struct ClipboardSnapshot {
    bool active = false;
    bool image = false;
    std::wstring text;
    std::wstring appName;
    BitmapPixels appIcon;
    double expiresAt = 0.0;
};

struct BatterySnapshot {
    bool active = false;
    bool low = false;
    bool charging = false;
    int percent = 100;
    DWORD secondsRemaining = BATTERY_LIFE_UNKNOWN;
    double expiresAt = 0.0;
};

struct ProgressSnapshot {
    bool active = false;
    int percent = 0;
};

struct NotificationSnapshot {
    bool active = false;
    std::wstring app;
    std::wstring title;
    std::wstring body;
    BitmapPixels icon;
    double expiresAt = 0.0;
};

struct VolumeSnapshot {
    bool active = false;
    int percent = 0;
    bool muted = false;
    std::wstring deviceName;
    double expiresAt = 0.0;
};

struct CapsLockSnapshot {
    bool active = false;
    bool capsOn = false;
    bool numOn = false;
    bool isNumEvent = false;
    double expiresAt = 0.0;
};

enum class DeviceEventType {
    Connected,
    Disconnected,
};

struct DeviceSnapshot {
    bool active = false;
    DeviceEventType eventType = DeviceEventType::Connected;
    std::wstring deviceName;  // e.g. "USB Drive" or "Bluetooth Device"
    bool isBluetoothLike = false;
    double expiresAt = 0.0;
};

struct SystemSnapshot {
    int volumePercent = 0;
    bool volumeMuted = false;
    int cpuPercent = 0;
    int memoryPercent = 0;
    int diskFreePercent = 0;
    int renderFps = 0;
    int gpuPercent = -1;
    bool charging = false;
    bool micActive = false;      // orange dot: microphone in use
    bool cameraActive = false;   // green dot: camera in use
    std::wstring foregroundTitle;
    bool internetConnected = false;
    bool bluetoothOn = false;
};

struct Activity {
    IslandKind kind = IslandKind::Idle;
    float width = 120.0f;
    float height = 36.0f;
};

struct WeatherSnapshot {
    bool hasData = false;
    float temperature = 0.0f;
    int weatherCode = 0;
    std::wstring city;
    std::wstring weatherDesc;
    std::wstring windSpeed;
    std::wstring windDir;
    std::wstring humidity;
    std::wstring feelsLike;
    std::wstring uvIndex;
    std::wstring tempMax;
    std::wstring tempMin;
    double lastUpdated = 0.0;
};

struct SharedState {
    MediaSnapshot media;
    ClipboardSnapshot clipboard;
    NotificationSnapshot notification;
    VolumeSnapshot volume;
    CapsLockSnapshot capsLock;
    DeviceSnapshot device;
    BatterySnapshot battery;
    ProgressSnapshot progress;
    SystemSnapshot system;
    WeatherSnapshot weather;
    std::array<float, 48> waveform{};
    size_t waveformWrite = 0;
    bool muted = false;
};

struct SpringValue {
    float value = 0.0f;
    float velocity = 0.0f;
    float target = 0.0f;

    void Reset(float v) {
        value = target = v;
        velocity = 0.0f;
    }

    void Step(float dt, float stiffness, float damping) {
        const float displacement = value - target;
        const float acceleration = -stiffness * displacement - damping * velocity;
        velocity += acceleration * dt;
        value += velocity * dt;

        if (std::fabs(value - target) < 0.01f && std::fabs(velocity) < 0.01f) {
            value = target;
            velocity = 0.0f;
        }
    }
};

Settings g_settings;
std::mutex g_stateMutex;
SharedState g_state;
std::atomic<uint64_t> g_artGenerationCounter = 0;

HWND g_hwnd = nullptr;
HANDLE g_stopEvent = nullptr;
HANDLE g_settingsChangedEvent = nullptr;
HANDLE g_renderThread = nullptr;
HANDLE g_mediaThread = nullptr;
HANDLE g_audioThread = nullptr;
HANDLE g_weatherThread = nullptr;
HANDLE g_notificationThread = nullptr;
std::atomic<bool> g_running = false;
std::atomic<int> g_idleTab = 0;
std::atomic<bool> g_layoutDirty = true;
std::atomic<bool> g_clickExpanded = false;
std::atomic<int> g_pressedMediaButton = -1;
FILETIME g_prevIdleTime = {};
FILETIME g_prevKernelTime = {};
FILETIME g_prevUserTime = {};
UINT g_shellHookMessage = 0;
bool g_volumeInitialized = false;
std::atomic<double> g_lastNudgeTime = 0.0;
double g_deviceConnectedAt = -999.0;  // timestamp of last BT device connect (to suppress spurious volume change)
double g_suppressVolumeUntil = 0.0;   // timestamp until which volume changes are suppressed
std::wstring g_lastBtName;            // name of the last active Bluetooth device

constexpr GUID kSubTypeIeeeFloat = {
    0x00000003,
    0x0000,
    0x0010,
    {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
};



double NowSeconds() {
    using clock = std::chrono::steady_clock;
    static const auto start = clock::now();
    return std::chrono::duration<double>(clock::now() - start).count();
}

float Clamp(float v, float lo, float hi) {
    return std::max(lo, std::min(hi, v));
}

int ClampInt(int v, int lo, int hi) {
    return std::max(lo, std::min(hi, v));
}

bool EqualsNoCase(std::wstring_view a, std::wstring_view b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (towlower(a[i]) != towlower(b[i])) {
            return false;
        }
    }

    return true;
}

std::wstring GetStringSettingCopy(PCWSTR name) {
    PCWSTR value = Wh_GetStringSetting(name);
    std::wstring result = value ? value : L"";
    Wh_FreeStringSetting(value);
    return result;
}

D2D1_COLOR_F ColorFromHex(std::wstring text, D2D1_COLOR_F fallback) {
    if (!text.empty() && text[0] == L'#') {
        text.erase(text.begin());
    }

    if (text.size() != 6) {
        return fallback;
    }

    wchar_t* end = nullptr;
    const unsigned long rgb = wcstoul(text.c_str(), &end, 16);
    if (!end || *end != L'\0') {
        return fallback;
    }

    return D2D1::ColorF(
        ((rgb >> 16) & 0xff) / 255.0f,
        ((rgb >> 8) & 0xff) / 255.0f,
        (rgb & 0xff) / 255.0f,
        1.0f);
}

// Returns the DPI scale factor for the primary monitor (1.0 = 96 DPI = 100%)
float GetPrimaryMonitorDpiScale() {
    POINT pt = {0, 0};
    HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    UINT dpiX = 96, dpiY = 96;
    using GetDpiForMonitor_t = HRESULT(WINAPI*)(HMONITOR, int, UINT*, UINT*);
    static auto pGetDpiForMonitor = reinterpret_cast<GetDpiForMonitor_t>(
        GetProcAddress(GetModuleHandleW(L"shcore.dll"), "GetDpiForMonitor"));
    if (pGetDpiForMonitor) {
        pGetDpiForMonitor(monitor, 0 /* MDT_EFFECTIVE_DPI */, &dpiX, &dpiY);
    }
    return static_cast<float>(dpiX) / 96.0f;
}

D2D1_COLOR_F GetSystemAccentColor() {
    DWORD color = 0;
    BOOL opaque = FALSE;
    using DwmGetColorizationColor_t = HRESULT(WINAPI*)(DWORD*, BOOL*);
    auto proc = reinterpret_cast<DwmGetColorizationColor_t>(
        GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmGetColorizationColor"));

    if (proc && SUCCEEDED(proc(&color, &opaque))) {
        return D2D1::ColorF(
            ((color >> 16) & 0xff) / 255.0f,
            ((color >> 8) & 0xff) / 255.0f,
            (color & 0xff) / 255.0f,
            1.0f);
    }

    return D2D1::ColorF(0x4cc9f0);
}

void LoadToolsList();
void SaveToolsList();
void UpdateWindowBlur(HWND hwnd, int themeIndex);

void LoadSettings() {
    LoadToolsList();
    Settings next;

    const std::wstring position = GetStringSettingCopy(L"Appearance.Position");
    if (EqualsNoCase(position, L"top-left")) {
        next.position = Position::TopLeft;
    } else if (EqualsNoCase(position, L"top-right")) {
        next.position = Position::TopRight;
    } else if (EqualsNoCase(position, L"bottom-center")) {
        next.position = Position::BottomCenter;
    }

    const std::wstring scale = GetStringSettingCopy(L"Appearance.SizeScale");
    if (!scale.empty()) {
        wchar_t* end;
        float parsedScale = wcstof(scale.c_str(), &end);
        if (end != scale.c_str() && parsedScale > 0.1f && parsedScale < 10.0f) {
            next.sizeScale = parsedScale;
        }
    }

    // Auto DPI scaling: multiply sizeScale by monitor DPI factor.
    // On a 4K 200% display this doubles the island to the right physical size.
    if (Wh_GetIntSetting(L"Appearance.AutoDpiScale") != 0) {
        next.sizeScale *= GetPrimaryMonitorDpiScale();
    }

    const std::wstring accentMode = GetStringSettingCopy(L"Themes.AccentColorMode");
    if (EqualsNoCase(accentMode, L"system")) {
        next.accentMode = AccentMode::System;
    } else if (EqualsNoCase(accentMode, L"custom")) {
        next.accentMode = AccentMode::Custom;
    }

    next.customAccent = ColorFromHex(GetStringSettingCopy(L"Themes.CustomAccentHex"), next.customAccent);

    const std::wstring speed = GetStringSettingCopy(L"Appearance.AnimationSpeed");
    if (EqualsNoCase(speed, L"slow")) {
        next.animationSpeed = 0.75f;
    } else if (EqualsNoCase(speed, L"fast")) {
        next.animationSpeed = 1.35f;
    }

    next.media = Wh_GetIntSetting(L"Modules.Media") != 0;
    next.clipboard = Wh_GetIntSetting(L"Modules.Clipboard") != 0;
    next.battery = Wh_GetIntSetting(L"Modules.Battery") != 0;
    next.progress = Wh_GetIntSetting(L"Modules.Progress") != 0;
    next.tintOpacity = Clamp(Wh_GetIntSetting(L"Themes.TintIntensity") / 100.0f, 0.0f, 1.0f);
    const int settingOpacity = Wh_GetIntSetting(L"Themes.PillOpacity");
    const int localOpacity = Wh_GetIntValue(L"PillOpacityOverride", -1);
    next.pillOpacity = Clamp((localOpacity >= 0 ? localOpacity : settingOpacity) / 100.0f,
                             0.35f, 1.0f);
    next.gameOverlay = Wh_GetIntSetting(L"Modules.GameOverlay") != 0;
    next.showMetricText = Wh_GetIntSetting(L"Modules.ShowMetricText") != 0;
    next.weatherCity = GetStringSettingCopy(L"Modules.WeatherCity");
    next.weatherFahrenheit = Wh_GetIntSetting(L"Modules.WeatherFahrenheit") != 0;
    const std::wstring hideSec = GetStringSettingCopy(L"Appearance.AutoHideIdleSeconds");
    next.autoHideIdleSeconds = hideSec.empty() ? 0 : _wtoi(hideSec.c_str());
    next.unhideOnHover = Wh_GetIntSetting(L"Appearance.UnhideOnHover") != 0;
    next.alwaysOnTop = Wh_GetIntSetting(L"Appearance.AlwaysOnTop") != 0;
    const int localExpandOnHover = Wh_GetIntValue(L"ExpandOnHoverOverride", -1);
    next.expandOnHover = localExpandOnHover >= 0 ? (localExpandOnHover != 0) : (Wh_GetIntSetting(L"Appearance.ExpandOnHover") != 0);
    next.autoDpiScale = Wh_GetIntSetting(L"Appearance.AutoDpiScale") != 0;
    next.offsetX = Wh_GetIntSetting(L"Appearance.OffsetX");
    next.offsetY = Wh_GetIntSetting(L"Appearance.OffsetY");
    next.colorfulModules = Wh_GetIntSetting(L"Themes.ColorfulModules") != 0;
    next.expandOnTrackChange = Wh_GetIntSetting(L"Appearance.ExpandOnTrackChange") != 0;
    next.splitMode = Wh_GetIntSetting(L"Appearance.SplitMode") != 0;
    next.startWithWindows = Wh_GetIntSetting(L"Appearance.StartWithWindows") != 0;
    if (next.startWithWindows != IsStartupRegistryEnabled()) {
        SetStartupRegistry(next.startWithWindows);
    }

    std::wstring mon = GetStringSettingCopy(L"Appearance.TargetMonitor");
    if (mon == L"primary") next.targetMonitor = 0;
    else if (mon == L"follow") next.targetMonitor = -1;
    else next.targetMonitor = _wtoi(mon.c_str());

    const std::wstring apMode = GetStringSettingCopy(L"Modules.AirPodsMode");
    if (EqualsNoCase(apMode, L"small")) {
        next.airpodsMode = AirPodsMode::SmallOnly;
    } else if (EqualsNoCase(apMode, L"bigger")) {
        next.airpodsMode = AirPodsMode::BiggerOnly;
    } else {
        next.airpodsMode = AirPodsMode::Both;
    }

    const int localW11Style = Wh_GetIntValue(L"W11StyleOverride", -1);
    next.w11Style = localW11Style >= 0 ? (localW11Style != 0) : (Wh_GetIntSetting(L"Appearance.W11Style") != 0);

    // Color settings — check local theme override first, then settings YAML.
    struct ThemeColors { const wchar_t* bg; const wchar_t* fg; const wchar_t* sec; };
    static constexpr ThemeColors kThemes[] = {
        {L"#0D0D0F", L"#F7F7F7", L"#888888"},  // 0: Apple Style (translucent notch black)
        {L"#000000", L"#FFFFFF", L"#888888"},  // 1: OLED Black (pure solid black)
        {L"#1C1C1E", L"#FFFFFF", L"#999999"},  // 2: Dark Gray
        {L"#050D1A", L"#E8F0FF", L"#7090BB"},  // 3: Midnight Blue
        {L"#0E0014", L"#F0E8FF", L"#9060BB"},  // 4: Deep Purple
        {L"#1F1F1F", L"#FFFFFF", L"#A0A0A0"},  // 5: Fluent Design
        {L"#050505", L"#FFFFFF", L"#888888"},  // 6: Nothing OS
        {L"#1A1A1A", L"#FFFFFF", L"#B0B0B0"},  // 7: Glassmorphism
        {L"#030303", L"#00FFFF", L"#FF00FF"},  // 8: Cyberpunk
        {L"#181B1F", L"#FFD8E4", L"#E4C0CE"},  // 9: Material You
        {L"#000000", L"#FFFFFF", L"#777777"},  // 10: AMOLED Dark
        {L"#020813", L"#00E5FF", L"#006688"},  // 11: Holographic AI
    };
    const int theme = Wh_GetIntValue(L"ColorTheme", -1);
    next.colorTheme = theme;
    if (theme >= 0 && theme < static_cast<int>(ARRAYSIZE(kThemes))) {
        next.pillBgColor = ColorFromHex(kThemes[theme].bg,
                                        D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
        next.textPrimaryColor = ColorFromHex(kThemes[theme].fg,
                                             D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
        next.textSecondaryColor = ColorFromHex(kThemes[theme].sec,
                                               D2D1::ColorF(0.6f, 0.6f, 0.6f, 1.0f));
    } else {
        next.pillBgColor = ColorFromHex(GetStringSettingCopy(L"Themes.PillBgColor"),
                                        D2D1::ColorF(0.051f, 0.051f, 0.059f, 1.0f));
        next.textPrimaryColor = ColorFromHex(GetStringSettingCopy(L"Themes.TextPrimaryColor"),
                                             D2D1::ColorF(0.969f, 0.969f, 0.969f, 1.0f));
        next.textSecondaryColor = ColorFromHex(GetStringSettingCopy(L"Themes.TextSecondaryColor"),
                                               D2D1::ColorF(0.533f, 0.533f, 0.533f, 1.0f));
    }

    bool cityChanged = next.weatherCity != g_settings.weatherCity;
    g_settings = next;
    g_layoutDirty = true;
    if (g_hwnd) {
        UpdateWindowBlur(g_hwnd, next.colorTheme);
    }
    if (cityChanged && g_settingsChangedEvent) {
        SetEvent(g_settingsChangedEvent);
    }
}

void UpdateWindowBlur(HWND hwnd, int themeIndex) {
    // 1. Windows 11 Acrylic Backdrop (DWMWA_SYSTEMBACKDROP_TYPE = 38)
    // 3 = DWMSBT_TRANSIENTWINDOW (Acrylic backdrop), 1 = DWMSBT_NONE (Disable)
    DWORD backdrop = (themeIndex == 7) ? 3 : 1;
    DwmSetWindowAttribute(hwnd, 38 /* DWMWA_SYSTEMBACKDROP_TYPE */, &backdrop, sizeof(backdrop));

    // 2. Windows 10 / Windows 11 SetWindowCompositionAttribute (Frosted Glass Acrylic)
    typedef enum _WINDOWCOMPOSITIONATTRIB {
        WCA_ACCENT_POLICY = 19
    } WINDOWCOMPOSITIONATTRIB;
    typedef enum _ACCENT_STATE {
        ACCENT_DISABLED = 0,
        ACCENT_ENABLE_GRADIENT = 1,
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        ACCENT_ENABLE_BLURBEHIND = 3,
        ACCENT_ENABLE_ACRYLICBLURBEHIND = 4
    } ACCENT_STATE;
    typedef struct _ACCENT_POLICY {
        ACCENT_STATE AccentState;
        DWORD AccentFlags;
        DWORD GradientColor;
        DWORD AnimationId;
    } ACCENT_POLICY;
    typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
        WINDOWCOMPOSITIONATTRIB Attrib;
        PVOID pvData;
        SIZE_T cbData;
    } WINDOWCOMPOSITIONATTRIBDATA;
    typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

    HMODULE hUser = GetModuleHandleW(L"user32.dll");
    if (hUser) {
        auto setWindowCompositionAttribute = reinterpret_cast<pfnSetWindowCompositionAttribute>(
            GetProcAddress(hUser, "SetWindowCompositionAttribute"));
        if (setWindowCompositionAttribute) {
            ACCENT_POLICY accent = { ACCENT_DISABLED, 0, 0, 0 };
            if (themeIndex == 7) { // Glassmorphism is index 7
                accent.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
                accent.GradientColor = 0x011A1A1A;
            }
            WINDOWCOMPOSITIONATTRIBDATA data;
            data.Attrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(hwnd, &data);
        }
    }

    // 3. Legacy fallback DWM blur behind
    DWM_BLURBEHIND blur = {};
    blur.dwFlags = DWM_BB_ENABLE;
    blur.fEnable = (themeIndex == 7) ? TRUE : FALSE;
    DwmEnableBlurBehindWindow(hwnd, &blur);
}



struct MonitorEnumData {
    std::vector<HMONITOR> monitors;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData) {
    auto* data = reinterpret_cast<MonitorEnumData*>(dwData);
    data->monitors.push_back(hMonitor);
    return TRUE;
}

RECT GetAnchorWorkRect() {
    HMONITOR selectedMonitor = nullptr;

    if (g_settings.targetMonitor == -1) {
        POINT pt = {0, 0};
        GetCursorPos(&pt);
        selectedMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    } else if (g_settings.targetMonitor > 0) {
        MonitorEnumData data;
        EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&data));
        
        int index = g_settings.targetMonitor - 1;
        if (index >= 0 && index < static_cast<int>(data.monitors.size())) {
            selectedMonitor = data.monitors[index];
        }
    }

    if (!selectedMonitor) {
        POINT pt = {0, 0};
        selectedMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    }

    MONITORINFO mi = {sizeof(mi)};
    GetMonitorInfoW(selectedMonitor, &mi);
    return mi.rcWork;
}

void PositionOverlayWindow(HWND hwnd, int width, int height) {
    RECT work = GetAnchorWorkRect();
    int x = work.left + (work.right - work.left - width) / 2;
    int y = work.top + 8;

    switch (g_settings.position) {
        case Position::TopLeft:
            x = work.left + 16;
            y = work.top + 8;
            break;
        case Position::TopRight:
            x = work.right - width - 16;
            y = work.top + 8;
            break;
        case Position::BottomCenter:
            x = work.left + (work.right - work.left - width) / 2;
            y = work.bottom - height - 40;
            break;
        case Position::TopCenter:
        default:
            break;
    }

    HWND zOrder = g_settings.alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST;
    
    // Manage owner window to firmly anchor to desktop when alwaysOnTop is false
    if (g_settings.alwaysOnTop) {
        SetWindowLongPtr(hwnd, GWLP_HWNDPARENT, 0);
    } else {
        HWND hProgman = FindWindowW(L"Progman", nullptr);
        if (hProgman) {
            SetWindowLongPtr(hwnd, GWLP_HWNDPARENT, reinterpret_cast<LONG_PTR>(hProgman));
        }
    }

    x += g_settings.offsetX;
    y += g_settings.offsetY;
    SetWindowPos(hwnd, zOrder, x, y, width, height,
                 SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_SHOWWINDOW);
}

bool DecodeImageBytesToPixels(const std::vector<uint8_t>& bytes, BitmapPixels* outPixels) {
    if (!outPixels || bytes.empty()) {
        return false;
    }

    ComPtr<IWICImagingFactory> factory;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        return false;
    }

    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, bytes.size());
    if (!mem) {
        return false;
    }

    void* locked = GlobalLock(mem);
    memcpy(locked, bytes.data(), bytes.size());
    GlobalUnlock(mem);

    ComPtr<IStream> stream;
    hr = CreateStreamOnHGlobal(mem, TRUE, &stream);
    if (FAILED(hr)) {
        GlobalFree(mem);
        return false;
    }

    ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad,
                                          &decoder);
    if (FAILED(hr)) {
        return false;
    }

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) {
        return false;
    }

    ComPtr<IWICFormatConverter> converter;
    hr = factory->CreateFormatConverter(&converter);
    if (FAILED(hr)) {
        return false;
    }

    hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA,
                               WICBitmapDitherTypeNone, nullptr, 0.0,
                               WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) {
        return false;
    }

    UINT width = 0;
    UINT height = 0;
    converter->GetSize(&width, &height);
    if (!width || !height || width > 2048 || height > 2048) {
        return false;
    }

    BitmapPixels pixels;
    pixels.width = width;
    pixels.height = height;
    pixels.bgra.resize(static_cast<size_t>(width) * height * 4);

    hr = converter->CopyPixels(nullptr, width * 4,
                               static_cast<UINT>(pixels.bgra.size()),
                               pixels.bgra.data());
    if (FAILED(hr)) {
        return false;
    }

    struct Bucket {
        uint32_t count = 0;
        uint32_t r = 0;
        uint32_t g = 0;
        uint32_t b = 0;
    };

    std::array<Bucket, 16 * 16 * 16> buckets{};
    for (size_t i = 0; i + 3 < pixels.bgra.size(); i += 4) {
        const uint8_t alpha = pixels.bgra[i + 3];
        const uint8_t blue = pixels.bgra[i + 0];
        const uint8_t green = pixels.bgra[i + 1];
        const uint8_t red = pixels.bgra[i + 2];
        if (alpha < 32) {
            continue;
        }

        const int maxc = std::max({red, green, blue});
        const int minc = std::min({red, green, blue});
        const int luminance = (54 * red + 183 * green + 19 * blue) / 256;
        const int saturation = maxc - minc;
        if (luminance < 28 || luminance > 232 || saturation < 24) {
            continue;
        }

        const size_t bucketIndex = ((red >> 4) << 8) | ((green >> 4) << 4) | (blue >> 4);
        Bucket& bucket = buckets[bucketIndex];
        const uint32_t weight = 1 + static_cast<uint32_t>(saturation / 48);
        bucket.count += weight;
        bucket.r += red * weight;
        bucket.g += green * weight;
        bucket.b += blue * weight;
    }

    const Bucket* best = nullptr;
    for (const Bucket& bucket : buckets) {
        if (!best || bucket.count > best->count) {
            best = &bucket;
        }
    }

    if (best && best->count > 0) {
        pixels.sampledAccent = D2D1::ColorF(
            Clamp((best->r / static_cast<float>(best->count)) / 255.0f, 0.14f, 1.0f),
            Clamp((best->g / static_cast<float>(best->count)) / 255.0f, 0.14f, 1.0f),
            Clamp((best->b / static_cast<float>(best->count)) / 255.0f, 0.14f, 1.0f),
            1.0f);
    }

    pixels.generation = ++g_artGenerationCounter;
    *outPixels = std::move(pixels);
    return true;
}

bool IconToPixels(HICON icon, UINT size, BitmapPixels* outPixels) {
    if (!icon || !outPixels || !size) {
        return false;
    }

    HDC screen = GetDC(nullptr);
    HDC dc = CreateCompatibleDC(screen);
    ReleaseDC(nullptr, screen);
    if (!dc) {
        return false;
    }

    BITMAPINFO bi = {};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = static_cast<LONG>(size);
    bi.bmiHeader.biHeight = -static_cast<LONG>(size);
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    HBITMAP bitmap = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!bitmap) {
        DeleteDC(dc);
        return false;
    }

    // Fill with transparent black (alpha=0) so icon edges don't bleed dark fringe.
    HGDIOBJ old = SelectObject(dc, bitmap);
    RECT fill = {0, 0, static_cast<LONG>(size), static_cast<LONG>(size)};
    // Use NULL_BRUSH (transparent) then manually zero-fill the BGRA buffer after copy.
    FillRect(dc, &fill, reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));
    // Zero out the bits so background is fully transparent before drawing icon.
    ZeroMemory(bits, static_cast<size_t>(size) * size * 4);
    DrawIconEx(dc, 0, 0, icon, size, size, 0, nullptr, DI_NORMAL);

    BitmapPixels pixels;
    pixels.width = size;
    pixels.height = size;
    pixels.bgra.resize(static_cast<size_t>(size) * size * 4);
    memcpy(pixels.bgra.data(), bits, pixels.bgra.size());

    // Older icons can have no alpha in the color bitmap. Treat black pixels as
    // transparent only when the icon did not write any alpha at all.
    bool hasAlpha = false;
    for (size_t i = 3; i < pixels.bgra.size(); i += 4) {
        if (pixels.bgra[i] != 0) {
            hasAlpha = true;
            break;
        }
    }
    if (!hasAlpha) {
        // No alpha channel: treat near-black as transparent, rest as opaque.
        for (size_t i = 0; i + 3 < pixels.bgra.size(); i += 4) {
            const bool black = pixels.bgra[i] < 4 && pixels.bgra[i + 1] < 4 && pixels.bgra[i + 2] < 4;
            pixels.bgra[i + 3] = black ? 0 : 255;
        }
    } else {
        // Convert to premultiplied alpha so D2D renders edges cleanly without dark fringing.
        for (size_t i = 0; i + 3 < pixels.bgra.size(); i += 4) {
            const uint8_t a = pixels.bgra[i + 3];
            if (a < 255 && a > 0) {
                pixels.bgra[i + 0] = static_cast<uint8_t>(pixels.bgra[i + 0] * a / 255);
                pixels.bgra[i + 1] = static_cast<uint8_t>(pixels.bgra[i + 1] * a / 255);
                pixels.bgra[i + 2] = static_cast<uint8_t>(pixels.bgra[i + 2] * a / 255);
            }
        }
    }

    pixels.generation = ++g_artGenerationCounter;
    *outPixels = std::move(pixels);

    SelectObject(dc, old);
    DeleteObject(bitmap);
    DeleteDC(dc);
    return true;
}

bool ProcessImageNameForPid(DWORD pid, std::wstring* imageName) {
    if (!pid || !imageName) {
        return false;
    }

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!process) {
        return false;
    }

    wchar_t path[MAX_PATH] = {};
    DWORD size = ARRAYSIZE(path);
    const bool ok = QueryFullProcessImageNameW(process, 0, path, &size) != FALSE;
    CloseHandle(process);
    if (ok) {
        *imageName = path;
    }
    return ok;
}

HICON CopyWindowIcon(HWND hwnd, WPARAM iconType) {
    DWORD_PTR result = 0;
    SendMessageTimeoutW(hwnd, WM_GETICON, iconType, 0,
                        SMTO_ABORTIFHUNG | SMTO_BLOCK, 80, &result);
    return result ? CopyIcon(reinterpret_cast<HICON>(result)) : nullptr;
}

HICON getProcessIcon(DWORD pid) {
    std::wstring path;
    if (ProcessImageNameForPid(pid, &path) && !path.empty()) {
        HICON hIcon = nullptr;
        UINT iconId = 0;
        // Try to fetch a high-res 64x64 icon first to avoid pixelated icons
        using PrivateExtractIconsW_t = UINT(WINAPI*)(LPCWSTR, int, int, int, HICON*, UINT*, UINT, UINT);
        static auto pPrivateExtractIconsW = reinterpret_cast<PrivateExtractIconsW_t>(
            GetProcAddress(GetModuleHandleW(L"user32.dll"), "PrivateExtractIconsW"));
        if (pPrivateExtractIconsW && pPrivateExtractIconsW(path.c_str(), 0, 64, 64, &hIcon, &iconId, 1, 0) == 1 && hIcon) {
            return hIcon;
        }

        SHFILEINFOW sfi = {};
        if (SHGetFileInfoW(path.c_str(), 0, &sfi, sizeof(sfi),
                           SHGFI_ICON | SHGFI_LARGEICON)) {
            return sfi.hIcon;
        }

        HICON large = nullptr;
        HICON small = nullptr;
        if (ExtractIconExW(path.c_str(), 0, &large, &small, 1) > 0) {
            if (small) {
                DestroyIcon(small);
            }
            if (large) {
                return large;
            }
        }
    }

    return CopyIcon(LoadIconW(nullptr, IDI_APPLICATION));
}

HICON getWindowIcon(HWND hwnd) {
    if (!hwnd) {
        return CopyIcon(LoadIconW(nullptr, IDI_APPLICATION));
    }

    if (HICON icon = CopyWindowIcon(hwnd, ICON_BIG)) {
        return icon;
    }
    if (HICON icon = CopyWindowIcon(hwnd, ICON_SMALL)) {
        return icon;
    }

    if (auto icon = reinterpret_cast<HICON>(GetClassLongPtrW(hwnd, GCLP_HICON))) {
        return CopyIcon(icon);
    }
    if (auto icon = reinterpret_cast<HICON>(GetClassLongPtrW(hwnd, GCLP_HICONSM))) {
        return CopyIcon(icon);
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return getProcessIcon(pid);
}

std::wstring ToLowerCopy(std::wstring value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(towlower(ch)); });
    return value;
}

std::wstring BaseNameFromPath(std::wstring path) {
    const size_t slash = path.find_last_of(L"\\/");
    if (slash != std::wstring::npos) {
        path.erase(0, slash + 1);
    }
    return path;
}

std::wstring StripExtension(std::wstring value) {
    const size_t dot = value.find_last_of(L'.');
    if (dot != std::wstring::npos) {
        value.resize(dot);
    }
    return value;
}

bool ProcessImageNameForWindow(HWND hwnd, std::wstring* imageName) {
    if (!imageName) {
        return false;
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (!pid) {
        return false;
    }

    return ProcessImageNameForPid(pid, imageName);
}

std::wstring FriendlyMediaSourceName(std::wstring_view source) {
    const std::wstring lower = ToLowerCopy(std::wstring(source));
    if (lower.find(L"youtube") != std::wstring::npos) return L"YouTube";
    if (lower.find(L"spotify") != std::wstring::npos) return L"Spotify";
    if (lower.find(L"tidal") != std::wstring::npos) return L"Tidal";
    if (lower.find(L"deezer") != std::wstring::npos) return L"Deezer";
    if (lower.find(L"amazon") != std::wstring::npos && lower.find(L"music") != std::wstring::npos) return L"Amazon Music";
    if (lower.find(L"itunes") != std::wstring::npos || lower.find(L"applemusic") != std::wstring::npos) return L"Apple Music";
    if (lower.find(L"plex") != std::wstring::npos) return L"Plex";
    if (lower.find(L"mpc") != std::wstring::npos) return L"MPC";
    if (lower.find(L"chrome") != std::wstring::npos) return L"Chrome";
    if (lower.find(L"msedge") != std::wstring::npos || lower.find(L"edge") != std::wstring::npos) return L"Edge";
    if (lower.find(L"firefox") != std::wstring::npos) return L"Firefox";
    if (lower.find(L"vlc") != std::wstring::npos) return L"VLC";
    if (lower.find(L"wmplayer") != std::wstring::npos) return L"Windows Media";
    if (lower.find(L"zune") != std::wstring::npos || lower.find(L"media") != std::wstring::npos) return L"Media Player";

    std::wstring text(source);
    const size_t bang = text.find(L'!');
    if (bang != std::wstring::npos && bang + 1 < text.size()) {
        text.erase(0, bang + 1);
    }
    const size_t dot = text.find(L'.');
    if (dot != std::wstring::npos) {
        text.resize(dot);
    }
    return text.empty() ? L"Media" : text;
}

std::wstring MediaSourceBadge(std::wstring_view sourceName) {
    const std::wstring lower = ToLowerCopy(std::wstring(sourceName));
    if (lower.find(L"youtube") != std::wstring::npos) return L"YT";
    if (lower.find(L"spotify") != std::wstring::npos) return L"SP";
    if (lower.find(L"tidal") != std::wstring::npos) return L"TD";
    if (lower.find(L"deezer") != std::wstring::npos) return L"DZ";
    if (lower.find(L"amazon") != std::wstring::npos) return L"AM";
    if (lower.find(L"itunes") != std::wstring::npos || lower.find(L"applemusic") != std::wstring::npos) return L"♫";
    if (lower.find(L"plex") != std::wstring::npos) return L"PX";
    if (lower.find(L"mpc") != std::wstring::npos) return L"MP";
    if (lower.find(L"chrome") != std::wstring::npos) return L"CH";
    if (lower.find(L"edge") != std::wstring::npos) return L"ED";
    if (lower.find(L"firefox") != std::wstring::npos) return L"FF";
    if (lower.find(L"vlc") != std::wstring::npos) return L"VLC";
    if (sourceName.empty()) return L"\u25b6";
    std::wstring badge;
    badge.push_back(static_cast<wchar_t>(towupper(sourceName[0])));
    return badge;
}

bool WindowLooksLikeMediaSource(HWND hwnd, const std::wstring& sourceLower) {
    if (!IsWindowVisible(hwnd) || hwnd == g_hwnd) {
        return false;
    }

    std::wstring image;
    if (!ProcessImageNameForWindow(hwnd, &image)) {
        return false;
    }

    const std::wstring base = ToLowerCopy(BaseNameFromPath(image));
    if (base.empty()) {
        return false;
    }

    return sourceLower.find(base) != std::wstring::npos ||
           (base.find(L"chrome") != std::wstring::npos && sourceLower.find(L"chrome") != std::wstring::npos) ||
           (base.find(L"msedge") != std::wstring::npos && sourceLower.find(L"edge") != std::wstring::npos) ||
           (base.find(L"vlc") != std::wstring::npos && sourceLower.find(L"vlc") != std::wstring::npos);
}

bool IsBrowserMediaSource(std::wstring_view source) {
    const std::wstring lower = ToLowerCopy(std::wstring(source));
    return lower.find(L"chrome") != std::wstring::npos ||
           lower.find(L"edge") != std::wstring::npos ||
           lower.find(L"msedge") != std::wstring::npos ||
           lower.find(L"firefox") != std::wstring::npos ||
           lower.find(L"youtube") != std::wstring::npos;
}

std::wstring SiteBadgeFromTitle(std::wstring_view title) {
    const std::wstring lower = ToLowerCopy(std::wstring(title));
    if (lower.find(L"youtube") != std::wstring::npos) return L"YT";
    if (lower.find(L"netflix") != std::wstring::npos) return L"NF";
    if (lower.find(L"prime video") != std::wstring::npos || lower.find(L"amazon") != std::wstring::npos) return L"PV";
    if (lower.find(L"disney") != std::wstring::npos) return L"D+";
    if (lower.find(L"hotstar") != std::wstring::npos) return L"HS";
    if (lower.find(L"spotify") != std::wstring::npos) return L"SP";
    if (lower.find(L"soundcloud") != std::wstring::npos) return L"SC";
    return L"WEB";
}

struct IconCacheEntry {
    DWORD pid = 0;
    std::wstring exePath;
    BitmapPixels pixels;
    uint64_t lastUsed = 0;
};

std::mutex g_iconCacheMutex;
std::unordered_map<DWORD, IconCacheEntry> g_iconCacheByPid;
uint64_t g_iconCacheClock = 0;

BitmapPixels GetCachedProcessIconPixels(DWORD pid, UINT size) {
    std::wstring exePath;
    ProcessImageNameForPid(pid, &exePath);

    {
        std::lock_guard lock(g_iconCacheMutex);
        auto it = g_iconCacheByPid.find(pid);
        if (it != g_iconCacheByPid.end() && it->second.exePath == exePath &&
            it->second.pixels.width == size) {
            it->second.lastUsed = ++g_iconCacheClock;
            return it->second.pixels;
        }
    }

    BitmapPixels pixels;
    HICON icon = getProcessIcon(pid);
    if (icon) {
        IconToPixels(icon, size, &pixels);
        DestroyIcon(icon);
    }

    if (!pixels.bgra.empty()) {
        std::lock_guard lock(g_iconCacheMutex);
        g_iconCacheByPid[pid] = IconCacheEntry{pid, exePath, pixels, ++g_iconCacheClock};
        if (g_iconCacheByPid.size() > 32) {
            auto oldest = g_iconCacheByPid.begin();
            for (auto it = g_iconCacheByPid.begin(); it != g_iconCacheByPid.end(); ++it) {
                if (it->second.lastUsed < oldest->second.lastUsed) {
                    oldest = it;
                }
            }
            g_iconCacheByPid.erase(oldest);
        }
    }

    return pixels;
}

BitmapPixels GetWindowIconPixels(HWND hwnd, UINT size) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    BitmapPixels pixels = GetCachedProcessIconPixels(pid, size);
    if (!pixels.bgra.empty()) {
        return pixels;
    }

    HICON icon = getWindowIcon(hwnd);
    if (icon) {
        IconToPixels(icon, size, &pixels);
        DestroyIcon(icon);
    }
    return pixels;
}

bool IsIgnorableForegroundWindow(HWND hwnd, const std::wstring& title) {
    wchar_t className[128] = {};
    GetClassNameW(hwnd, className, ARRAYSIZE(className));
    const std::wstring cls = ToLowerCopy(className);
    const std::wstring lowerTitle = ToLowerCopy(title);

    std::wstring image;
    ProcessImageNameForWindow(hwnd, &image);
    const std::wstring base = ToLowerCopy(BaseNameFromPath(image));

    return title.empty() ||
           hwnd == g_hwnd ||
           cls == L"shell_traywnd" ||
           cls == L"workerw" ||
           cls == L"progman" ||
           lowerTitle.find(L"windhawk") != std::wstring::npos ||
           base == L"explorer.exe" ||
           base == L"windhawk.exe";
}

BOOL CALLBACK FindMediaSourceWindowProc(HWND hwnd, LPARAM lParam) {
    auto* data = reinterpret_cast<std::pair<const std::wstring*, HWND*>*>(lParam);
    if (WindowLooksLikeMediaSource(hwnd, *data->first)) {
        *data->second = hwnd;
        return FALSE;
    }
    return TRUE;
}

std::wstring FindBrowserMediaSiteBadge(const std::wstring& sourceAppUserModelId) {
    const std::wstring sourceLower = ToLowerCopy(sourceAppUserModelId);
    HWND found = nullptr;
    std::pair<const std::wstring*, HWND*> data{&sourceLower, &found};
    EnumWindows(FindMediaSourceWindowProc, reinterpret_cast<LPARAM>(&data));
    if (found) {
        wchar_t title[192] = {};
        GetWindowTextW(found, title, ARRAYSIZE(title));
        return SiteBadgeFromTitle(title);
    }
    return L"WEB";
}

BitmapPixels FindMediaSourceIcon(const std::wstring& sourceAppUserModelId) {
    BitmapPixels pixels;
    if (IsBrowserMediaSource(sourceAppUserModelId)) {
        // Continue searching for the browser window anyway to retrieve
        // the browser's app icon or the PWA's app icon.
    }

    const std::wstring sourceLower = ToLowerCopy(sourceAppUserModelId);
    HWND found = nullptr;
    std::pair<const std::wstring*, HWND*> data{&sourceLower, &found};
    EnumWindows(FindMediaSourceWindowProc, reinterpret_cast<LPARAM>(&data));
    if (found) {
        pixels = GetWindowIconPixels(found, 32);
    }
    return pixels;
}

struct FindAppIconData {
    const std::wstring* targetName;
    HWND bestHwnd = nullptr;
    std::unordered_map<DWORD, std::wstring> pidToProcessName;
};

BOOL CALLBACK FindAppIconWindowProc(HWND hwnd, LPARAM lParam) {
    auto* d = reinterpret_cast<FindAppIconData*>(lParam);
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    if (hwnd == g_hwnd) {
        return TRUE;
    }

    // 1. Fast path: Check window title first (lightweight check without opening process handles)
    wchar_t title[128] = {};
    GetWindowTextW(hwnd, title, ARRAYSIZE(title));
    std::wstring titleLower = ToLowerCopy(title);
    if (!titleLower.empty() && titleLower.find(*d->targetName) != std::wstring::npos) {
        d->bestHwnd = hwnd;
        return FALSE; // Found via title, stop enumeration
    }

    // 2. Slow path fallback: Check process executable name
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (!pid) {
        return TRUE;
    }

    std::wstring baseNoExe;
    auto it = d->pidToProcessName.find(pid);
    if (it != d->pidToProcessName.end()) {
        baseNoExe = it->second;
    } else {
        std::wstring image;
        if (ProcessImageNameForPid(pid, &image)) {
            std::wstring base = ToLowerCopy(BaseNameFromPath(image));
            baseNoExe = base;
            if (baseNoExe.size() > 4 && baseNoExe.substr(baseNoExe.size() - 4) == L".exe") {
                baseNoExe = baseNoExe.substr(0, baseNoExe.size() - 4);
            }
            d->pidToProcessName[pid] = baseNoExe;
        } else {
            d->pidToProcessName[pid] = L"";
        }
    }

    if (!baseNoExe.empty()) {
        if (baseNoExe.find(*d->targetName) != std::wstring::npos ||
            d->targetName->find(baseNoExe) != std::wstring::npos) {
            d->bestHwnd = hwnd;
            return FALSE; // Found via process name, stop enumeration
        }
    }

    return TRUE;
}

BitmapPixels FindAppIconByName(const std::wstring& appName, UINT size) {
    BitmapPixels pixels;
    if (appName.empty()) {
        return pixels;
    }

    const std::wstring appNameLower = ToLowerCopy(appName);
    FindAppIconData data;
    data.targetName = &appNameLower;

    EnumWindows(FindAppIconWindowProc, reinterpret_cast<LPARAM>(&data));

    if (data.bestHwnd) {
        pixels = GetWindowIconPixels(data.bestHwnd, size);
    }
    return pixels;
}

std::vector<uint8_t> ReadWinRtStreamBytes(
    const winrt::Windows::Storage::Streams::IRandomAccessStreamReference& reference) {
    std::vector<uint8_t> bytes;
    if (!reference) {
        return bytes;
    }

    auto stream = reference.OpenReadAsync().get();
    if (!stream) {
        return bytes;
    }

    const uint64_t size64 = stream.Size();
    if (size64 == 0 || size64 > 8 * 1024 * 1024) {
        return bytes;
    }

    const uint32_t size = static_cast<uint32_t>(size64);
    winrt::Windows::Storage::Streams::DataReader reader(stream.GetInputStreamAt(0));
    reader.LoadAsync(size).get();
    bytes.resize(size);
    reader.ReadBytes(winrt::array_view<uint8_t>(bytes.data(), bytes.data() + bytes.size()));
    return bytes;
}

void TriggerNudge() {
    const double now = NowSeconds();
    const double previous = g_lastNudgeTime.load();
    if (now - previous < 0.45) {
        return;
    }
    g_lastNudgeTime = now;
    HWND hwnd = g_hwnd;
    if (hwnd) {
        PostMessageW(hwnd, WM_APP_NEW_EVENT, 0, 0);
    }
}

DWORD WINAPI MediaThreadProc(void*) {
    winrt::init_apartment(winrt::apartment_type::multi_threaded);

    using Manager = winrt::Windows::Media::Control::GlobalSystemMediaTransportControlsSessionManager;
    using PlaybackStatus = winrt::Windows::Media::Control::GlobalSystemMediaTransportControlsSessionPlaybackStatus;

    Manager manager{nullptr};
    bool loggedUnavailable = false;

    while (WaitForSingleObject(g_stopEvent, 0) == WAIT_TIMEOUT) {
        MediaSnapshot next;

        try {
            if (!manager) {
                manager = Manager::RequestAsync().get();
            }

            if (manager) {
                auto session = manager.GetCurrentSession();
                if (session) {
                    auto properties = session.TryGetMediaPropertiesAsync().get();
                    auto playback = session.GetPlaybackInfo();
                    auto timeline = session.GetTimelineProperties();

                    next.available = true;
                    next.playing = playback.PlaybackStatus() == PlaybackStatus::Playing;
                    next.title = properties.Title().c_str();
                    next.artist = properties.Artist().c_str();
                    
                    if (timeline) {
                        int64_t np = timeline.Position().count();
                        int64_t ne = timeline.EndTime().count();
                        bool npP = (playback.PlaybackStatus() == PlaybackStatus::Playing);
                        
                        std::lock_guard lock(g_stateMutex);
                        if (np != g_state.media.positionTicks ||
                            ne != g_state.media.endTicks ||
                            npP != g_state.media.playing) {
                            next.positionTicks = np;
                            next.endTicks = ne;
                            next.lastUpdatedTicks = GetTickCount64();
                        } else {
                            next.positionTicks = g_state.media.positionTicks;
                            next.endTicks = g_state.media.endTicks;
                            next.lastUpdatedTicks = g_state.media.lastUpdatedTicks;
                        }
                    }

                    next.sourceAppUserModelId = session.SourceAppUserModelId().c_str();
                    next.sourceName = FriendlyMediaSourceName(next.sourceAppUserModelId);
                    
                    std::wstring prevSourceAppUserModelId;
                    bool hasPrevIcon = false;
                    BitmapPixels prevIcon;
                    uint64_t prevIconGeneration = 0;
                    std::wstring prevBadge;
                    {
                        std::lock_guard lock(g_stateMutex);
                        prevSourceAppUserModelId = g_state.media.sourceAppUserModelId;
                        hasPrevIcon = !g_state.media.sourceIcon.bgra.empty();
                        prevIcon = g_state.media.sourceIcon;
                        prevIconGeneration = g_state.media.sourceIconGeneration;
                        prevBadge = g_state.media.sourceBadge;
                    }

                    if (next.sourceAppUserModelId == prevSourceAppUserModelId) {
                        next.sourceBadge = prevBadge;
                        next.sourceIcon = prevIcon;
                        next.sourceIconGeneration = prevIconGeneration;
                        
                        if (!hasPrevIcon) {
                            next.sourceIcon = FindMediaSourceIcon(next.sourceAppUserModelId);
                            next.sourceIconGeneration = next.sourceIcon.generation;
                        }
                    } else {
                        next.sourceBadge = MediaSourceBadge(next.sourceName);
                        if (IsBrowserMediaSource(next.sourceAppUserModelId)) {
                            next.sourceBadge = FindBrowserMediaSiteBadge(next.sourceAppUserModelId);
                        }
                        next.sourceIcon = FindMediaSourceIcon(next.sourceAppUserModelId);
                        next.sourceIconGeneration = next.sourceIcon.generation;
                    }

                    if (auto thumbnail = properties.Thumbnail()) {
                        std::vector<uint8_t> bytes = ReadWinRtStreamBytes(thumbnail);
                        if (!bytes.empty()) {
                            BitmapPixels decoded;
                            if (DecodeImageBytesToPixels(bytes, &decoded)) {
                                next.art = std::move(decoded);
                                next.artGeneration = next.art.generation;
                                next.artChangedAt = NowSeconds();
                            }
                        }
                    }
                }
            }
        } catch (...) {
            if (!loggedUnavailable) {
                Wh_Log(L"WinRT media session unavailable; media module will fall back to idle.");
                loggedUnavailable = true;
            }
        }

        {
            std::lock_guard lock(g_stateMutex);
            const bool wasDifferent =
                next.playing != g_state.media.playing ||
                next.title != g_state.media.title ||
                next.artist != g_state.media.artist;

            if (!g_state.media.art.bgra.empty() &&
                next.title == g_state.media.title && next.artist == g_state.media.artist) {
                next.art = g_state.media.art;
                next.artGeneration = g_state.media.artGeneration;
                next.artChangedAt = g_state.media.artChangedAt;
            }
            if (next.sourceIcon.bgra.empty() &&
                next.sourceAppUserModelId == g_state.media.sourceAppUserModelId) {
                next.sourceIcon = g_state.media.sourceIcon;
                next.sourceIconGeneration = g_state.media.sourceIconGeneration;
            }

            g_state.media = std::move(next);
            if (wasDifferent && g_state.media.available) {
                TriggerNudge();
            }
        }

        WaitForSingleObject(g_stopEvent, 1500);
    }

    winrt::uninit_apartment();
    return 0;
}

#if DYNAMIC_ISLAND_HAS_USER_NOTIFICATION_LISTENER
DWORD WINAPI NotificationThreadProc(void*) {
    winrt::init_apartment(winrt::apartment_type::multi_threaded);

    using winrt::Windows::UI::Notifications::KnownNotificationBindings;
    using winrt::Windows::UI::Notifications::NotificationKinds;
    using winrt::Windows::UI::Notifications::Management::UserNotificationListener;
    using winrt::Windows::UI::Notifications::Management::UserNotificationListenerAccessStatus;

    uint32_t lastSeenId = 0;
    bool accessLogged = false;

    try {
        auto listener = UserNotificationListener::Current();
        auto access = listener.RequestAccessAsync().get();
        if (access != UserNotificationListenerAccessStatus::Allowed) {
            Wh_Log(L"Notification listener permission not granted; shell-hook notification fallback remains active.");
            winrt::uninit_apartment();
            return 0;
        }

        while (WaitForSingleObject(g_stopEvent, 0) == WAIT_TIMEOUT) {
            try {
                auto notifications = listener.GetNotificationsAsync(NotificationKinds::Toast).get();
                for (uint32_t i = 0; i < notifications.Size(); ++i) {
                    auto userNotification = notifications.GetAt(i);
                    const uint32_t id = userNotification.Id();
                    if (id <= lastSeenId) {
                        continue;
                    }

                    NotificationSnapshot snapshot;
                    snapshot.active = true;
                    snapshot.expiresAt = NowSeconds() + 4.0;
                    auto appInfo = userNotification.AppInfo();
                    auto displayInfo = appInfo.DisplayInfo();
                    snapshot.app = displayInfo.DisplayName().c_str();
                    try {
                        auto logo = displayInfo.GetLogo({32.0f, 32.0f});
                        std::vector<uint8_t> logoBytes = ReadWinRtStreamBytes(logo);
                        if (!logoBytes.empty()) {
                            DecodeImageBytesToPixels(logoBytes, &snapshot.icon);
                        }
                    } catch (...) {
                    }

                    if (snapshot.icon.bgra.empty() && !snapshot.app.empty()) {
                        snapshot.icon = FindAppIconByName(snapshot.app, 64);
                    }

                    auto notification = userNotification.Notification();
                    auto binding = notification.Visual().GetBinding(KnownNotificationBindings::ToastGeneric());
                    if (binding) {
                        auto textElements = binding.GetTextElements();
                        if (textElements.Size() > 0) {
                            snapshot.title = textElements.GetAt(0).Text().c_str();
                        }
                        if (textElements.Size() > 1) {
                            snapshot.body = textElements.GetAt(1).Text().c_str();
                        }
                    }

                    if (snapshot.title.empty()) {
                        snapshot.title = snapshot.app.empty() ? L"New notification" : snapshot.app;
                    }
                    if (!snapshot.body.empty()) {
                        snapshot.title += L" - " + snapshot.body;
                    }
                    if (snapshot.title.size() > 120) {
                        snapshot.title.resize(120);
                        snapshot.title += L"...";
                    }

                    {
                        std::lock_guard lock(g_stateMutex);
                        g_state.notification = std::move(snapshot);
                    }
                    TriggerNudge();
                    lastSeenId = id;
                }
            } catch (const winrt::hresult_error& ex) {
                const HRESULT hr = ex.to_abi();
                Wh_Log(L"NotificationThreadProc loop WinRT error: %s (0x%08X)", ex.message().c_str(), hr);
                if (hr == E_NOTIMPL || hr == E_ACCESSDENIED || hr == REGDB_E_CLASSNOTREG) {
                    Wh_Log(L"NotificationThreadProc: Fatal/Unsupported WinRT context. Exiting WinRT notification listener thread.");
                    break;
                }
            } catch (...) {
                Wh_Log(L"NotificationThreadProc loop unknown exception.");
            }

            WaitForSingleObject(g_stopEvent, 1000);
        }
    } catch (const winrt::hresult_error& ex) {
        Wh_Log(L"NotificationThreadProc initialization WinRT error: %s (0x%08X). Falling back to shell hook.", ex.message().c_str(), ex.to_abi());
    } catch (...) {
        Wh_Log(L"NotificationThreadProc initialization unknown exception. Falling back to shell hook.");
    }

    winrt::uninit_apartment();
    return 0;
}
#endif

float SampleAudioAmplitude(BYTE* data, UINT32 frames, WAVEFORMATEX* format) {
    if (!data || !frames || !format || !format->nChannels) {
        return 0.0f;
    }

    double sum = 0.0;
    size_t samples = static_cast<size_t>(frames) * format->nChannels;

    if (format->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
        (format->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
         format->cbSize >= sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) &&
         IsEqualGUID(reinterpret_cast<WAVEFORMATEXTENSIBLE*>(format)->SubFormat,
                     kSubTypeIeeeFloat))) {
        auto* f = reinterpret_cast<float*>(data);
        for (size_t i = 0; i < samples; ++i) {
            sum += f[i] * f[i];
        }
    } else if (format->wBitsPerSample == 16) {
        auto* s = reinterpret_cast<int16_t*>(data);
        for (size_t i = 0; i < samples; ++i) {
            const double v = s[i] / 32768.0;
            sum += v * v;
        }
    }

    const double rms = samples ? std::sqrt(sum / samples) : 0.0;
    return Clamp(static_cast<float>(rms * 4.0), 0.0f, 1.0f);
}

void PushWaveformSample(float amplitude) {
    std::lock_guard lock(g_stateMutex);
    
    float lastVal = 0.0f;
    if (g_state.waveformWrite > 0) {
        lastVal = g_state.waveform[(g_state.waveformWrite - 1) % g_state.waveform.size()];
    }

    // Apply an attack/release envelope (Exponential Moving Average)
    // Quick snappy attack (0.7) for beats, buttery smooth release (0.85) for decay.
    float smoothed;
    if (amplitude > lastVal) {
        smoothed = lastVal * 0.3f + amplitude * 0.7f;
    } else {
        smoothed = lastVal * 0.85f + amplitude * 0.15f;
    }

    g_state.waveform[g_state.waveformWrite % g_state.waveform.size()] = smoothed;
    ++g_state.waveformWrite;
}

void PushAudioChunks(BYTE* data, UINT32 frames, WAVEFORMATEX* format) {
    if (!data || !frames || !format || !format->nChannels) {
        PushWaveformSample(0.0f);
        return;
    }

    constexpr UINT32 chunkFrames = 64;
    const UINT32 channels = format->nChannels;
    const bool isFloat =
        format->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
        (format->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
         format->cbSize >= sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) &&
         IsEqualGUID(reinterpret_cast<WAVEFORMATEXTENSIBLE*>(format)->SubFormat,
                     kSubTypeIeeeFloat));
    const bool is16 = format->wBitsPerSample == 16;

    if (!isFloat && !is16) {
        PushWaveformSample(SampleAudioAmplitude(data, frames, format));
        return;
    }

    for (UINT32 frame = 0; frame < frames; frame += chunkFrames) {
        const UINT32 chunk = std::min(chunkFrames, frames - frame);
        double sum = 0.0;
        const size_t samples = static_cast<size_t>(chunk) * channels;
        const size_t start = static_cast<size_t>(frame) * channels;

        if (isFloat) {
            auto* f = reinterpret_cast<float*>(data);
            for (size_t i = 0; i < samples; ++i) {
                const double v = f[start + i];
                sum += v * v;
            }
        } else {
            auto* s = reinterpret_cast<int16_t*>(data);
            for (size_t i = 0; i < samples; ++i) {
                const double v = s[start + i] / 32768.0;
                sum += v * v;
            }
        }

        PushWaveformSample(Clamp(static_cast<float>(std::sqrt(sum / samples) * 4.0), 0.0f, 1.0f));
    }
}

// --- Weather Fetching Helpers ---
std::string HttpGet(const wchar_t* host, const wchar_t* path, bool https = true) {
    std::string response;
    HINTERNET hSession = WinHttpOpen(L"DynamicIsland/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        Wh_Log(L"Weather HttpGet: WinHttpOpen failed with error %lu", GetLastError());
        return response;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, host, https ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
    if (hConnect) {
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, https ? WINHTTP_FLAG_SECURE : 0);
        if (hRequest) {
            if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
                WinHttpReceiveResponse(hRequest, nullptr)) {
                DWORD size = 0;
                DWORD downloaded = 0;
                do {
                    if (WinHttpQueryDataAvailable(hRequest, &size) && size > 0) {
                        std::vector<char> buffer(size + 1);
                        if (WinHttpReadData(hRequest, buffer.data(), size, &downloaded)) {
                            buffer[downloaded] = '\0';
                            response.append(buffer.data());
                        } else {
                            Wh_Log(L"Weather HttpGet: WinHttpReadData failed with error %lu", GetLastError());
                        }
                    }
                } while (size > 0);
            } else {
                Wh_Log(L"Weather HttpGet: WinHttpSendRequest/ReceiveResponse failed with error %lu", GetLastError());
            }
            WinHttpCloseHandle(hRequest);
        } else {
            Wh_Log(L"Weather HttpGet: WinHttpOpenRequest failed with error %lu", GetLastError());
        }
        WinHttpCloseHandle(hConnect);
    } else {
        Wh_Log(L"Weather HttpGet: WinHttpConnect failed with error %lu", GetLastError());
    }
    WinHttpCloseHandle(hSession);
    return response;
}

DWORD WINAPI WeatherThreadProc(void*) {
    // Initial delay to avoid slowing down startup
    WaitForSingleObject(g_stopEvent, 3000);

    while (WaitForSingleObject(g_stopEvent, 0) == WAIT_TIMEOUT) {
        std::wstring cityOverride;
        bool isFahrenheit = false;
        {
            std::lock_guard lock(g_stateMutex);
            cityOverride = g_settings.weatherCity;
            isFahrenheit = g_settings.weatherFahrenheit;
        }

        std::wstring url = L"/?format=j1";
        if (!cityOverride.empty()) {
            std::wstring urlName = cityOverride;
            size_t pos = 0;
            while ((pos = urlName.find(L" ", pos)) != std::wstring::npos) {
                urlName.replace(pos, 1, L"%20");
                pos += 3;
            }
            url = L"/" + urlName + L"?format=j1";
        }

        Wh_Log(L"Weather: Requesting weather from wttr.in/host: wttr.in, path: %s", url.c_str());
        std::string wRes = HttpGet(L"wttr.in", url.c_str(), true);
        if (wRes.empty()) {
            Wh_Log(L"Weather: HTTPS request failed, retrying over plain HTTP...");
            wRes = HttpGet(L"wttr.in", url.c_str(), false);
        }
        
        if (!wRes.empty()) {
            Wh_Log(L"Weather: Received response from wttr.in (size: %zu bytes)", wRes.size());
            float temp = 0.0f;
            int code = 0;
            std::wstring desc = L"";
            std::wstring windSpeed = L"";
            std::wstring windDir = L"";
            std::wstring humidity = L"";
            std::wstring feelsLike = L"";
            std::wstring uvIndex = L"";
            std::wstring tempMax = L"";
            std::wstring tempMin = L"";
            std::wstring cityLabel = L"Local Weather";
            
            const char* areaStr = strstr(wRes.c_str(), "\"areaName\":");
            if (areaStr) {
                const char* valStr = strstr(areaStr, "\"value\":");
                if (valStr) {
                    valStr += 8;
                    while (*valStr == ' ' || *valStr == '\"') valStr++;
                    const char* end = strchr(valStr, '\"');
                    if (end) {
                        std::string cityA(valStr, end - valStr);
                        int wchars_num = MultiByteToWideChar(CP_UTF8, 0, cityA.c_str(), -1, NULL, 0);
                        if (wchars_num > 0) {
                            std::vector<wchar_t> wstr(wchars_num);
                            MultiByteToWideChar(CP_UTF8, 0, cityA.c_str(), -1, &wstr[0], wchars_num);
                            cityLabel = wstr.data();
                        }
                    }
                }
            }
            
            const char* currentStr = strstr(wRes.c_str(), "\"current_condition\":");
            if (currentStr) {
                auto ParseStringField = [&](const char* key, std::wstring& out) {
                    const char* kStr = strstr(currentStr, key);
                    if (kStr) {
                        kStr += strlen(key);
                        while (*kStr == ' ' || *kStr == '\"' || *kStr == ':') kStr++;
                        const char* end = strchr(kStr, '\"');
                        if (end) {
                            std::string valA(kStr, end - kStr);
                            int wchars_num = MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, NULL, 0);
                            if (wchars_num > 0) {
                                std::vector<wchar_t> wstr(wchars_num);
                                MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, &wstr[0], wchars_num);
                                out = wstr.data();
                            }
                        }
                    }
                };

                const char* tempStr = strstr(currentStr, isFahrenheit ? "\"temp_F\":" : "\"temp_C\":");
                if (tempStr) {
                    tempStr += 9;
                    while (*tempStr == ' ' || *tempStr == '\"') tempStr++;
                    sscanf(tempStr, "%f", &temp);
                }
                const char* codeStr = strstr(currentStr, "\"weatherCode\":");
                if (codeStr) {
                    codeStr += 14;
                    while (*codeStr == ' ' || *codeStr == '\"') codeStr++;
                    sscanf(codeStr, "%d", &code);
                }
                
                const char* descStr = strstr(currentStr, "\"weatherDesc\":");
                if (descStr) {
                    const char* valStr = strstr(descStr, "\"value\":");
                    if (valStr) {
                        valStr += 8;
                        while (*valStr == ' ' || *valStr == '\"') valStr++;
                        const char* end = strchr(valStr, '\"');
                        if (end) {
                            std::string valA(valStr, end - valStr);
                            int wchars_num = MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, NULL, 0);
                            if (wchars_num > 0) {
                                std::vector<wchar_t> wstr(wchars_num);
                                MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, &wstr[0], wchars_num);
                                desc = wstr.data();
                                while(!desc.empty() && desc.back() == L' ') desc.pop_back();
                            }
                        }
                    }
                }
                
                ParseStringField(isFahrenheit ? "\"windspeedMiles\"" : "\"windspeedKmph\"", windSpeed);
                ParseStringField("\"winddir16Point\"", windDir);
                ParseStringField("\"humidity\"", humidity);
                ParseStringField(isFahrenheit ? "\"FeelsLikeF\"" : "\"FeelsLikeC\"", feelsLike);
                ParseStringField("\"uvIndex\"", uvIndex);

                tempMax = L"";
                tempMin = L"";
                const char* maxKey = isFahrenheit ? "\"maxtempF\":" : "\"maxtempC\":";
                const char* minKey = isFahrenheit ? "\"mintempF\":" : "\"mintempC\":";
                const char* maxStr = strstr(wRes.c_str(), maxKey);
                if (maxStr) {
                    maxStr += strlen(maxKey);
                    while (*maxStr == ' ' || *maxStr == '\"' || *maxStr == ':') maxStr++;
                    const char* end = strchr(maxStr, '\"');
                    if (end) {
                        std::string valA(maxStr, end - maxStr);
                        int wchars_num = MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, NULL, 0);
                        if (wchars_num > 0) {
                            std::vector<wchar_t> wstr(wchars_num);
                            MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, &wstr[0], wchars_num);
                            tempMax = wstr.data();
                        }
                    }
                }
                const char* minStr = strstr(wRes.c_str(), minKey);
                if (minStr) {
                    minStr += strlen(minKey);
                    while (*minStr == ' ' || *minStr == '\"' || *minStr == ':') minStr++;
                    const char* end = strchr(minStr, '\"');
                    if (end) {
                        std::string valA(minStr, end - minStr);
                        int wchars_num = MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, NULL, 0);
                        if (wchars_num > 0) {
                            std::vector<wchar_t> wstr(wchars_num);
                            MultiByteToWideChar(CP_UTF8, 0, valA.c_str(), -1, &wstr[0], wchars_num);
                            tempMin = wstr.data();
                        }
                    }
                }

                std::wstring finalCity = cityOverride.empty() ? cityLabel : cityOverride;
                Wh_Log(L"Weather parsed success: city=%s, temp=%.1f, max=%s, min=%s, feelsLike=%s, humidity=%s%%, uvIndex=%s, desc=%s",
                       finalCity.c_str(), temp, tempMax.c_str(), tempMin.c_str(), feelsLike.c_str(), humidity.c_str(), uvIndex.c_str(), desc.c_str());
            } else {
                Wh_Log(L"Weather: Failed to find \"current_condition\" in response.");
            }
            
            {
                std::lock_guard lock(g_stateMutex);
                g_state.weather.hasData = true;
                g_state.weather.temperature = temp;
                g_state.weather.weatherCode = code;
                if (!cityOverride.empty()) g_state.weather.city = cityOverride;
                else g_state.weather.city = cityLabel;
                g_state.weather.weatherDesc = desc;
                g_state.weather.windSpeed = windSpeed;
                g_state.weather.windDir = windDir;
                g_state.weather.humidity = humidity;
                g_state.weather.feelsLike = feelsLike;
                g_state.weather.uvIndex = uvIndex;
                g_state.weather.tempMax = tempMax;
                g_state.weather.tempMin = tempMin;
                g_state.weather.lastUpdated = NowSeconds();
            }
        } else {
            Wh_Log(L"Weather: HttpGet returned empty response.");
        }

        HANDLE events[] = {g_stopEvent, g_settingsChangedEvent};
        DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, 15 * 60 * 1000);
        if (waitResult == WAIT_OBJECT_0) {
            break;
        }
    }
    return 0;
}

DWORD WINAPI AudioThreadProc(void*) {
    HRESULT hrCo = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    while (WaitForSingleObject(g_stopEvent, 0) == WAIT_TIMEOUT) {
        ComPtr<IMMDeviceEnumerator> enumerator;
        ComPtr<IMMDevice> device;
        ComPtr<IAudioClient> client;
        ComPtr<IAudioCaptureClient> capture;
        WAVEFORMATEX* mixFormat = nullptr;

        HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                                      CLSCTX_ALL, IID_PPV_ARGS(&enumerator));
        if (SUCCEEDED(hr)) {
            hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
        }
        if (SUCCEEDED(hr)) {
            hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                                  reinterpret_cast<void**>(client.GetAddressOf()));
        }
        if (SUCCEEDED(hr)) {
            hr = client->GetMixFormat(&mixFormat);
        }
        if (SUCCEEDED(hr)) {
            REFERENCE_TIME bufferDuration = 10000000 / 5;
            hr = client->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                    AUDCLNT_STREAMFLAGS_LOOPBACK,
                                    bufferDuration, 0, mixFormat, nullptr);
        }
        if (SUCCEEDED(hr)) {
            hr = client->GetService(IID_PPV_ARGS(&capture));
        }
        if (SUCCEEDED(hr)) {
            hr = client->Start();
        }

        if (FAILED(hr)) {
            if (mixFormat) {
                CoTaskMemFree(mixFormat);
            }
            WaitForSingleObject(g_stopEvent, 2500);
            continue;
        }

        while (WaitForSingleObject(g_stopEvent, 16) == WAIT_TIMEOUT) {
            UINT32 packetFrames = 0;
            if (FAILED(capture->GetNextPacketSize(&packetFrames))) {
                break;
            }

            float amplitude = 0.0f;
            int packets = 0;
            while (packetFrames > 0) {
                BYTE* data = nullptr;
                UINT32 frames = 0;
                DWORD flags = 0;
                if (FAILED(capture->GetBuffer(&data, &frames, &flags, nullptr, nullptr))) {
                    break;
                }

                if (!(flags & AUDCLNT_BUFFERFLAGS_SILENT)) {
                    amplitude = std::max(amplitude, SampleAudioAmplitude(data, frames, mixFormat));
                    PushAudioChunks(data, frames, mixFormat);
                } else {
                    PushWaveformSample(0.0f);
                }
                capture->ReleaseBuffer(frames);
                ++packets;

                if (FAILED(capture->GetNextPacketSize(&packetFrames))) {
                    packetFrames = 0;
                }
            }

            if (packets > 0) {
                if (amplitude <= 0.001f) {
                    PushWaveformSample(0.0f);
                }
            } else {
                PushWaveformSample(0.0f);
            }
        }

        client->Stop();
        if (mixFormat) {
            CoTaskMemFree(mixFormat);
        }
    }

    if (SUCCEEDED(hrCo)) {
        CoUninitialize();
    }

    return 0;
}

void UpdateBatterySnapshot() {
    SYSTEM_POWER_STATUS status = {};
    if (!GetSystemPowerStatus(&status)) {
        return;
    }

    bool newCharging = (status.ACLineStatus == 1);
    int newPercent = status.BatteryLifePercent == 255 ? 100 : status.BatteryLifePercent;

    bool triggerAlert = false;

    {
        std::lock_guard lock(g_stateMutex);
        static bool s_batteryInit = false;
        if (!s_batteryInit) {
            g_state.battery.charging = newCharging;
            g_state.battery.percent = newPercent;
            s_batteryInit = true;
        }

        // Alert: charge state changed (plugged in / unplugged)
        if (g_state.battery.charging != newCharging) {
            triggerAlert = true;
        }

        // Alert: battery full while charging (once, when crossing 100%)
        if (newCharging && newPercent == 100 && g_state.battery.percent < 100) {
            triggerAlert = true;
        }

        // Alert: dropped into a low-battery band while discharging.
        // Fires on the first poll that lands at or below 20%, 15%, 10%, 5%.
        if (!newCharging && newPercent < g_state.battery.percent) {
            const int thresholds[] = {20, 15, 10, 5};
            for (int t : thresholds) {
                if (g_state.battery.percent > t && newPercent <= t) {
                    triggerAlert = true;
                    break;
                }
            }
        }

        g_state.battery.charging = newCharging;
        g_state.battery.percent = newPercent;
        g_state.battery.secondsRemaining = status.BatteryLifeTime;
        g_state.battery.low = (!newCharging && newPercent <= 20);

        if (triggerAlert) {
            g_state.battery.active = true;
            g_state.battery.expiresAt = NowSeconds() + 4.0;
        }
    }

    if (triggerAlert) {
        TriggerNudge();
    }
}

ULONGLONG FileTimeToUInt64(FILETIME ft) {
    ULARGE_INTEGER value = {};
    value.LowPart = ft.dwLowDateTime;
    value.HighPart = ft.dwHighDateTime;
    return value.QuadPart;
}

static PDH_HQUERY g_gpuQuery = NULL;
static PDH_HCOUNTER g_gpuCounter = NULL;

static void InitGpuQuery() {
    if (g_gpuQuery == NULL) {
        if (PdhOpenQueryW(NULL, 0, &g_gpuQuery) == ERROR_SUCCESS) {
            PdhAddEnglishCounterW(g_gpuQuery, L"\\GPU Engine(*)\\Utilization Percentage", 0, &g_gpuCounter);
            PdhCollectQueryData(g_gpuQuery);
        }
    }
}

static int GetGpuUsage() {
    InitGpuQuery();
    if (!g_gpuQuery || !g_gpuCounter) return 0;
    
    PdhCollectQueryData(g_gpuQuery);
    
    DWORD bufferSize = 0;
    DWORD itemCount = 0;
    PdhGetFormattedCounterArrayW(g_gpuCounter, PDH_FMT_DOUBLE, &bufferSize, &itemCount, NULL);
    
    if (bufferSize > 0) {
        std::vector<BYTE> buffer(bufferSize);
        PDH_FMT_COUNTERVALUE_ITEM_W* items = reinterpret_cast<PDH_FMT_COUNTERVALUE_ITEM_W*>(buffer.data());
        
        if (PdhGetFormattedCounterArrayW(g_gpuCounter, PDH_FMT_DOUBLE, &bufferSize, &itemCount, items) == ERROR_SUCCESS) {
            double total = 0;
            for (DWORD i = 0; i < itemCount; i++) {
                if (items[i].szName && wcsstr(items[i].szName, L"engtype_3D")) {
                    total += items[i].FmtValue.doubleValue;
                }
            }
            return ClampInt(static_cast<int>(total), 0, 100);
        }
    }
    return 0;
}


inline bool IsBluetoothFriendlyName(const std::wstring& name) {
    if (name.empty()) return false;
    std::wstring lower = name;
    for (auto& c : lower) {
        c = std::towlower(c);
    }
    return lower.find(L"airpods") != std::wstring::npos ||
           lower.find(L"air pods") != std::wstring::npos ||
           lower.find(L"buds") != std::wstring::npos ||
           lower.find(L"bluetooth") != std::wstring::npos ||
           lower.find(L"wireless") != std::wstring::npos ||
           lower.find(L"headphone") != std::wstring::npos ||
           lower.find(L"headset") != std::wstring::npos ||
           lower.find(L"hands-free") != std::wstring::npos ||
           lower.find(L"hands free") != std::wstring::npos ||
           lower.find(L"earphone") != std::wstring::npos ||
           lower.find(L"stereo") != std::wstring::npos;
}

inline std::wstring GetDevicePropertyString(IPropertyStore* props, ULONG pid) {
    std::wstring result;
    PROPERTYKEY key = {
        {0xa45c254e, 0xdf1c, 0x4efd,
         {0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0}}, pid};
    PROPVARIANT pv;
    PropVariantInit(&pv);
    if (SUCCEEDED(props->GetValue(key, &pv)) && pv.vt == VT_LPWSTR && pv.pwszVal) {
        result = pv.pwszVal;
    }
    PropVariantClear(&pv);
    return result;
}

inline bool IsBluetoothDevice(const std::wstring& friendlyName, const std::wstring& instanceId, const std::wstring& enumerator, const std::wstring& busName) {
    auto CheckStr = [](const std::wstring& s) {
        if (s.empty()) return false;
        std::wstring lower = s;
        for (auto& c : lower) c = std::towlower(c);
        return lower.find(L"bth") != std::wstring::npos ||
               lower.find(L"bluetooth") != std::wstring::npos ||
               lower.find(L"airpods") != std::wstring::npos ||
               lower.find(L"air pods") != std::wstring::npos ||
               lower.find(L"buds") != std::wstring::npos ||
               lower.find(L"headphone") != std::wstring::npos ||
               lower.find(L"headset") != std::wstring::npos ||
               lower.find(L"hands-free") != std::wstring::npos ||
               lower.find(L"hands free") != std::wstring::npos ||
               lower.find(L"earphone") != std::wstring::npos ||
               lower.find(L"wireless") != std::wstring::npos;
    };
    return CheckStr(friendlyName) || CheckStr(instanceId) || CheckStr(enumerator) || CheckStr(busName);
}

typedef BOOL (WINAPI *t_InternetGetConnectedState)(LPDWORD, DWORD);

inline bool IsInternetConnected() {
    HMODULE hLib = LoadLibraryW(L"wininet.dll");
    if (!hLib) return false;

    auto pInternetGetConnectedState = (t_InternetGetConnectedState)GetProcAddress(hLib, "InternetGetConnectedState");
    bool connected = false;
    if (pInternetGetConnectedState) {
        DWORD flags = 0;
        connected = pInternetGetConnectedState(&flags, 0);
    }
    FreeLibrary(hLib);
    return connected;
}

#ifndef BLUETOOTH_FIND_RADIO_PARAMS
struct BLUETOOTH_FIND_RADIO_PARAMS {
    DWORD dwSize;
};
#endif

typedef HANDLE (WINAPI *t_BluetoothFindFirstRadio)(const BLUETOOTH_FIND_RADIO_PARAMS*, HANDLE*);
typedef BOOL (WINAPI *t_BluetoothFindRadioClose)(HANDLE);

inline bool IsBluetoothOn() {
    HMODULE hLib = LoadLibraryW(L"bthprops.cpl");
    if (!hLib) hLib = LoadLibraryW(L"irprops.cpl");
    if (!hLib) return false;

    auto pBluetoothFindFirstRadio = (t_BluetoothFindFirstRadio)GetProcAddress(hLib, "BluetoothFindFirstRadio");
    auto pBluetoothFindRadioClose = (t_BluetoothFindRadioClose)GetProcAddress(hLib, "BluetoothFindRadioClose");

    bool isOn = false;
    if (pBluetoothFindFirstRadio && pBluetoothFindRadioClose) {
        BLUETOOTH_FIND_RADIO_PARAMS params = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
        HANDLE hRadio = nullptr;
        HANDLE hFind = pBluetoothFindFirstRadio(&params, &hRadio);
        if (hFind) {
            isOn = true;
            CloseHandle(hRadio);
            pBluetoothFindRadioClose(hFind);
        }
    }
    FreeLibrary(hLib);
    return isOn;
}


void UpdateSystemSnapshot() {
    SystemSnapshot next;
    {
        std::lock_guard lock(g_stateMutex);
        next = g_state.system;
        next.charging = g_state.system.charging;
    }

    next.internetConnected = IsInternetConnected();
    next.bluetoothOn = IsBluetoothOn();

    next.gpuPercent = GetGpuUsage();

    MEMORYSTATUSEX memory = {};
    memory.dwLength = sizeof(memory);
    if (GlobalMemoryStatusEx(&memory)) {
        next.memoryPercent = static_cast<int>(memory.dwMemoryLoad);
    }

    ULARGE_INTEGER freeBytesAvailable = {};
    ULARGE_INTEGER totalBytes = {};
    ULARGE_INTEGER totalFreeBytes = {};
    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes) &&
        totalBytes.QuadPart > 0) {
        next.diskFreePercent = ClampInt(
            static_cast<int>(totalFreeBytes.QuadPart * 100 / totalBytes.QuadPart), 0, 100);
    }

    HWND foreground = GetForegroundWindow();
    if (foreground && foreground != g_hwnd) {
        wchar_t title[96] = {};
        GetWindowTextW(foreground, title, ARRAYSIZE(title));
        if (!IsIgnorableForegroundWindow(foreground, title)) {
            next.foregroundTitle = title;
            if (next.foregroundTitle.size() > 42) {
                next.foregroundTitle.resize(42);
                next.foregroundTitle += L"...";
            }

        } else {
            next.foregroundTitle.clear();
        }
    }

    FILETIME idle = {};
    FILETIME kernel = {};
    FILETIME user = {};
    if (GetSystemTimes(&idle, &kernel, &user)) {
        const ULONGLONG idleNow = FileTimeToUInt64(idle);
        const ULONGLONG kernelNow = FileTimeToUInt64(kernel);
        const ULONGLONG userNow = FileTimeToUInt64(user);
        const ULONGLONG idlePrev = FileTimeToUInt64(g_prevIdleTime);
        const ULONGLONG kernelPrev = FileTimeToUInt64(g_prevKernelTime);
        const ULONGLONG userPrev = FileTimeToUInt64(g_prevUserTime);

        const ULONGLONG total = (kernelNow - kernelPrev) + (userNow - userPrev);
        const ULONGLONG idleDelta = idleNow - idlePrev;
        if (total > 0 && kernelPrev != 0) {
            next.cpuPercent = ClampInt(static_cast<int>((total - idleDelta) * 100 / total), 0, 100);
        }

        g_prevIdleTime = idle;
        g_prevKernelTime = kernel;
        g_prevUserTime = user;
    }

    static ComPtr<IAudioEndpointVolume> s_volume;  // declared here so endpoint block can call s_volume.Reset()

    // ── Audio endpoint change detection ─────────────────────────────────────

    // WM_DEVICECHANGE with DBT_DEVICEARRIVAL is NOT sent for Bluetooth audio
    // devices without RegisterDeviceNotification. Instead, we track the default
    // audio endpoint ID every poll cycle; a change = device connect/disconnect.
    {
        static ComPtr<IMMDeviceEnumerator> s_epEnum;
        static std::wstring s_lastEndpointId;

        if (!s_epEnum) {
            CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                             CLSCTX_ALL, IID_PPV_ARGS(&s_epEnum));
        }
        if (s_epEnum) {
            ComPtr<IMMDevice> epDev;
            if (SUCCEEDED(s_epEnum->GetDefaultAudioEndpoint(eRender, eConsole, &epDev))) {
                wchar_t* rawId = nullptr;
                std::wstring currentId;
                if (SUCCEEDED(epDev->GetId(&rawId)) && rawId) {
                    currentId = rawId;
                    CoTaskMemFree(rawId);
                }

                bool endpointChanged = !currentId.empty() &&
                                       !s_lastEndpointId.empty() &&
                                       currentId != s_lastEndpointId;
                if (!currentId.empty()) s_lastEndpointId = currentId;

                if (endpointChanged) {
                    g_suppressVolumeUntil = NowSeconds() + 8.0;
                    s_volume.Reset();  // force s_volume to reconnect to new endpoint
                }

                // Query collection of active rendering endpoints to detect BT headphone connect/disconnect
                ComPtr<IMMDeviceCollection> collection;
                if (SUCCEEDED(s_epEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &collection))) {
                    UINT count = 0;
                    collection->GetCount(&count);

                    std::vector<std::wstring> currentBtIds;
                    std::vector<std::wstring> currentBtNames;

                    for (UINT i = 0; i < count; i++) {
                        ComPtr<IMMDevice> dev;
                        if (SUCCEEDED(collection->Item(i, &dev))) {
                            wchar_t* rawId = nullptr;
                            std::wstring devId;
                            if (SUCCEEDED(dev->GetId(&rawId)) && rawId) {
                                devId = rawId;
                                CoTaskMemFree(rawId);
                            }

                            std::wstring friendlyName;
                            std::wstring instanceId;
                            std::wstring enumerator;
                            std::wstring busName;

                            ComPtr<IPropertyStore> props;
                            if (SUCCEEDED(dev->OpenPropertyStore(STGM_READ, &props))) {
                                friendlyName = GetDevicePropertyString(props.Get(), 14); // PKEY_Device_FriendlyName
                                instanceId   = GetDevicePropertyString(props.Get(), 2);  // PKEY_Device_InstanceId
                                enumerator   = GetDevicePropertyString(props.Get(), 24); // PKEY_Device_EnumeratorName
                                busName      = GetDevicePropertyString(props.Get(), 4);  // PKEY_Device_BusReportedDeviceName
                            }

                            if (IsBluetoothDevice(friendlyName, instanceId, enumerator, busName)) {
                                currentBtIds.push_back(devId);
                                currentBtNames.push_back(friendlyName.empty() ? L"Bluetooth Audio Device" : friendlyName);
                            }
                        }
                    }

                    static std::vector<std::wstring> s_lastBtIds;
                    static bool s_firstBtPoll = true;

                    if (s_firstBtPoll) {
                        s_lastBtIds = currentBtIds;
                        s_firstBtPoll = false;
                    } else {
                        // Check for newly connected Bluetooth devices
                        for (size_t i = 0; i < currentBtIds.size(); i++) {
                            const auto& cid = currentBtIds[i];
                            if (std::find(s_lastBtIds.begin(), s_lastBtIds.end(), cid) == s_lastBtIds.end()) {
                                // Connect Event!
                                std::wstring name = currentBtNames[i];
                                Wh_Log(L"[DynamicIsland] Bluetooth audio device CONNECTED: name='%ls', id='%ls'", name.c_str(), cid.c_str());

                                std::lock_guard lock(g_stateMutex);
                                g_state.device.active        = true;
                                g_state.device.eventType     = DeviceEventType::Connected;
                                g_state.device.deviceName    = name;
                                g_state.device.isBluetoothLike = true;
                                g_state.device.expiresAt     = NowSeconds() + 5.0;
                                g_deviceConnectedAt          = NowSeconds();
                                g_suppressVolumeUntil        = NowSeconds() + 8.0;
                                g_lastBtName                 = name;
                                TriggerNudge();
                            }
                        }

                        // Check for disconnected Bluetooth devices
                        for (size_t i = 0; i < s_lastBtIds.size(); i++) {
                            const auto& lid = s_lastBtIds[i];
                            if (std::find(currentBtIds.begin(), currentBtIds.end(), lid) == currentBtIds.end()) {
                                // Disconnect Event!
                                std::wstring name = g_lastBtName.empty() ? L"Bluetooth Device" : g_lastBtName;
                                Wh_Log(L"[DynamicIsland] Bluetooth audio device DISCONNECTED: name='%ls', id='%ls'", name.c_str(), lid.c_str());

                                std::lock_guard lock(g_stateMutex);
                                g_state.device.active        = true;
                                g_state.device.eventType     = DeviceEventType::Disconnected;
                                g_state.device.deviceName    = name;
                                g_state.device.isBluetoothLike = true;
                                g_state.device.expiresAt     = NowSeconds() + 5.0;
                                g_deviceConnectedAt          = NowSeconds();
                                g_suppressVolumeUntil        = NowSeconds() + 8.0;
                                g_lastBtName                 = L"";
                                TriggerNudge();
                            }
                        }

                        s_lastBtIds = currentBtIds;
                    }
                }
            }
        }
    }
    // ── End endpoint detection ─────────────────────────────────────────────

    if (!s_volume) {
        ComPtr<IMMDeviceEnumerator> enumerator;
        ComPtr<IMMDevice> device;
        HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&enumerator));
        if (SUCCEEDED(hr)) {
            hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
        }
        if (SUCCEEDED(hr)) {
            hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(s_volume.GetAddressOf()));
        }
    }

    if (s_volume) {
        float level = 0.0f;
        BOOL muted = FALSE;
        if (SUCCEEDED(s_volume->GetMasterVolumeLevelScalar(&level)) && SUCCEEDED(s_volume->GetMute(&muted))) {
            next.volumePercent = ClampInt(static_cast<int>(level * 100.0f + 0.5f), 0, 100);
            next.volumeMuted = muted != FALSE;
        } else {
            s_volume.Reset(); // Retry next time
        }
    }



    std::lock_guard lock(g_stateMutex);
    const double nowSec = NowSeconds();
    // Suppress spurious volume jumps for 4 seconds after a BT device connects
    // (Windows switches the default audio endpoint, causing a false volume change)
    const bool suppressVolume = ((nowSec - g_deviceConnectedAt) < 8.0) || (nowSec < g_suppressVolumeUntil) || g_state.device.active;
    const bool volumeChanged =
        g_volumeInitialized && !suppressVolume &&
        (std::abs(next.volumePercent - g_state.system.volumePercent) >= 2 ||
         next.volumeMuted != g_state.system.volumeMuted);
    g_state.system = next;
    g_state.muted = next.volumeMuted;
    g_volumeInitialized = true;
}

// ---- Privacy indicator helpers ----
bool IsDeviceActiveViaRegistry(const wchar_t* capability) {
    bool isActive = false;
    std::wstring basePath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\CapabilityAccessManager\\ConsentStore\\";
    basePath += capability;

    auto CheckSubkeys = [](HKEY hKeyParent) -> bool {
        DWORD index = 0;
        wchar_t subKeyName[256];
        DWORD nameLen = ARRAYSIZE(subKeyName);
        while (RegEnumKeyExW(hKeyParent, index, subKeyName, &nameLen, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
            HKEY hSub;
            if (RegOpenKeyExW(hKeyParent, subKeyName, 0, KEY_READ, &hSub) == ERROR_SUCCESS) {
                if (_wcsicmp(subKeyName, L"NonPackaged") == 0) {
                    DWORD npIndex = 0;
                    wchar_t npSubKeyName[256];
                    DWORD npNameLen = ARRAYSIZE(npSubKeyName);
                    while (RegEnumKeyExW(hSub, npIndex, npSubKeyName, &npNameLen, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
                        HKEY hNpSub;
                        if (RegOpenKeyExW(hSub, npSubKeyName, 0, KEY_READ, &hNpSub) == ERROR_SUCCESS) {
                            uint64_t stopTime = 1;
                            DWORD dataSize = sizeof(stopTime);
                            if (RegQueryValueExW(hNpSub, L"LastUsedTimeStop", nullptr, nullptr, reinterpret_cast<LPBYTE>(&stopTime), &dataSize) == ERROR_SUCCESS) {
                                if (stopTime == 0) {
                                    RegCloseKey(hNpSub);
                                    RegCloseKey(hSub);
                                    return true;
                                }
                            }
                            RegCloseKey(hNpSub);
                        }
                        npIndex++;
                        npNameLen = ARRAYSIZE(npSubKeyName);
                    }
                } else {
                    uint64_t stopTime = 1;
                    DWORD dataSize = sizeof(stopTime);
                    if (RegQueryValueExW(hSub, L"LastUsedTimeStop", nullptr, nullptr, reinterpret_cast<LPBYTE>(&stopTime), &dataSize) == ERROR_SUCCESS) {
                        if (stopTime == 0) {
                            RegCloseKey(hSub);
                            return true;
                        }
                    }
                }
                RegCloseKey(hSub);
            }
            index++;
            nameLen = ARRAYSIZE(subKeyName);
        }
        return false;
    };

    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, basePath.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        isActive = CheckSubkeys(hKey);
        RegCloseKey(hKey);
    }
    
    if (!isActive) {
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, basePath.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            isActive = CheckSubkeys(hKey);
            RegCloseKey(hKey);
        }
    }
    
    return isActive;
}

bool IsMicrophoneActive() {
    return IsDeviceActiveViaRegistry(L"microphone");
}

bool IsCameraActive() {
    return IsDeviceActiveViaRegistry(L"webcam");
}

void UpdateProgressSnapshot() {
    const int progress = Wh_GetIntValue(L"ProgressPercent", -1);
    std::lock_guard lock(g_stateMutex);
    g_state.progress.active = progress >= 0 && progress <= 100;
    g_state.progress.percent = ClampInt(progress, 0, 100);
}

void UpdatePrivacyIndicators() {
    const bool mic = IsMicrophoneActive();
    const bool cam = IsCameraActive();
    std::lock_guard lock(g_stateMutex);
    g_state.system.micActive = mic;
    g_state.system.cameraActive = cam;
}

std::wstring ReadClipboardText(HWND hwnd) {
    std::wstring text;
    if (!OpenClipboard(hwnd)) {
        return text;
    }

    HANDLE data = GetClipboardData(CF_UNICODETEXT);
    if (data) {
        auto* locked = static_cast<const wchar_t*>(GlobalLock(data));
        if (locked) {
            text = locked;
            GlobalUnlock(data);
        }
    }

    CloseClipboard();
    return text;
}

bool ClipboardHasBitmap(HWND hwnd) {
    bool result = false;
    if (OpenClipboard(hwnd)) {
        result = IsClipboardFormatAvailable(CF_BITMAP) ||
                 IsClipboardFormatAvailable(CF_DIB) ||
                 IsClipboardFormatAvailable(CF_DIBV5);
        CloseClipboard();
    }
    return result;
}

bool IsLikelyToastWindow(HWND hwnd, const wchar_t* className, const wchar_t* title) {
    if (hwnd == g_hwnd || !hwnd) {
        return false;
    }

    if (GetWindow(hwnd, GW_OWNER)) {
        return false;
    }

    const std::wstring cls = ToLowerCopy(className ? className : L"");
    const std::wstring text = ToLowerCopy(title ? title : L"");

    // Classic Windows 10 toasts have clear class names
    if (cls.find(L"notification") != std::wstring::npos ||
        cls.find(L"toast") != std::wstring::npos ||
        cls.find(L"windows.ui.notifications") != std::wstring::npos) {
        return true;
    }

    // Windows 11 toasts use generic XAML or CoreWindow classes, usually hosted by
    // explorer.exe, sihost.exe, or ShellExperienceHost.exe.
    // Importantly, their title is often empty at the exact moment of creation!
    if (cls.find(L"xaml_windowedpopupclass") != std::wstring::npos ||
        cls.find(L"windows.ui.core.corewindow") != std::wstring::npos) {
        
        std::wstring image;
        if (ProcessImageNameForWindow(hwnd, &image)) {
            const std::wstring base = ToLowerCopy(BaseNameFromPath(image));
            if (base == L"explorer.exe" || base == L"sihost.exe" || base == L"shellexperiencehost.exe") {
                // Ensure it's not the start menu, search, or action center main panel
                if (text != L"start" && text != L"action center" && text != L"search" && text != L"task view") {
                    return true;
                }
            }
        }
    }

    return false;
}

void CaptureShellNotification(HWND hwnd) {
    // Grace period: ignore notifications that fire in the first 3 seconds after
    // the mod starts. sihost.exe gets injected while system windows are still
    // settling, which causes false positives (e.g. Snipping Tool windows).
    if (NowSeconds() < 3.0) {
        return;
    }

    wchar_t className[128] = {};
    wchar_t title[192] = {};
    GetClassNameW(hwnd, className, ARRAYSIZE(className));
    GetWindowTextW(hwnd, title, ARRAYSIZE(title));

    if (!IsLikelyToastWindow(hwnd, className, title)) {
        return;
    }

    NotificationSnapshot notification;
    notification.active = true;
    notification.app = L"Notification";
    notification.title = title;
    notification.expiresAt = NowSeconds() + 4.0;
    // Fetch a 64px icon to ensure crisp rendering inside the pill
    notification.icon = GetWindowIconPixels(hwnd, 64);

    if (notification.title.size() > 96) {
        notification.body = notification.title.substr(64);
        notification.title.resize(64);
        notification.title += L"...";
    }

    {
        std::lock_guard lock(g_stateMutex);
        g_state.notification = std::move(notification);
    }
    TriggerNudge();

    // Spawn a background thread to extract the full rich text body of the toast using UI Automation.
    // Modern Windows Toasts often only provide the App Name via GetWindowTextW, leaving the body hidden in the XAML tree.
    std::thread([hwnd]() {
        Sleep(400); // Give the heavy UWP XAML tree enough time to fully construct the text nodes
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (SUCCEEDED(hr)) {
            IUIAutomation* uia = nullptr;
            hr = CoCreateInstance(__uuidof(CUIAutomation), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&uia);
            if (SUCCEEDED(hr) && uia) {
                IUIAutomation2* uia2 = nullptr;
                if (SUCCEEDED(uia->QueryInterface(__uuidof(IUIAutomation2), (void**)&uia2)) && uia2) {
                    uia2->put_TransactionTimeout(500);
                    uia2->put_ConnectionTimeout(500);
                    uia2->Release();
                }
                IUIAutomationElement* windowEl = nullptr;
                if (SUCCEEDED(uia->ElementFromHandle(hwnd, &windowEl)) && windowEl) {
                    IUIAutomationCondition* cond = nullptr;
                    uia->CreateTrueCondition(&cond);
                    IUIAutomationElementArray* elements = nullptr;
                    if (SUCCEEDED(windowEl->FindAll(TreeScope_Descendants, cond, &elements)) && elements) {
                        int count = 0;
                        elements->get_Length(&count);
                        std::wstring appName;
                        std::wstring fullText;
                        for (int i = 0; i < count; ++i) {
                            IUIAutomationElement* el = nullptr;
                            if (SUCCEEDED(elements->GetElement(i, &el)) && el) {
                                BSTR name = nullptr;
                                el->get_CurrentName(&name);
                                if (name && wcslen(name) > 0) {
                                    std::wstring chunk = name;
                                    // Skip generic screen-reader labels often found in toasts
                                    if (chunk != L"Notification" && chunk != L"New notification") {
                                        if (appName.empty()) {
                                            appName = chunk;
                                        } else {
                                            if (!fullText.empty()) fullText += L"  -  ";
                                            fullText += chunk;
                                        }
                                    }
                                }
                                if (name) SysFreeString(name);
                                el->Release();
                            }
                        }
                        elements->Release();
                        
                        if (fullText.empty() && !appName.empty()) {
                            fullText = appName;
                            appName = L"Notification";
                        }
                        
                        if (!fullText.empty()) {
                            std::lock_guard lock(g_stateMutex);
                            if (g_state.notification.active) {
                                if (!appName.empty()) {
                                    g_state.notification.app = appName;
                                    if (appName != L"Notification") {
                                        BitmapPixels resolvedIcon = FindAppIconByName(appName, 64);
                                        if (!resolvedIcon.bgra.empty()) {
                                            g_state.notification.icon = std::move(resolvedIcon);
                                        }
                                    }
                                }
                                g_state.notification.title = fullText;
                            }
                        }
                    }
                    if (cond) cond->Release();
                    windowEl->Release();
                }
                uia->Release();
            }
            CoUninitialize();
        }
    }).detach();
}

void CaptureClipboard(HWND hwnd) {
    ClipboardSnapshot clip;
    clip.expiresAt = NowSeconds() + 4.0;
    HWND owner = GetClipboardOwner();
    if (!owner) {
        owner = GetForegroundWindow();
    }
    wchar_t ownerTitle[80] = {};
    if (owner) {
        GetWindowTextW(owner, ownerTitle, ARRAYSIZE(ownerTitle));
    }
    if (owner && !IsIgnorableForegroundWindow(owner, ownerTitle)) {
        DWORD pid = 0;
        GetWindowThreadProcessId(owner, &pid);
        // Fetch at 64px for crisp rendering — 18px/32px is often too small for icon APIs and returns empty.
        clip.appIcon = GetWindowIconPixels(owner, 64);

        clip.appName = ownerTitle;
        if (clip.appName.empty()) {
            std::wstring path;
            if (ProcessImageNameForPid(pid, &path)) {
                clip.appName = StripExtension(BaseNameFromPath(path));
            }
        }
        if (clip.appName.size() > 24) {
            clip.appName.resize(24);
            clip.appName += L"...";
        }
    }

    std::wstring text = ReadClipboardText(hwnd);
    if (!text.empty()) {
        constexpr size_t maxChars = 96;
        std::replace(text.begin(), text.end(), L'\r', L' ');
        std::replace(text.begin(), text.end(), L'\n', L' ');
        if (text.size() > maxChars) {
            text.resize(maxChars);
            text += L"...";
        }
        clip.text = text;
        clip.image = false;
        clip.active = true;
    } else if (ClipboardHasBitmap(hwnd)) {
        clip.text = L"Image copied";
        clip.image = true;
        clip.active = true;
    }

    if (clip.active) {
        {
            std::lock_guard lock(g_stateMutex);
            g_state.clipboard = std::move(clip);
        }
        TriggerNudge();
    }
}

void SetClickThrough(HWND hwnd, bool clickThrough) {
    LONG_PTR exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    const bool has = (exStyle & WS_EX_TRANSPARENT) != 0;
    if (clickThrough == has) {
        return;
    }

    if (clickThrough) {
        exStyle |= WS_EX_TRANSPARENT;
    } else {
        exStyle &= ~WS_EX_TRANSPARENT;
    }

    SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exStyle);
}

void SendVirtualKey(WORD vk) {
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vk;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vk;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, inputs, sizeof(INPUT));
}

void OpenRelevantApp();

void SendMediaCommandAtPoint(HWND hwnd, LPARAM lParam) {
    RECT rc = {};
    GetClientRect(hwnd, &rc);
    const int x = GET_X_LPARAM(lParam);
    const int y = GET_Y_LPARAM(lParam);

    // Media controls are drawn in the right side of the media pill. Hit-test
    // using client coordinates so this stays independent of monitor position.
    // Hit-test positions must match DrawMedia control layout:
    // cx = rect.right - 42, prev = cx-26, play = cx, next = cx+26
    // rect.right = rc.right - kRenderPadX, so offset from rc.right:
    const int centerY = (rc.bottom + rc.top) / 2;
    const int prevX = rc.right - static_cast<int>(kRenderPadX) - 68;
    const int playX = rc.right - static_cast<int>(kRenderPadX) - 42;
    const int nextX = rc.right - static_cast<int>(kRenderPadX) - 16;
    const int radius = 15;

    auto hit = [&](int cx) {
        const int dx = x - cx;
        const int dy = y - centerY;
        return dx * dx + dy * dy <= radius * radius;
    };

    if (hit(prevX)) {
        SendVirtualKey(VK_MEDIA_PREV_TRACK);
    } else if (hit(playX)) {
        SendVirtualKey(VK_MEDIA_PLAY_PAUSE);
    } else if (hit(nextX)) {
        SendVirtualKey(VK_MEDIA_NEXT_TRACK);
    } else {
        OpenRelevantApp();
    }
}

void ToggleEndpointMute();

void HandleStatusClickAtPoint(HWND hwnd, LPARAM lParam) {
    // Disabled click handlers for status chips as requested by the user
    return;
}

void ToggleEndpointMute() {
    ComPtr<IMMDeviceEnumerator> enumerator;
    ComPtr<IMMDevice> device;
    ComPtr<IAudioEndpointVolume> volume;

    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                                  CLSCTX_ALL, IID_PPV_ARGS(&enumerator));
    if (SUCCEEDED(hr)) {
        hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    }
    if (SUCCEEDED(hr)) {
        hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr,
                              reinterpret_cast<void**>(volume.GetAddressOf()));
    }
    if (SUCCEEDED(hr)) {
        BOOL muted = FALSE;
        volume->GetMute(&muted);
        volume->SetMute(!muted, nullptr);
        std::lock_guard lock(g_stateMutex);
        g_state.muted = !muted;
    }
}

struct WindowSearch {
    std::wstring targetTitle;
    HWND foundHwnd = nullptr;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    wchar_t title[512];
    if (GetWindowTextW(hwnd, title, ARRAYSIZE(title)) > 0) {
        auto* search = reinterpret_cast<WindowSearch*>(lParam);
        std::wstring wTitle(title);
        // Case-insensitive check if window title contains currently playing media title
        auto it = std::search(
            wTitle.begin(), wTitle.end(),
            search->targetTitle.begin(), search->targetTitle.end(),
            [](wchar_t ch1, wchar_t ch2) { return towlower(ch1) == towlower(ch2); }
        );

        if (it != wTitle.end()) {
            search->foundHwnd = hwnd;
            return FALSE; // found, stop enumerating
        }
    }
    return TRUE;
}

void OpenRelevantApp() {
    std::wstring title;
    std::wstring app;
    {
        std::lock_guard lock(g_stateMutex);
        title = g_state.media.title;
        app = g_state.media.sourceAppUserModelId;
    }

    // Try to find and focus window containing track title (ideal for browser playing YouTube/Spotify)
    if (!title.empty()) {
        WindowSearch search;
        search.targetTitle = title;
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&search));

        if (search.foundHwnd) {
            if (IsIconic(search.foundHwnd)) {
                ShowWindow(search.foundHwnd, SW_RESTORE);
            }
            SetForegroundWindow(search.foundHwnd);
            return;
        }
    }

    // Fallback: Launch or focus via AppUserModelId or Path
    if (!app.empty()) {
        std::wstring executePath = app;
        
        // Remove surrounding quotes if any
        if (executePath.size() >= 2 && executePath.front() == L'"' && executePath.back() == L'"') {
            executePath = executePath.substr(1, executePath.size() - 2);
        }

        bool isFilePath = (executePath.find(L":\\") != std::wstring::npos || 
                           (executePath.size() >= 4 && executePath.substr(executePath.size() - 4) == L".exe"));

        if (isFilePath) {
            if (GetFileAttributesW(executePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
                // Path doesn't exist. Try 64-bit Program Files if it was in x86
                size_t x86Pos = executePath.find(L" (x86)");
                if (x86Pos != std::wstring::npos) {
                    std::wstring altPath = executePath;
                    altPath.erase(x86Pos, 6);
                    if (GetFileAttributesW(altPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                        executePath = altPath;
                    }
                }
                
                // If it STILL doesn't exist after trying alternatives, just gracefully abort!
                // Trying to guess 'brave.exe' triggers broken Windows Registry App Paths.
                if (GetFileAttributesW(executePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
                    return;
                }
            }
            
            SHELLEXECUTEINFOW sei = { sizeof(sei) };
            sei.fMask = SEE_MASK_FLAG_NO_UI;
            sei.lpFile = executePath.c_str();
            sei.nShow = SW_SHOWNORMAL;
            ShellExecuteExW(&sei);
        } else {
            // It's a UWP/Desktop AppUserModelId, launch via AppsFolder
            std::wstring shellPath = L"shell:AppsFolder\\" + executePath;
            SHELLEXECUTEINFOW sei = { sizeof(sei) };
            sei.fMask = SEE_MASK_FLAG_NO_UI;
            sei.lpFile = shellPath.c_str();
            sei.nShow = SW_SHOWNORMAL;
            ShellExecuteExW(&sei);
        }
        return;
    }

    ShellExecuteW(nullptr, L"open", L"ms-settings:", nullptr, nullptr, SW_SHOWNORMAL);
}

void DismissTransientState() {
    std::lock_guard lock(g_stateMutex);
    g_state.clipboard.active = false;
    g_state.notification.active = false;
    g_state.volume.active = false;
    g_state.progress.active = false;
    Wh_SetIntValue(L"ProgressPercent", -1);
}

void ShowContextMenu(HWND hwnd, POINT screenPoint) {
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, 1, L"Dismiss");
    AppendMenuW(menu, MF_STRING, 2, L"Pin expanded");
    AppendMenuW(menu, MF_STRING, 3, Wh_GetIntValue(L"GameOverlayPinned", 0) ? L"Hide game overlay" : L"Show game overlay");
    const int activeW11 = Wh_GetIntValue(L"W11StyleOverride", -1) >= 0
                          ? Wh_GetIntValue(L"W11StyleOverride", 0)
                          : Wh_GetIntSetting(L"Appearance.W11Style");
    AppendMenuW(menu, MF_STRING, 10, activeW11 ? L"Use iPhone Pill Style" : L"Use Windows 11 Flyout Style");
    const int activeExpandOnHover = Wh_GetIntValue(L"ExpandOnHoverOverride", -1) >= 0
                          ? Wh_GetIntValue(L"ExpandOnHoverOverride", 0)
                          : Wh_GetIntSetting(L"Appearance.ExpandOnHover");
    AppendMenuW(menu, MF_STRING, 11, activeExpandOnHover ? L"Expand on Click" : L"Expand on Hover");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, 4, L"Transparency 100%");
    AppendMenuW(menu, MF_STRING, 5, L"Transparency 85%");
    AppendMenuW(menu, MF_STRING, 6, L"Transparency 70%");
    AppendMenuW(menu, MF_STRING, 7, L"Transparency 55%");
    AppendMenuW(menu, MF_STRING, 8, L"Reset transparency");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    // Color theme presets
    AppendMenuW(menu, MF_STRING, 20, L"Theme: Apple Style");
    AppendMenuW(menu, MF_STRING, 36, L"Theme: OLED Black");
    AppendMenuW(menu, MF_STRING, 21, L"Theme: Dark Gray");
    AppendMenuW(menu, MF_STRING, 22, L"Theme: Midnight Blue");
    AppendMenuW(menu, MF_STRING, 23, L"Theme: Deep Purple");
    AppendMenuW(menu, MF_STRING, 24, L"Theme: Fluent Design");
    AppendMenuW(menu, MF_STRING, 30, L"Theme: Nothing OS");
    AppendMenuW(menu, MF_STRING, 31, L"Theme: Glassmorphism");
    AppendMenuW(menu, MF_STRING, 32, L"Theme: Cyberpunk");
    AppendMenuW(menu, MF_STRING, 33, L"Theme: Material You");
    AppendMenuW(menu, MF_STRING, 34, L"Theme: AMOLED Dark");
    AppendMenuW(menu, MF_STRING, 35, L"Theme: Holographic AI");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    // Quick actions
    const bool nightModeActive = (Wh_GetIntValue(L"PillOpacityOverride", -1) == 40);
    AppendMenuW(menu, MF_STRING | (nightModeActive ? MF_CHECKED : 0), 27, nightModeActive ? L"Night Mode \u2713" : L"Night Mode");
    AppendMenuW(menu, MF_STRING, 25, L"Refresh Weather Now");
    AppendMenuW(menu, MF_STRING, 26, L"Reset All Overrides");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
#ifdef STANDALONE_APP
    AppendMenuW(menu, MF_STRING, 9, L"Settings");
    AppendMenuW(menu, MF_STRING, 99, L"Exit");
#else
    AppendMenuW(menu, MF_STRING, 9, L"Open Windhawk settings");
#endif

    SetForegroundWindow(hwnd);
    const UINT cmd = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON,
                                   screenPoint.x, screenPoint.y, 0, hwnd, nullptr);
    DestroyMenu(menu);

    switch (cmd) {
        case 1:
            DismissTransientState();
            break;
        case 2:
            Wh_SetIntValue(L"PinnedExpanded", Wh_GetIntValue(L"PinnedExpanded", 0) ? 0 : 1);
            break;
        case 3:
            Wh_SetIntValue(L"GameOverlayPinned", Wh_GetIntValue(L"GameOverlayPinned", 0) ? 0 : 1);
            break;
        case 4:
            Wh_SetIntValue(L"PillOpacityOverride", 100);
            LoadSettings();
            break;
        case 5:
            Wh_SetIntValue(L"PillOpacityOverride", 85);
            LoadSettings();
            break;
        case 6:
            Wh_SetIntValue(L"PillOpacityOverride", 70);
            LoadSettings();
            break;
        case 7:
            Wh_SetIntValue(L"PillOpacityOverride", 55);
            LoadSettings();
            break;
        case 8:
            Wh_SetIntValue(L"PillOpacityOverride", -1);
            LoadSettings();
            break;
        case 9: {
#ifdef STANDALONE_APP
            ::OpenSettingsDialog(hwnd);
#else
            wchar_t currentProcessPath[MAX_PATH] = {};
            GetModuleFileNameW(nullptr, currentProcessPath, ARRAYSIZE(currentProcessPath));

            HINSTANCE result = ShellExecuteW(nullptr, L"open",
                                             currentProcessPath,
                                             nullptr,
                                             nullptr, SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(result) <= 32) {
                Wh_Log(L"Failed to open Windhawk settings.");
            }
#endif
            break;
        }
#ifdef STANDALONE_APP
        case 99:
            SetEvent(g_stopEvent);
            break;
#endif
        case 10: {
            const int activeW11Val = Wh_GetIntValue(L"W11StyleOverride", -1) >= 0
                                  ? Wh_GetIntValue(L"W11StyleOverride", 0)
                                  : Wh_GetIntSetting(L"Appearance.W11Style");
            Wh_SetIntValue(L"W11StyleOverride", activeW11Val ? 0 : 1);
            LoadSettings();
            g_layoutDirty = true;
            break;
        }
        case 11: {
            const int activeExpandOnHover = Wh_GetIntValue(L"ExpandOnHoverOverride", -1) >= 0
                                  ? Wh_GetIntValue(L"ExpandOnHoverOverride", 0)
                                  : Wh_GetIntSetting(L"Appearance.ExpandOnHover");
            Wh_SetIntValue(L"ExpandOnHoverOverride", activeExpandOnHover ? 0 : 1);
            LoadSettings();
            g_layoutDirty = true;
            break;
        }
        // Color theme presets — stored as integer index in local mod storage.
         case 20:  // Apple Style
             Wh_SetIntValue(L"ColorTheme", 0);
             LoadSettings();
             break;
         case 36:  // OLED Black
             Wh_SetIntValue(L"ColorTheme", 1);
             LoadSettings();
             break;
         case 21:  // Dark Gray
             Wh_SetIntValue(L"ColorTheme", 2);
             LoadSettings();
             break;
         case 22:  // Midnight Blue
             Wh_SetIntValue(L"ColorTheme", 3);
             LoadSettings();
             break;
         case 23:  // Deep Purple
             Wh_SetIntValue(L"ColorTheme", 4);
             LoadSettings();
             break;
         case 24:  // Fluent Design
             Wh_SetIntValue(L"ColorTheme", 5);
             LoadSettings();
             break;
         case 30:  // Nothing OS
             Wh_SetIntValue(L"ColorTheme", 6);
             LoadSettings();
             break;
         case 31:  // Glassmorphism
             Wh_SetIntValue(L"ColorTheme", 7);
             LoadSettings();
             break;
         case 32:  // Cyberpunk
             Wh_SetIntValue(L"ColorTheme", 8);
             LoadSettings();
             break;
         case 33:  // Material You
             Wh_SetIntValue(L"ColorTheme", 9);
             LoadSettings();
             break;
         case 34:  // AMOLED Dark
             Wh_SetIntValue(L"ColorTheme", 10);
             LoadSettings();
             break;
         case 35:  // Holographic AI
             Wh_SetIntValue(L"ColorTheme", 11);
             LoadSettings();
             break;
        case 25:  // Q5: Refresh Weather Now
            // Signal the weather thread to refresh immediately by resetting
            // the last-updated timestamp so the next poll triggers a fetch.
            {
                std::lock_guard lock(g_stateMutex);
                g_state.weather.lastUpdated = 0.0;
            }
            SetEvent(g_settingsChangedEvent);
            break;
        case 26:  // Q6: Reset All Overrides
            Wh_SetIntValue(L"PillOpacityOverride", -1);
            Wh_SetIntValue(L"W11StyleOverride", -1);
            Wh_SetIntValue(L"ExpandOnHoverOverride", -1);
            Wh_SetIntValue(L"ColorTheme", -1);
            Wh_SetIntValue(L"PinnedExpanded", 0);
            Wh_SetIntValue(L"GameOverlayPinned", 0);
            LoadSettings();
            g_layoutDirty = true;
            break;
        case 27:  // Q7: Night Mode toggle
            if (nightModeActive) {
                Wh_SetIntValue(L"PillOpacityOverride", -1);
            } else {
                Wh_SetIntValue(L"PillOpacityOverride", 40);
            }
            LoadSettings();
            break;
    }
}


// --- Global Timer Variables ---
static bool g_timerActive = false;
static ULONGLONG g_timerTargetTick = 0;
static ULONGLONG g_timerDurationMs = 0;
static bool g_timerPaused = false;
static ULONGLONG g_timerPauseRemainingMs = 0;
static bool g_timerAlertActive = false;
static ULONGLONG g_timerAlertStartTick = 0;

// --- Customizable Tools Tray Dashboard ---
#include "dynamic_island_ui.h"

const ToolItem kAllTools[] = {
    { L"calc", L"Calculator", L"\xD83E\xDDEE", L"calc.exe" },
    { L"notepad", L"Notes", L"\xD83D\xDCDD", L"notepad.exe" },
    { L"clipboard", L"Clipboard", L"\xD83D\xDCCB", L"clipboard_history" },
    { L"screenshot", L"Screenshot", L"\xD83D\xDCF8", L"explorer.exe ms-screenclip:" },
    { L"explorer", L"Explorer", L"\xD83D\xDCC1", L"explorer.exe" },
    { L"settings", L"Settings", L"\x2699\xFE0F", L"settings_dialog" },
    { L"taskmgr", L"Task Mgr", L"\xD83D\xDCCA", L"taskmgr.exe" },
    { L"cmd", L"Terminal", L"\xD83D\xDCBB", L"cmd.exe" },
    { L"browser", L"Browser", L"\xD83C\xDF10", L"explorer.exe http://www.google.com" },
    { L"timer", L"Set Timer", L"\x23F1\xFE0F", L"explorer.exe ms-clock:" }
};

std::vector<std::wstring> g_activeTools = { L"calc", L"notepad", L"clipboard", L"screenshot", L"explorer", L"settings", L"taskmgr", L"cmd", L"browser" };
bool g_toolsEditMode = false;

const ToolItem* FindToolById(const std::wstring& toolId) {
    for (const auto& t : kAllTools) {
        if (t.id == toolId) return &t;
    }
    return nullptr;
}

static void LoadToolsList() {
    wchar_t buffer[512] = {};
    GetPrivateProfileStringW(L"Settings", L"Modules.ToolsTrayList", L"calc,notepad,clipboard,screenshot,explorer,settings,taskmgr,cmd,browser", buffer, 512, GetIniPath().c_str());
    g_activeTools.clear();
    std::wstring s = buffer;
    size_t pos = 0;
    while ((pos = s.find(L",")) != std::wstring::npos) {
        std::wstring token = s.substr(0, pos);
        if (!token.empty()) g_activeTools.push_back(token);
        s.erase(0, pos + 1);
    }
    if (!s.empty()) g_activeTools.push_back(s);
}

static void SaveToolsList() {
    std::wstring s;
    for (size_t i = 0; i < g_activeTools.size(); ++i) {
        if (i > 0) s += L",";
        s += g_activeTools[i];
    }
    WritePrivateProfileStringW(L"Settings", L"Modules.ToolsTrayList", s.c_str(), GetIniPath().c_str());
}


// UI drawing code already included above

Activity ActivityForKind(IslandKind kind, const Settings& settings, const SharedState& state) {
    Activity activity;
    activity.kind = kind;

    switch (kind) {
        case IslandKind::Media:
            activity.width = 150.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::Progress:
            activity.width = 150.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::Clipboard:
            activity.width = 180.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::Notification:
            activity.width = 180.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::Volume:
            activity.width = 170.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::BatteryLow:
            activity.width = 150.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::CapsLock:
            activity.width = 150.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::Device:
            if (settings.airpodsMode == AirPodsMode::BiggerOnly) {
                activity.width = 280.0f;
                activity.height = 64.0f;
            } else {
                activity.width = 180.0f;
                activity.height = 36.0f;
            }
            break;
        case IslandKind::Timer:
            activity.width = 190.0f;
            activity.height = 36.0f;
            break;
        case IslandKind::Idle:
        default:
            if (settings.autoHideIdleSeconds == -1 && !state.system.micActive && !state.system.cameraActive) {
                activity.width = 0.0f;
                activity.height = 0.0f;
            } else {
                activity.width = 170.0f;
                activity.height = 36.0f;
            }
            break;
    }

    activity.width *= settings.sizeScale;
    activity.height *= settings.sizeScale;
    return activity;
}

std::vector<IslandKind> ChooseActivities(const SharedState& state, const Settings& settings, double now) {
    std::vector<IslandKind> activities;

    if (state.clipboard.active && now < state.clipboard.expiresAt) {
        activities.push_back(IslandKind::Clipboard);
    }
    if (state.capsLock.active && now < state.capsLock.expiresAt) {
        activities.push_back(IslandKind::CapsLock);
    }
    if (state.device.active && now < state.device.expiresAt) {
        activities.push_back(IslandKind::Device);
    }
    if (state.volume.active && now < state.volume.expiresAt) {
        activities.push_back(IslandKind::Volume);
    }
    if (state.notification.active && now < state.notification.expiresAt) {
        activities.push_back(IslandKind::Notification);
    }
    if (settings.battery && state.battery.active && now < state.battery.expiresAt) {
        activities.push_back(IslandKind::BatteryLow);
    }
    if (settings.progress && state.progress.active) {
        activities.push_back(IslandKind::Progress);
    }
    if (g_timerActive) {
        activities.push_back(IslandKind::Timer);
    }
    if (settings.media && state.media.available) {
        activities.push_back(IslandKind::Media);
    }

    if (activities.empty()) {
        activities.push_back(IslandKind::Idle);
    }

    return activities;
}

constexpr UINT WM_APP_CAPSLOCK = WM_APP + 0x444;
HHOOK g_keyboardHook = nullptr;
HANDLE g_keyboardThread = nullptr;
DWORD g_keyboardThreadId = 0;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            auto* kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            if (kbd->vkCode == VK_CAPITAL || kbd->vkCode == VK_NUMLOCK) {
                PostMessageW(g_hwnd, WM_APP_CAPSLOCK, kbd->vkCode, 0);
            }
        }
    }
    return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardThreadProc(void*) {
    g_keyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    if (g_keyboardHook) {
        UnhookWindowsHookEx(g_keyboardHook);
        g_keyboardHook = nullptr;
    }
    return 0;
}

static int g_tpSelectedHours = 0;
static int g_tpSelectedMinutes = 5;
static int g_tpSelectedSeconds = 0;
static bool g_tpOk = false;
static bool g_hoverStart = false;
static bool g_hoverCancel = false;
static bool g_hoverClose = false;

LRESULT CALLBACK TimerPickerWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Apply rounded corners regional clip
            HRGN rgn = CreateRoundRectRgn(0, 0, 280, 180, 16, 16);
            SetWindowRgn(hwnd, rgn, TRUE);

            int x = 20, y = 68, w = 70;

            HWND cbH = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP, x, y, w, 200, hwnd, (HMENU)101, nullptr, nullptr);
            SendMessageW(cbH, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            for (int i = 0; i <= 23; ++i) {
                wchar_t buf[16]; swprintf_s(buf, L"%02d", i);
                SendMessageW(cbH, CB_ADDSTRING, 0, (LPARAM)buf);
            }
            SendMessageW(cbH, CB_SETCURSEL, g_tpSelectedHours, 0);

            x += w + 15;
            HWND cbM = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP, x, y, w, 200, hwnd, (HMENU)102, nullptr, nullptr);
            SendMessageW(cbM, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            for (int i = 0; i <= 59; ++i) {
                wchar_t buf[16]; swprintf_s(buf, L"%02d", i);
                SendMessageW(cbM, CB_ADDSTRING, 0, (LPARAM)buf);
            }
            SendMessageW(cbM, CB_SETCURSEL, g_tpSelectedMinutes, 0);

            x += w + 15;
            HWND cbS = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP, x, y, w, 200, hwnd, (HMENU)103, nullptr, nullptr);
            SendMessageW(cbS, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            for (int i = 0; i <= 59; ++i) {
                wchar_t buf[16]; swprintf_s(buf, L"%02d", i);
                SendMessageW(cbS, CB_ADDSTRING, 0, (LPARAM)buf);
            }
            SendMessageW(cbS, CB_SETCURSEL, g_tpSelectedSeconds, 0);
            break;
        }
        case WM_NCHITTEST: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            POINT ptClient = pt;
            ScreenToClient(hwnd, &ptClient);
            // Draggable in header area (y < 40), excluding the close X button region
            if (ptClient.y < 40 && ptClient.x < 240) {
                return HTCAPTION;
            }
            return HTCLIENT;
        }
        case WM_MOUSEMOVE: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            RECT rcStart = {20, 115, 130, 147};
            RECT rcCancel = {150, 115, 260, 147};
            RECT rcClose = {250, 8, 272, 28};

            bool prevStart = g_hoverStart;
            bool prevCancel = g_hoverCancel;
            bool prevClose = g_hoverClose;

            g_hoverStart = PtInRect(&rcStart, pt) != FALSE;
            g_hoverCancel = PtInRect(&rcCancel, pt) != FALSE;
            g_hoverClose = PtInRect(&rcClose, pt) != FALSE;

            if (g_hoverStart != prevStart || g_hoverCancel != prevCancel || g_hoverClose != prevClose) {
                InvalidateRect(hwnd, nullptr, FALSE);
            }

            TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0};
            TrackMouseEvent(&tme);
            break;
        }
        case WM_MOUSELEAVE: {
            g_hoverStart = false;
            g_hoverCancel = false;
            g_hoverClose = false;
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        }
        case WM_LBUTTONDOWN: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            RECT rcStart = {20, 115, 130, 147};
            RECT rcCancel = {150, 115, 260, 147};
            RECT rcClose = {250, 8, 272, 28};

            if (PtInRect(&rcStart, pt)) {
                HWND cbH = GetDlgItem(hwnd, 101);
                HWND cbM = GetDlgItem(hwnd, 102);
                HWND cbS = GetDlgItem(hwnd, 103);
                g_tpSelectedHours = static_cast<int>(SendMessage(cbH, CB_GETCURSEL, 0, 0));
                g_tpSelectedMinutes = static_cast<int>(SendMessage(cbM, CB_GETCURSEL, 0, 0));
                g_tpSelectedSeconds = static_cast<int>(SendMessage(cbS, CB_GETCURSEL, 0, 0));
                g_tpOk = true;
                DestroyWindow(hwnd);
            } else if (PtInRect(&rcCancel, pt) || PtInRect(&rcClose, pt)) {
                g_tpOk = false;
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(220, 220, 220));
            SetBkColor(hdc, RGB(36, 36, 36));
            static HBRUSH hBrush = CreateSolidBrush(RGB(36, 36, 36));
            return (INT_PTR)hBrush;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Dark Charcoal background
            HBRUSH bgBrush = CreateSolidBrush(RGB(22, 22, 22));
            FillRect(hdc, &ps.rcPaint, bgBrush);
            DeleteObject(bgBrush);

            HFONT oldFont = (HFONT)SelectObject(hdc, g_hFont);
            SetBkMode(hdc, TRANSPARENT);

            // Title: "Set a Timer"
            SetTextColor(hdc, RGB(255, 255, 255));
            TextOutW(hdc, 20, 15, L"Set a Timer", 11);

            // Close (X) button
            if (g_hoverClose) {
                SetTextColor(hdc, RGB(255, 100, 100));
            } else {
                SetTextColor(hdc, RGB(140, 140, 140));
            }
            TextOutW(hdc, 254, 12, L"✕", 1);

            // Column Labels
            SetTextColor(hdc, RGB(160, 160, 160));
            TextOutW(hdc, 20, 46, L"Hours", 5);
            TextOutW(hdc, 105, 46, L"Minutes", 7);
            TextOutW(hdc, 190, 46, L"Seconds", 7);

            // Custom "Start Timer" button (Orange)
            HBRUSH btnStartBrush = CreateSolidBrush(g_hoverStart ? RGB(255, 155, 30) : RGB(255, 133, 0));
            HPEN nullPen = CreatePen(PS_NULL, 0, 0);
            HPEN oldPen = (HPEN)SelectObject(hdc, nullPen);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, btnStartBrush);
            RoundRect(hdc, 20, 115, 130, 147, 8, 8);

            // Custom "Cancel" button (Grey)
            HBRUSH btnCancelBrush = CreateSolidBrush(g_hoverCancel ? RGB(70, 70, 70) : RGB(45, 45, 45));
            SelectObject(hdc, btnCancelBrush);
            RoundRect(hdc, 150, 115, 260, 147, 8, 8);

            // Cleanup brushes/pens
            SelectObject(hdc, oldBrush);
            DeleteObject(btnStartBrush);
            DeleteObject(btnCancelBrush);
            SelectObject(hdc, oldPen);
            DeleteObject(nullPen);

            // Button Texts
            SetTextColor(hdc, RGB(255, 255, 255));
            RECT rTextStart = {20, 115, 130, 147};
            DrawTextW(hdc, L"Start Timer", -1, &rTextStart, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            RECT rTextCancel = {150, 115, 260, 147};
            DrawTextW(hdc, L"Cancel", -1, &rTextCancel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(hdc, oldFont);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

bool ShowTimerPickerModal(HWND parent) {
    WNDCLASSEXW wcx = {sizeof(WNDCLASSEXW)};
    wcx.lpfnWndProc = TimerPickerWndProc;
    wcx.hInstance = GetModuleHandle(nullptr);
    wcx.lpszClassName = L"DynamicIslandTimerPickerClass";
    wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcx.hbrBackground = nullptr;
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassExW(&wcx);

    int width = 280, height = 180;
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenW - width) / 2;
    int y = (screenH - height) / 2;

    g_tpOk = false;
    HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, L"DynamicIslandTimerPickerClass", L"Set a Timer",
                                WS_POPUP, x, y, width, height,
                                nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnregisterClassW(L"DynamicIslandTimerPickerClass", GetModuleHandle(nullptr));
    return g_tpOk;
}


LRESULT CALLBACK OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            AddClipboardFormatListener(hwnd);
            RegisterShellHookWindow(hwnd);
            return 0;

        case WM_DESTROY:
            RemoveClipboardFormatListener(hwnd);
            DeregisterShellHookWindow(hwnd);
            return 0;

        case WM_APP_CAPSLOCK: {
            bool isNum = (wParam == VK_NUMLOCK);
            bool capsOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
            bool numOn = (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
            {
                std::lock_guard lock(g_stateMutex);
                g_state.capsLock.active = true;
                g_state.capsLock.capsOn = capsOn;
                g_state.capsLock.numOn = numOn;
                g_state.capsLock.isNumEvent = isNum;
                g_state.capsLock.expiresAt = NowSeconds() + 2.5;
            }
            TriggerNudge();
            return 0;
        }

        case WM_DEVICECHANGE: {
            // DBT_DEVICEARRIVAL = 0x8000, DBT_DEVICEREMOVECOMPLETE = 0x8004
            if (wParam == 0x8000 || wParam == 0x8004) {
                bool arrived = (wParam == 0x8000);
                std::wstring devName;
                bool isBt = false;

                if (lParam) {
                    auto* hdr = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
                    if (hdr->dbch_devicetype == DBT_DEVTYP_VOLUME) {
                        devName = L"USB Drive";
                    } else if (hdr->dbch_devicetype == DBT_DEVTYP_PORT) {
                        devName = L"COM Device";
                    } else {
                        // Bluetooth / OEM — show notification immediately with fallback name,
                        // then asynchronously query the real audio endpoint friendly name
                        isBt = true;
                        devName = L"Bluetooth Device";
                    }
                }

                {
                    std::lock_guard lock(g_stateMutex);
                    g_state.device.active = true;
                    g_state.device.eventType = arrived ? DeviceEventType::Connected
                                                       : DeviceEventType::Disconnected;
                    g_state.device.deviceName = (!arrived && isBt && !g_lastBtName.empty()) ? g_lastBtName : devName;
                    g_state.device.isBluetoothLike = isBt;
                    g_state.device.expiresAt = NowSeconds() + 5.0;
                    if (isBt) {
                        // Mark timestamp so volume polling suppresses the spurious
                        // volume-change that fires when Windows switches audio endpoints
                        g_deviceConnectedAt = NowSeconds();
                        g_suppressVolumeUntil = NowSeconds() + 8.0;
                    }
                }
                TriggerNudge();

                // Asynchronously resolve the real Bluetooth device name from the
                // new default audio endpoint (which BT headphones become on connect)
                if (arrived && isBt) {
                    std::thread([] {
                        // Wait for Windows to settle the new default audio endpoint
                        Sleep(800);
                        CoInitializeEx(nullptr, COINIT_MULTITHREADED);

                        std::wstring resolvedName;
                        ComPtr<IMMDeviceEnumerator> devEnum;
                        HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                            CLSCTX_ALL, IID_PPV_ARGS(&devEnum));
                        if (SUCCEEDED(hr)) {
                            ComPtr<IMMDevice> audioDevice;
                            hr = devEnum->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);
                            if (SUCCEEDED(hr)) {
                                ComPtr<IPropertyStore> props;
                                hr = audioDevice->OpenPropertyStore(STGM_READ, &props);
                                if (SUCCEEDED(hr)) {
                                    PROPVARIANT pv;
                                    PropVariantInit(&pv);
                                    // PKEY_Device_FriendlyName GUID
                                    static const PROPERTYKEY PKEY_FriendlyName = {
                                        {0xa45c254e, 0xdf1c, 0x4efd,
                                         {0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0}}, 14};
                                    if (SUCCEEDED(props->GetValue(PKEY_FriendlyName, &pv)) &&
                                        pv.vt == VT_LPWSTR && pv.pwszVal && pv.pwszVal[0]) {
                                        resolvedName = pv.pwszVal;
                                    }
                                    PropVariantClear(&pv);
                                }
                            }
                        }
                        CoUninitialize();

                        if (!resolvedName.empty()) {
                            std::lock_guard lock(g_stateMutex);
                            // Only update if the device notification is still active
                            if (g_state.device.active && g_state.device.isBluetoothLike) {
                                g_state.device.deviceName = resolvedName;
                            }
                        }
                        TriggerNudge();
                    }).detach();
                }
            } // end if (wParam == 0x8000 || wParam == 0x8004)
            return 0;
        }

        case WM_CLIPBOARDUPDATE:
            if (g_settings.clipboard) {
                CaptureClipboard(hwnd);
            }
            return 0;



        case WM_APP_LAYOUT_CHANGED:
            g_layoutDirty = true;
            return 0;

        case WM_LBUTTONDOWN:
            {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                
                bool mediaActive = false;
                {
                    std::lock_guard lock(g_stateMutex);
                    mediaActive = g_settings.media && g_state.media.available;
                }

                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                const float height = static_cast<float>(clientRect.bottom - clientRect.top);
                const float width = static_cast<float>(clientRect.right - clientRect.left);

                if (mediaActive && height > 60.0f && (g_idleTab % 4) == 0) {
                    float totalScale = (GetDpiForWindow(hwnd) / 96.0f) * g_settings.sizeScale;
                    float cx = width / 2.0f;
                    float cy = height / 2.0f;
                    
                    float unX = (xPos - cx) / totalScale;
                    float unY = (yPos - cy) / totalScale;

                    if (unY > 56.0f - 30.0f && unY < 56.0f + 30.0f) {
                        int cmd = -1;
                        // 4-button layout: Back=-81, Play=-27, Forward=+27, AirPlay=+81
                        if      (unX > -81.0f - 20.0f && unX < -81.0f + 20.0f) cmd = 0; // Prev
                        else if (unX > -27.0f - 26.0f && unX < -27.0f + 26.0f) cmd = 1; // Play/Pause
                        else if (unX >  27.0f - 20.0f && unX <  27.0f + 20.0f) cmd = 2; // Next
                        else if (unX >  81.0f - 20.0f && unX <  81.0f + 20.0f) cmd = 3; // AirPlay (no-op)

                        if (cmd != -1) {
                            g_pressedMediaButton = cmd;
                            SetCapture(hwnd);
                            g_layoutDirty = true;
                            return 0;
                        }
                    }
                }
            }
            break;

        case WM_LBUTTONUP:
            {
                if (g_pressedMediaButton.load() != -1) {
                    g_pressedMediaButton = -1;
                    ReleaseCapture();
                    g_layoutDirty = true;
                }

                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                
                bool expanded = Wh_GetIntValue(L"PinnedExpanded", 0) != 0 || g_clickExpanded.load();
                if (!g_settings.expandOnHover && !expanded) {
                    g_clickExpanded = true;
                    g_layoutDirty = true;
                    return 0; // consumed click to expand
                }
                
                bool mediaActive = false;
                std::vector<IslandKind> kinds;
                {
                    std::lock_guard lock(g_stateMutex);
                    mediaActive = g_settings.media && g_state.media.available;
                    kinds = ChooseActivities(g_state, g_settings, NowSeconds());
                }

                if (g_timerAlertActive) {
                    g_timerActive = false;
                    g_timerAlertActive = false;
                    g_layoutDirty = true;
                    return 0;
                }

                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                const float height = static_cast<float>(clientRect.bottom - clientRect.top);
                const float width = static_cast<float>(clientRect.right - clientRect.left);

                float totalScale = (GetDpiForWindow(hwnd) / 96.0f) * g_settings.sizeScale;
                float cx = width / 2.0f;
                float cy = height / 2.0f;
                float unX = (xPos - cx) / totalScale;
                float unY = (yPos - cy) / totalScale;
                float unW = width / totalScale;
                float unH = height / totalScale;

                if (!kinds.empty() && kinds[0] == IslandKind::Timer && unH > 40.0f) {
                    float clickX = unX + unW * 0.5f;
                    float clickY = unY + unH * 0.5f;

                    // Subtract render padding (kRenderPadX = 28.0f, kRenderPadY = 22.0f)
                    float pillX = clickX - 28.0f;
                    float pillY = clickY - 22.0f;

                    // A. Check Pause/Resume Button (center x = 36.0f, y = unH * 0.5f, radius = 22.0f)
                    float distPauseSq = (pillX - 36.0f) * (pillX - 36.0f) + (pillY - (unH * 0.5f)) * (pillY - (unH * 0.5f));
                    if (distPauseSq <= 22.0f * 22.0f) {
                        if (g_timerPaused) {
                            g_timerTargetTick = GetTickCount64() + g_timerPauseRemainingMs;
                            g_timerPaused = false;
                        } else {
                            g_timerPaused = true;
                            ULONGLONG nowTick = GetTickCount64();
                            g_timerPauseRemainingMs = (g_timerTargetTick > nowTick) ? (g_timerTargetTick - nowTick) : 0;
                        }
                        g_layoutDirty = true;
                        return 0;
                    }

                    // B. Check Cancel Button (center x = 92.0f, y = unH * 0.5f, radius = 22.0f)
                    float distCancelSq = (pillX - 92.0f) * (pillX - 92.0f) + (pillY - (unH * 0.5f)) * (pillY - (unH * 0.5f));
                    if (distCancelSq <= 22.0f * 22.0f) {
                        g_timerActive = false;
                        g_timerAlertActive = false;
                        g_layoutDirty = true;
                        return 0;
                    }
                }

                float pillRight = width - 28.0f;
                float pillTop = 22.0f;
                float scale = g_settings.sizeScale;

                float gearLeft = pillRight - 44.0f * scale;
                float gearRight = pillRight - 8.0f * scale;
                float gearTop = pillTop + 6.0f * scale;
                float gearBottom = pillTop + 42.0f * scale;

                float pillHeight = (height - 44.0f) / scale;
                if (pillHeight > 50.0f) {
                    if (xPos >= gearLeft && xPos <= gearRight && yPos >= gearTop && yPos <= gearBottom) {
                        ::OpenSettingsDialog(hwnd);
                        return 0;
                    }
                }

                if (mediaActive && height > 60.0f && (g_idleTab % 4) == 0) {
                    float cx = width / 2.0f;
                    float cy = height / 2.0f;
                    
                    float unX = (xPos - cx) / totalScale;
                    float unY = (yPos - cy) / totalScale;

                    if (unY > 56.0f - 30.0f && unY < 56.0f + 30.0f) {
                        // Check button clicks in expanded media view
                        // 4-button layout: Back=-81, Play=-27, Forward=+27, AirPlay=+81
                        int cmd = -1;
                        if      (unX > -81.0f - 20.0f && unX < -81.0f + 20.0f) cmd = 0; // Prev
                        else if (unX > -27.0f - 26.0f && unX < -27.0f + 26.0f) cmd = 1; // Play/Pause
                        else if (unX >  27.0f - 20.0f && unX <  27.0f + 20.0f) cmd = 2; // Next
                        // AirPlay (+81) intentionally ignored — no media action

                        if (cmd != -1) {
                            std::thread([cmd]() {
                                winrt::init_apartment(winrt::apartment_type::multi_threaded);
                                try {
                                    using Manager = winrt::Windows::Media::Control::GlobalSystemMediaTransportControlsSessionManager;
                                    auto manager = Manager::RequestAsync().get();
                                    if (manager) {
                                        auto sessions = manager.GetSessions();
                                        std::wstring currentAumid;
                                        {
                                            std::lock_guard lock(g_stateMutex);
                                            currentAumid = g_state.media.sourceAppUserModelId;
                                        }
                                        winrt::Windows::Media::Control::GlobalSystemMediaTransportControlsSession session = nullptr;
                                        for (auto const& s : sessions) {
                                            if (s.SourceAppUserModelId().c_str() == currentAumid) {
                                                session = s;
                                                break;
                                            }
                                        }
                                        if (!session) session = manager.GetCurrentSession();

                                        if (session) {
                                            if (cmd == 0) session.TrySkipPreviousAsync().get();
                                            else if (cmd == 1) session.TryTogglePlayPauseAsync().get();
                                            else if (cmd == 2) session.TrySkipNextAsync().get();
                                        }
                                    }
                                } catch (...) {}
                            }).detach();
                            return 0;
                        }
                    }
                }

                // Dynamic tab count depending on whether media is active
                int tabCount = mediaActive ? 5 : 4;
                int currentTab = g_idleTab % tabCount;
                if (currentTab < 0) currentTab += tabCount;

                // Check tools clicks when we are on the Tools Tray tab
                bool onToolsTab = (mediaActive && currentTab == 4) || (!mediaActive && currentTab == 3);
                if (height > 60.0f && onToolsTab && !kinds.empty() && (kinds[0] == IslandKind::Idle || kinds[0] == IslandKind::Media)) {
                    float clickX = unX + unW * 0.5f;
                    float clickY = unY + unH * 0.5f;

                    // Calculate button boundaries exactly as drawn in dynamic_island_ui.h
                    RECT windowRect = {};
                    GetWindowRect(hwnd, &windowRect);
                    POINT mouseCursor = {};
                    GetCursorPos(&mouseCursor);
                    bool hover = PtInRect(&windowRect, mouseCursor) != FALSE;
                    bool pinned = Wh_GetIntValue(L"PinnedExpanded", 0) != 0;

                    // Subtract the rendering padding offset (28.0f, 22.0f) so that the click coordinates
                    // align 100% perfectly with the unscaledRect rendering coordinate space.
                    float pillClickX = clickX - 28.0f;
                    float pillClickY = clickY - 22.0f;

                    float scale = (hover || pinned) ? 1.025f : 1.0f;
                    float topY = 12.0f * scale;
                    float btnSize = 22.0f * scale;
                    float gap = 8.0f * scale;
                    float startX = unW - 24.0f * scale;

                    // Settings Button
                    float gearLeft = startX - btnSize;
                    float gearRight = startX;
                    float gearTop = topY;
                    float gearBottom = topY + btnSize;

                    // Edit Button
                    float editLeft = startX - btnSize - (btnSize + gap);
                    float editRight = startX - (btnSize + gap);
                    float editTop = topY;
                    float editBottom = topY + btnSize;

                    // Add Button
                    float addLeft = startX - btnSize - 2.0f * (btnSize + gap);
                    float addRight = startX - 2.0f * (btnSize + gap);
                    float addTop = topY;
                    float addBottom = topY + btnSize;

                    // 1. Settings button (gear) click
                    if (pillClickX >= gearLeft && pillClickX <= gearRight && pillClickY >= gearTop && pillClickY <= gearBottom) {
                        ::OpenSettingsDialog(hwnd);
                        return 0;
                    }

                    // 2. Edit button (minus / check) click
                    if (pillClickX >= editLeft && pillClickX <= editRight && pillClickY >= editTop && pillClickY <= editBottom) {
                        g_toolsEditMode = !g_toolsEditMode;
                        g_layoutDirty = true;
                        return 0;
                    }

                    // 3. Add button (plus) click
                    if (pillClickX >= addLeft && pillClickX <= addRight && pillClickY >= addTop && pillClickY <= addBottom) {
                        HMENU menu = CreatePopupMenu();
                        std::vector<std::wstring> availableToAdd;
                        int idx = 0;
                        for (const auto& t : kAllTools) {
                            if (std::find(g_activeTools.begin(), g_activeTools.end(), t.id) == g_activeTools.end()) {
                                AppendMenuW(menu, MF_STRING, 100 + idx, (t.icon + L" " + t.name).c_str());
                                availableToAdd.push_back(t.id);
                                idx++;
                            }
                        }
                        if (idx > 0) {
                            POINT pt;
                            GetCursorPos(&pt);
                            int choice = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
                            if (choice >= 100 && choice < 100 + idx) {
                                g_activeTools.push_back(availableToAdd[choice - 100]);
                                SaveToolsList();
                                g_layoutDirty = true;
                            }
                        }
                        DestroyMenu(menu);
                        return 0;
                    }

                    // Card Grid Click Checking (matching grid layout in dynamic_island_ui.h)
                    float startGridY = 46.0f * scale;
                    float cardW = 90.0f * scale;
                    float cardH = 72.0f * scale;
                    float gapX = 12.0f * scale;
                    float gapY = 12.0f * scale;
                    float marginX = 12.0f * scale;

                    size_t totalItems = g_activeTools.size();

                    for (size_t i = 0; i < totalItems; ++i) {
                        size_t r = i / 4;
                        size_t c = i % 4;
                        float x = marginX + c * (cardW + gapX);
                        float y = startGridY + r * (cardH + gapY);

                        // Calculate hovered card boundary
                        float cardTop = y - 3.0f * scale;
                        float cardBottom = y + cardH - 3.0f * scale;

                        if (pillClickX >= x && pillClickX <= x + cardW && pillClickY >= cardTop && pillClickY <= cardBottom) {
                            if (g_toolsEditMode) {
                                // Check if clicked the red deletion badge at top-right of the card
                                float bx = (x + cardW) - 10.0f * scale;
                                float by = cardTop + 10.0f * scale;
                                float distBadgeSq = (pillClickX - bx) * (pillClickX - bx) + (pillClickY - by) * (pillClickY - by);
                                if (distBadgeSq <= 12.0f * 12.0f * scale * scale) { // Generous hit target
                                    g_activeTools.erase(g_activeTools.begin() + i);
                                    SaveToolsList();
                                    g_layoutDirty = true;
                                    return 0;
                                }
                            } else {
                                // Launch tool
                                std::wstring toolId = g_activeTools[i];
                                if (toolId == L"clipboard") {
                                    keybd_event(VK_LWIN, 0, 0, 0);
                                    keybd_event('V', 0, 0, 0);
                                    keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
                                    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
                                    return 0;
                                }
                                if (toolId == L"settings") {
                                    ::OpenSettingsDialog(hwnd);
                                    return 0;
                                }
                                if (toolId == L"timer") {
                                    std::thread([](HWND hParent) {
                                        if (ShowTimerPickerModal(hParent)) {
                                            int totalSec = g_tpSelectedHours * 3600 + g_tpSelectedMinutes * 60 + g_tpSelectedSeconds;
                                            if (totalSec > 0) {
                                                std::lock_guard<std::mutex> lock(g_stateMutex);
                                                g_timerActive = true;
                                                g_timerDurationMs = totalSec * 1000ULL;
                                                g_timerTargetTick = GetTickCount64() + g_timerDurationMs;
                                                g_timerPaused = false;
                                                g_timerPauseRemainingMs = 0;
                                                g_timerAlertActive = false;
                                                g_layoutDirty = true;
                                            }
                                            PostMessageW(hParent, WM_NULL, 0, 0);
                                        }
                                    }, hwnd).detach();
                                    return 0;
                                }
                                const ToolItem* item = FindToolById(toolId);
                                if (item) {
                                    ShellExecuteW(nullptr, L"open", item->cmd.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                                    return 0;
                                }
                            }
                        }
                    }
                }

                // If media is playing and we are on the Media tab (0), clicking outside controls opens music app
                if (mediaActive && currentTab == 0) {
                    if (height > 45.0f && xPos > width - 30.0f) {
                        g_idleTab = (g_idleTab + 1) % tabCount;
                        g_layoutDirty = true;
                    } else {
                        OpenRelevantApp();
                    }
                } else {
                    // We are either in Idle or on one of the sub-tabs of Media
                    if (!kinds.empty() && (kinds[0] == IslandKind::Idle || kinds[0] == IslandKind::Media) && height > 45.0f) {
                        if (xPos < width / 2.0f) g_idleTab = (g_idleTab - 1 + tabCount) % tabCount;
                        else g_idleTab = (g_idleTab + 1) % tabCount;
                        g_layoutDirty = true;
                    } else {
                        HandleStatusClickAtPoint(hwnd, lParam);
                    }
                }
            }
            return 0;

        case WM_MBUTTONUP:
            ToggleEndpointMute();
            return 0;

        case WM_LBUTTONDBLCLK:
            Wh_SetIntValue(L"PinnedExpanded", Wh_GetIntValue(L"PinnedExpanded", 0) ? 0 : 1);
            return 0;

        case WM_MOUSEWHEEL: {
            static ULONGLONG lastScrollTime = 0;
            ULONGLONG now = GetTickCount64();
            if (now - lastScrollTime < 150) return 0; // 150ms debounce
            lastScrollTime = now;

            bool mediaActive = false;
            {
                std::lock_guard lock(g_stateMutex);
                mediaActive = g_settings.media && g_state.media.available;
            }
            int tabCount = mediaActive ? 5 : 4;
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta > 0) {
                if (g_idleTab > 0) g_idleTab--;
            } else if (delta < 0) {
                if (g_idleTab < tabCount - 1) g_idleTab++;
            }
            
            g_layoutDirty = true;
            return 0;
        }

        case WM_RBUTTONUP: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ClientToScreen(hwnd, &pt);
            ShowContextMenu(hwnd, pt);
            return 0;
        }
    }

    if (msg == g_shellHookMessage) {
        if (wParam == HSHELL_WINDOWCREATED) {
            CaptureShellNotification(reinterpret_cast<HWND>(lParam));
        }
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

DWORD WINAPI RenderThreadProc(void*) {
    Wh_Log(L"RenderThreadProc started.");
    HRESULT hrCo = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = OverlayWndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = kWindowClass;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    RegisterClassExW(&wc);
    Wh_Log(L"Class registered.");

    HWND hwnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
        kWindowClass, L"Dynamic Island for Windows", WS_POPUP, 0, 0, 520, 140,
        nullptr, nullptr, wc.hInstance, nullptr);

    if (!hwnd) {
        Wh_Log(L"Failed to create Dynamic Island overlay window. Error: %d", GetLastError());
        if (SUCCEEDED(hrCo)) {
            CoUninitialize();
        }
        return 0;
    }

    Wh_Log(L"Window created successfully (HWND = %p).", hwnd);
    g_hwnd = hwnd;
    g_shellHookMessage = RegisterWindowMessageW(L"SHELLHOOK");
    UpdateWindowBlur(hwnd, g_settings.colorTheme);
    ShowWindow(hwnd, SW_SHOWNOACTIVATE);

    Renderer renderer;
    if (!renderer.Initialize(hwnd)) {
        Wh_Log(L"Renderer initialization failed!");
        DestroyWindow(hwnd);
        g_hwnd = nullptr;
        if (SUCCEEDED(hrCo)) {
            CoUninitialize();
        }
        return 0;
    }

    Wh_Log(L"Renderer initialized successfully.");
    SpringValue widthSpring;
    SpringValue heightSpring;
    SpringValue nudgeSpring;
    widthSpring.Reset((g_settings.autoHideIdleSeconds == -1 ? 0.0f : 120.0f) * g_settings.sizeScale);
    heightSpring.Reset((g_settings.autoHideIdleSeconds == -1 ? 0.0f : 36.0f) * g_settings.sizeScale);
    nudgeSpring.Reset(0.0f);

    IslandKind previousPrimary = IslandKind::Idle;
    auto previousFrame = std::chrono::steady_clock::now();
    double nextBatteryPoll = 0.0;
    double nextProgressPoll = 0.0;
    double nextSystemPoll = 0.0;
    double nextPrivacyPoll = 0.0;

    Wh_Log(L"Entering main rendering loop.");
    while (WaitForSingleObject(g_stopEvent, 0) == WAIT_TIMEOUT) {
        MSG message = {};
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            if (message.message == WM_APP_NEW_EVENT) {
                nudgeSpring.value = -6.0f;
                nudgeSpring.velocity = 0.0f;
                nudgeSpring.target = 0.0f;
                continue;
            }
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        const double now = NowSeconds();
        if (g_timerActive) {
            ULONGLONG nowTick = GetTickCount64();
            if (!g_timerPaused) {
                if (nowTick >= g_timerTargetTick) {
                    if (!g_timerAlertActive) {
                        g_timerAlertActive = true;
                        g_timerAlertStartTick = nowTick;
                    }
                    
                    static ULONGLONG lastBeep = 0;
                    if (nowTick - lastBeep > 1500) {
                        MessageBeep(MB_ICONINFORMATION);
                        lastBeep = nowTick;
                    }

                    if (nowTick - g_timerAlertStartTick > 8000) {
                        g_timerActive = false;
                        g_timerAlertActive = false;
                    }
                }
            }
        }

        if (now >= nextBatteryPoll) {
            UpdateBatterySnapshot();
            nextBatteryPoll = now + 15.0;
        }
        if (now >= nextProgressPoll) {
            UpdateProgressSnapshot();
            nextProgressPoll = now + 0.25;
        }
        if (now >= nextSystemPoll) {
            UpdateSystemSnapshot();
            nextSystemPoll = now + 1.0;
        }
        if (now >= nextPrivacyPoll) {
            UpdatePrivacyIndicators();
            nextPrivacyPoll = now + 2.0;  // poll every 2 s
        }

        SharedState snapshot;
        {
            std::lock_guard lock(g_stateMutex);
            snapshot = g_state;
            if (g_state.clipboard.active && now >= g_state.clipboard.expiresAt) {
                g_state.clipboard.active = false;
                snapshot.clipboard.active = false;
            }
            if (g_state.notification.active && now >= g_state.notification.expiresAt) {
                g_state.notification.active = false;
                snapshot.notification.active = false;
            }
            if (g_state.volume.active && now >= g_state.volume.expiresAt) {
                g_state.volume.active = false;
                snapshot.volume.active = false;
            }
            if (g_state.capsLock.active && now >= g_state.capsLock.expiresAt) {
                g_state.capsLock.active = false;
                snapshot.capsLock.active = false;
            }
            if (g_state.battery.active && now >= g_state.battery.expiresAt) {
                g_state.battery.active = false;
                snapshot.battery.active = false;
            }
            if (g_state.device.active && now >= g_state.device.expiresAt) {
                g_state.device.active = false;
                snapshot.device.active = false;
            }
        }

        const std::vector<IslandKind> kinds = ChooseActivities(snapshot, g_settings, now);
        Activity primary = ActivityForKind(kinds[0], g_settings, snapshot);
        std::optional<Activity> secondary;
        if (kinds.size() >= 2 && g_settings.splitMode) {
            secondary = ActivityForKind(kinds[1], g_settings, snapshot);
            // Adjust widths for Split mode!
            primary.width = 62.0f * g_settings.sizeScale;
            secondary->width = 36.0f * g_settings.sizeScale;
        }

        const bool pinned = Wh_GetIntValue(L"PinnedExpanded", 0) != 0;

        if (primary.kind != previousPrimary && primary.kind != IslandKind::Idle) {
            nudgeSpring.value = -6.0f;
            nudgeSpring.velocity = 0.0f;
            nudgeSpring.target = 0.0f;
        }
        previousPrimary = primary.kind;

        RECT windowRect = {};
        GetWindowRect(hwnd, &windowRect);
        POINT cursor = {};
        GetCursorPos(&cursor);
        
        // Expand hover detection area if the island is shrunk/hidden
        RECT hoverRect = windowRect;
        float minHoverWidth = 170.0f * g_settings.sizeScale;
        float currentWidth = static_cast<float>(windowRect.right - windowRect.left);
        if (currentWidth < minHoverWidth) {
            float cx = (windowRect.left + windowRect.right) * 0.5f;
            hoverRect.left = static_cast<long>(cx - minHoverWidth * 0.5f);
            hoverRect.right = static_cast<long>(cx + minHoverWidth * 0.5f);
        }
        float minHoverHeight = 36.0f * g_settings.sizeScale;
        float currentHeight = static_cast<float>(windowRect.bottom - windowRect.top);
        if (currentHeight < minHoverHeight) {
            if (g_settings.position == Position::BottomCenter) {
                hoverRect.top = static_cast<long>(windowRect.bottom - minHoverHeight);
            } else {
                hoverRect.bottom = static_cast<long>(windowRect.top + minHoverHeight);
            }
        }
        const bool hover = PtInRect(&hoverRect, cursor) != FALSE;
        
        bool needsRender = false;
        
        if (!hover && g_clickExpanded.load()) {
            g_clickExpanded = false;
            needsRender = true;
        }
        static double lastInteractionTime = NowSeconds();
        bool currentlyHidden = false;
        if (g_settings.autoHideIdleSeconds == -1) {
            currentlyHidden = true;
        } else if (g_settings.autoHideIdleSeconds > 0) {
            currentlyHidden = (now - lastInteractionTime > g_settings.autoHideIdleSeconds);
        }

        bool isHoverExpanded = g_settings.expandOnHover ? hover : (hover && g_clickExpanded.load());
        
        if (currentlyHidden && !g_settings.unhideOnHover && primary.kind == IslandKind::Idle) {
            isHoverExpanded = false;
        } else if ((hover && (g_settings.unhideOnHover || !currentlyHidden)) || pinned || primary.kind != IslandKind::Idle) {
            lastInteractionTime = now;
        }
        
        bool isHidden = false;
        if (g_settings.autoHideIdleSeconds == -1) {
            isHidden = true;
        } else if (g_settings.autoHideIdleSeconds > 0) {
            isHidden = (now - lastInteractionTime > g_settings.autoHideIdleSeconds);
        }

        bool privacyActive = snapshot.system.micActive || snapshot.system.cameraActive;
        if (primary.kind == IslandKind::Idle) {
            if (pinned || isHoverExpanded) {
                primary.width = 420.0f * g_settings.sizeScale;
                primary.height = 220.0f * g_settings.sizeScale;
            } else if (isHidden && !privacyActive) {
                primary.width = 0.0f;
                primary.height = 0.0f;
            }
        }
        if (primary.kind == IslandKind::Idle &&
            (g_settings.gameOverlay || Wh_GetIntValue(L"GameOverlayPinned", 0) != 0)) {
            primary.width = 372.0f * g_settings.sizeScale;
            primary.height = 64.0f * g_settings.sizeScale;
        }
        if (primary.kind == IslandKind::Media) {
            bool recentArtChange = g_settings.expandOnTrackChange && ((NowSeconds() - g_state.media.artChangedAt) < 4.0);
            if (isHoverExpanded || pinned || recentArtChange) {
                int tab = g_idleTab % 5;
                if (tab < 0) tab += 5;
                if (tab == 0) { // Media
                    primary.width = 380.0f * g_settings.sizeScale;
                    primary.height = 184.0f * g_settings.sizeScale;
                } else { // Calendar (tab == 1), Weather (tab == 2), System Status (tab == 3), Tools Tray (tab == 4)
                    primary.width = 420.0f * g_settings.sizeScale;
                    primary.height = 220.0f * g_settings.sizeScale;
                }
            }
        }
        if (primary.kind == IslandKind::Timer) {
            if (isHoverExpanded || pinned) {
                primary.width = 300.0f * g_settings.sizeScale;
                primary.height = 84.0f * g_settings.sizeScale;
            }
        }
        if (isHoverExpanded || pinned) {
            if (primary.kind == IslandKind::Clipboard) {
                primary.width = 340.0f * g_settings.sizeScale;
                primary.height = 56.0f * g_settings.sizeScale;
            } else if (primary.kind == IslandKind::Notification) {
                primary.width = 360.0f * g_settings.sizeScale;
                primary.height = 64.0f * g_settings.sizeScale;
            } else if (primary.kind == IslandKind::Volume) {
                primary.width = 310.0f * g_settings.sizeScale;
                primary.height = 72.0f * g_settings.sizeScale;
            } else if (primary.kind == IslandKind::BatteryLow) {
                primary.width = 290.0f * g_settings.sizeScale;
                primary.height = 52.0f * g_settings.sizeScale;
            } else if (primary.kind == IslandKind::CapsLock) {
                primary.width = 180.0f * g_settings.sizeScale;
                primary.height = 42.0f * g_settings.sizeScale;
            } else if (primary.kind == IslandKind::Device) {
                if (g_settings.airpodsMode == AirPodsMode::SmallOnly) {
                    primary.width = 180.0f * g_settings.sizeScale;
                    primary.height = 36.0f * g_settings.sizeScale;
                } else {
                    primary.width = 310.0f * g_settings.sizeScale;
                    primary.height = 64.0f * g_settings.sizeScale;
                }
            } else if (primary.kind == IslandKind::Progress) {
                primary.width = 230.0f * g_settings.sizeScale;
                primary.height = 48.0f * g_settings.sizeScale;
            }
        }

        float targetWidth = primary.width;
        float targetHeight = primary.height;
        if (secondary) {
            targetWidth = primary.width + secondary->width + 12.0f * g_settings.sizeScale;
            targetHeight = std::max(primary.height, secondary->height);
        }
        if (pinned) {
            targetWidth = std::max(targetWidth, 380.0f * g_settings.sizeScale);
            targetHeight = std::max(targetHeight, 64.0f * g_settings.sizeScale);
        }

        widthSpring.target = targetWidth;
        heightSpring.target = targetHeight;

        const auto currentFrame = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(currentFrame - previousFrame).count();
        previousFrame = currentFrame;
        dt = Clamp(dt, 0.001f, 0.050f);

        const float speed = g_settings.animationSpeed;
        float widthStiffness = 280.0f;
        float widthDamping = 24.0f;
        if (targetWidth > widthSpring.value) {
            widthStiffness = 380.0f;
            widthDamping = 26.0f;
        } else if (targetWidth < widthSpring.value) {
            widthStiffness = 200.0f;
            widthDamping = 28.0f;
        }

        float heightStiffness = 280.0f;
        float heightDamping = 24.0f;
        if (targetHeight > heightSpring.value) {
            heightStiffness = 380.0f;
            heightDamping = 26.0f;
        } else if (targetHeight < heightSpring.value) {
            heightStiffness = 200.0f;
            heightDamping = 28.0f;
        }

        widthSpring.Step(dt * speed, widthStiffness, widthDamping);
        if (widthSpring.value < 0.0f) {
            widthSpring.value = 0.0f;
            widthSpring.velocity = 0.0f;
        }

        heightSpring.Step(dt * speed, heightStiffness, heightDamping);
        if (heightSpring.value < 0.0f) {
            heightSpring.value = 0.0f;
            heightSpring.velocity = 0.0f;
        }

        nudgeSpring.Step(dt * speed, 280.0f, 24.0f);

        {
            std::lock_guard lock(g_stateMutex);
            g_state.system.renderFps = ClampInt(static_cast<int>(1.0f / std::max(dt, 0.001f) + 0.5f), 0, 240);
        }

        SetClickThrough(hwnd, primary.kind == IslandKind::Idle && !hover && !pinned);

        // Check if animating structurally
        if (std::abs(widthSpring.velocity) > 0.01f || std::abs(widthSpring.target - widthSpring.value) > 0.01f ||
            std::abs(heightSpring.velocity) > 0.01f || std::abs(heightSpring.target - heightSpring.value) > 0.01f ||
            std::abs(nudgeSpring.velocity) > 0.01f || std::abs(nudgeSpring.target - nudgeSpring.value) > 0.01f) {
            needsRender = true;
        }

        // Active Monitor Tracking (Follow Mouse)
        if (g_settings.targetMonitor == -1) {
            static HMONITOR s_lastMonitor = nullptr;
            POINT pt;
            GetCursorPos(&pt);
            HMONITOR currentMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
            if (currentMonitor != s_lastMonitor) {
                s_lastMonitor = currentMonitor;
                g_layoutDirty = true;
            }
        }

        // Check if layout was explicitly invalidated
        if (g_layoutDirty.load()) {
            needsRender = true;
        }

        // Hover or pinned state changes visual elements slightly
        static bool prevHover = false;
        static bool prevPinned = false;
        if (hover != prevHover || pinned != prevPinned) {
            needsRender = true;
            prevHover = hover;
            prevPinned = pinned;
        }

        // Animated activities that require continuous rendering
        if (primary.kind == IslandKind::Media || primary.kind == IslandKind::BatteryLow ||
            primary.kind == IslandKind::Clipboard || primary.kind == IslandKind::Notification ||
            primary.kind == IslandKind::Timer) {
            needsRender = true;
        }

        // Privacy dots
        if (snapshot.system.micActive || snapshot.system.cameraActive) {
            needsRender = true;
        }
        
        // Idle dashboard clock changes once a minute
        static SYSTEMTIME prevTime = {};
        static int prevCycleSec = -1;
        if (primary.kind == IslandKind::Idle && !isHidden) {
            SYSTEMTIME local = {};
            GetLocalTime(&local);
            if (local.wMinute != prevTime.wMinute) {
                needsRender = true;
                prevTime = local;
            }
            int currentCycleSec = static_cast<int>(now) / 5;
            if (currentCycleSec != prevCycleSec) {
                needsRender = true;
                prevCycleSec = currentCycleSec;
            }
        }

        // Compare data snapshot to detect changes
        static uint64_t prevArtGen = 0;
        static uint64_t prevSrcIconGen = 0;
        static uint64_t prevNotifIconGen = 0;
        static uint64_t prevClipIconGen = 0;
        static int prevCpu = -1;
        static int prevRam = -1;
        static int prevDisk = -1;
        static int prevVol = -1;
        static bool prevMuted = false;
        static int prevBat = -1;
        static bool prevCharging = false;
        static int prevProg = -1;
        static std::wstring prevMediaTitle;
        static std::wstring prevForegroundTitle;
        
        if (snapshot.media.artGeneration != prevArtGen ||
            snapshot.media.sourceIconGeneration != prevSrcIconGen ||
            snapshot.media.title != prevMediaTitle ||
            snapshot.system.foregroundTitle != prevForegroundTitle ||
            snapshot.notification.icon.generation != prevNotifIconGen ||
            snapshot.clipboard.appIcon.generation != prevClipIconGen ||
            snapshot.system.cpuPercent != prevCpu ||
            snapshot.system.memoryPercent != prevRam ||
            snapshot.system.diskFreePercent != prevDisk ||
            snapshot.system.volumePercent != prevVol ||
            snapshot.system.volumeMuted != prevMuted ||
            snapshot.battery.percent != prevBat ||
            snapshot.battery.charging != prevCharging ||
            snapshot.progress.percent != prevProg) {
            needsRender = true;
            prevArtGen = snapshot.media.artGeneration;
            prevSrcIconGen = snapshot.media.sourceIconGeneration;
            prevMediaTitle = snapshot.media.title;
            prevForegroundTitle = snapshot.system.foregroundTitle;
            prevNotifIconGen = snapshot.notification.icon.generation;
            prevClipIconGen = snapshot.clipboard.appIcon.generation;
            prevCpu = snapshot.system.cpuPercent;
            prevRam = snapshot.system.memoryPercent;
            prevDisk = snapshot.system.diskFreePercent;
            prevVol = snapshot.system.volumePercent;
            prevMuted = snapshot.system.volumeMuted;
            prevBat = snapshot.battery.percent;
            prevCharging = snapshot.battery.charging;
            prevProg = snapshot.progress.percent;
        }

        if (needsRender) {
            renderer.Render(snapshot, g_settings, primary, secondary,
                            widthSpring.value, heightSpring.value, nudgeSpring.value,
                            hover, pinned, now);
        }

        WaitForSingleObject(g_stopEvent, 16);
    }

    renderer.Shutdown();
    DestroyWindow(hwnd);
    g_hwnd = nullptr;
    UnregisterClassW(kWindowClass, wc.hInstance);

    if (SUCCEEDED(hrCo)) {
        CoUninitialize();
    }

    return 0;
}



bool StartThreads() {
    g_stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    g_settingsChangedEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (!g_stopEvent || !g_settingsChangedEvent) {
        return false;
    }

    g_running = true;
    g_renderThread = CreateThread(nullptr, 0, RenderThreadProc, nullptr, 0, nullptr);
    if (!g_renderThread) {
        return false;
    }

    g_mediaThread = CreateThread(nullptr, 0, MediaThreadProc, nullptr, 0, nullptr);
    g_audioThread = CreateThread(nullptr, 0, AudioThreadProc, nullptr, 0, nullptr);
    g_weatherThread = CreateThread(nullptr, 0, WeatherThreadProc, nullptr, 0, nullptr);
    g_keyboardThread = CreateThread(nullptr, 0, KeyboardThreadProc, nullptr, 0, &g_keyboardThreadId);
#if DYNAMIC_ISLAND_HAS_USER_NOTIFICATION_LISTENER
    g_notificationThread = CreateThread(nullptr, 0, NotificationThreadProc, nullptr, 0, nullptr);
#endif

    return true;
}

void StopThreads() {
    if (g_keyboardThreadId != 0) {
        PostThreadMessageW(g_keyboardThreadId, WM_QUIT, 0, 0);
    }
    if (g_stopEvent) {
        SetEvent(g_stopEvent);
    }

    HANDLE handles[] = {g_renderThread, g_mediaThread, g_audioThread, g_weatherThread, g_notificationThread, g_keyboardThread};
    for (HANDLE handle : handles) {
        if (handle) {
            WaitForSingleObject(handle, 3000);
            CloseHandle(handle);
        }
    }

    g_renderThread = nullptr;
    g_mediaThread = nullptr;
    g_audioThread = nullptr;
    g_weatherThread = nullptr;
    g_notificationThread = nullptr;
    g_keyboardThread = nullptr;
    g_keyboardThreadId = 0;

    if (g_stopEvent) {
        CloseHandle(g_stopEvent);
        g_stopEvent = nullptr;
    }
    if (g_settingsChangedEvent) {
        CloseHandle(g_settingsChangedEvent);
        g_settingsChangedEvent = nullptr;
    }

    g_running = false;
}



}  // namespace

BOOL WhTool_ModInit() {
    LoadSettings();

    if (!StartThreads()) {
        StopThreads();
        return FALSE;
    }

    g_layoutDirty = true;
    Wh_Log(L"Dynamic Island for Windows initialized.");
    return TRUE;
}

void WhTool_ModSettingsChanged() {
    LoadSettings();
}

void WhTool_ModUninit() {
    if (g_hwnd) {
        PostMessageW(g_hwnd, WM_CLOSE, 0, 0);
    }
    StopThreads();
    Wh_Log(L"Dynamic Island for Windows unloaded.");
}

#ifndef STANDALONE_APP
//////////////////////////////////////////////////////////////////////////////////
// Windhawk tool mod implementation for mods which don't need to inject to other
// processes or hook other functions. Context:
// https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process
//
// The mod will load and run in a dedicated windhawk.exe process.
//
// Paste the code below as part of the mod code, and use these callbacks:
// * WhTool_ModInit
// * WhTool_ModSettingsChanged
// * WhTool_ModUninit
//
// Currently, other callbacks are not supported.

bool g_isToolModProcessLauncher;
HANDLE g_toolModProcessMutex;

void WINAPI EntryPoint_Hook() {
    Wh_Log(L">");
    ExitThread(0);
}

BOOL Wh_ModInit() {
    DWORD sessionId;
    if (ProcessIdToSessionId(GetCurrentProcessId(), &sessionId) &&
        sessionId == 0) {
        return FALSE;
    }

    bool isExcluded = false;
    bool isToolModProcess = false;
    bool isCurrentToolModProcess = false;
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
    if (!argv) {
        Wh_Log(L"CommandLineToArgvW failed");
        return FALSE;
    }

    for (int i = 1; i < argc; i++) {
        if (wcscmp(argv[i], L"-service") == 0 ||
            wcscmp(argv[i], L"-service-start") == 0 ||
            wcscmp(argv[i], L"-service-stop") == 0) {
            isExcluded = true;
            break;
        }
    }

    for (int i = 1; i < argc - 1; i++) {
        if (wcscmp(argv[i], L"-tool-mod") == 0) {
            isToolModProcess = true;
            if (wcscmp(argv[i + 1], WH_MOD_ID) == 0) {
                isCurrentToolModProcess = true;
            }
            break;
        }
    }

    LocalFree(argv);

    if (isExcluded) {
        return FALSE;
    }

    if (isCurrentToolModProcess) {
        g_toolModProcessMutex =
            CreateMutex(nullptr, TRUE, L"windhawk-tool-mod_" WH_MOD_ID);
        if (!g_toolModProcessMutex) {
            Wh_Log(L"CreateMutex failed");
            ExitProcess(1);
        }

        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            Wh_Log(L"Tool mod already running (%s)", WH_MOD_ID);
            ExitProcess(1);
        }

        if (!WhTool_ModInit()) {
            ExitProcess(1);
        }

        IMAGE_DOS_HEADER* dosHeader =
            (IMAGE_DOS_HEADER*)GetModuleHandle(nullptr);
        IMAGE_NT_HEADERS* ntHeaders =
            (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);

        DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
        void* entryPoint = (BYTE*)dosHeader + entryPointRVA;

        Wh_SetFunctionHook(entryPoint, (void*)EntryPoint_Hook, nullptr);
        return TRUE;
    }

    if (isToolModProcess) {
        return FALSE;
    }

    g_isToolModProcessLauncher = true;
    return TRUE;
}

void Wh_ModAfterInit() {
    if (!g_isToolModProcessLauncher) {
        return;
    }

    WCHAR currentProcessPath[MAX_PATH];
    switch (GetModuleFileName(nullptr, currentProcessPath,
                               ARRAYSIZE(currentProcessPath))) {
        case 0:
        case ARRAYSIZE(currentProcessPath):
            Wh_Log(L"GetModuleFileName failed");
            return;
    }

    WCHAR
    commandLine[MAX_PATH + 2 +
                (sizeof(L" -tool-mod \"" WH_MOD_ID "\"") / sizeof(WCHAR)) - 1];
    swprintf_s(commandLine, L"\"%s\" -tool-mod \"%s\"", currentProcessPath,
               WH_MOD_ID);

    HMODULE kernelModule = GetModuleHandle(L"kernelbase.dll");
    if (!kernelModule) {
        kernelModule = GetModuleHandle(L"kernel32.dll");
        if (!kernelModule) {
            Wh_Log(L"No kernelbase.dll/kernel32.dll");
            return;
        }
    }

    using CreateProcessInternalW_t = BOOL(WINAPI*)(
        HANDLE hUserToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes, WINBOOL bInheritHandles,
        DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation,
        PHANDLE hRestrictedUserToken);
    CreateProcessInternalW_t pCreateProcessInternalW =
        (CreateProcessInternalW_t)GetProcAddress(kernelModule,
                                                 "CreateProcessInternalW");
    if (!pCreateProcessInternalW) {
        Wh_Log(L"No CreateProcessInternalW");
        return;
    }

    STARTUPINFO si{
        .cb = sizeof(STARTUPINFO),
        .dwFlags = STARTF_FORCEOFFFEEDBACK,
    };
    PROCESS_INFORMATION pi;
    if (!pCreateProcessInternalW(nullptr, currentProcessPath, commandLine,
                                 nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS,
                                 nullptr, nullptr, &si, &pi, nullptr)) {
        Wh_Log(L"CreateProcess failed");
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void Wh_ModSettingsChanged() {
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModSettingsChanged();
}

void Wh_ModUninit() {
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModUninit();
    ExitProcess(0);
}
#else
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ============================================================
//  HSV Color Picker Dialog
// ============================================================

// --- HSV <-> RGB helpers ---
// Shared result globals (must be declared before ColorPickerWndProc)
static COLORREF g_cpResult = RGB(255, 0, 0);
static bool     g_cpOk     = false;

static void RGBtoHSV(int r, int g, int b, float& h, float& s, float& v) {
    float rf = r / 255.0f, gf = g / 255.0f, bf = b / 255.0f;
    float mx = std::max({ rf, gf, bf }), mn = std::min({ rf, gf, bf });
    float delta = mx - mn;
    v = mx;
    s = (mx > 0.0f) ? (delta / mx) : 0.0f;
    if (delta < 1e-6f) { h = 0.0f; return; }
    if      (mx == rf) h = 60.0f * fmodf((gf - bf) / delta, 6.0f);
    else if (mx == gf) h = 60.0f * ((bf - rf) / delta + 2.0f);
    else               h = 60.0f * ((rf - gf) / delta + 4.0f);
    if (h < 0.0f) h += 360.0f;
}

static COLORREF HSVtoRGB(float h, float s, float v) {
    float c = v * s, x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f)), m = v - c;
    float r1, g1, b1;
    if      (h < 60)  { r1=c; g1=x; b1=0; }
    else if (h < 120) { r1=x; g1=c; b1=0; }
    else if (h < 180) { r1=0; g1=c; b1=x; }
    else if (h < 240) { r1=0; g1=x; b1=c; }
    else if (h < 300) { r1=x; g1=0; b1=c; }
    else              { r1=c; g1=0; b1=x; }
    return RGB((int)((r1+m)*255), (int)((g1+m)*255), (int)((b1+m)*255));
}

// --- Draw the color wheel into a DIB ---
static HBITMAP CreateColorWheelBitmap(HDC hdc, int size, float brightness) {
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = size;
    bmi.bmiHeader.biHeight      = -size; // top-down
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    DWORD* bits = nullptr;
    HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&bits, nullptr, 0);
    if (!hbm || !bits) return nullptr;

    float cx = size * 0.5f, cy = size * 0.5f, radius = size * 0.5f - 1.0f;
    for (int py = 0; py < size; ++py) {
        for (int px = 0; px < size; ++px) {
            float dx = px - cx, dy = py - cy;
            float dist = sqrtf(dx*dx + dy*dy);
            if (dist > radius) {
                bits[py * size + px] = 0xFF1E1E1E; // background color (dark)
            } else {
                float sat = dist / radius;
                float hue = fmodf(atan2f(dy, dx) * (180.0f / 3.14159265f) + 360.0f, 360.0f);
                COLORREF c = HSVtoRGB(hue, sat, brightness);
                // DIB stores BGR
                bits[py * size + px] = 0xFF000000 |
                    ((DWORD)(GetBValue(c)) << 16) |
                    ((DWORD)(GetGValue(c)) << 8)  |
                    ((DWORD)(GetRValue(c)));
            }
        }
    }
    return hbm;
}

// --- Color Picker state & IDs ---
#define CP_ID_WHEEL_AREA  2001
#define CP_ID_SAT_SLIDER  2002
#define CP_ID_VAL_SLIDER  2003
#define CP_ID_HEX_EDIT    2004
#define CP_ID_PREVIEW     2005
#define CP_ID_CHOOSE      IDOK
#define CP_ID_CANCEL      IDCANCEL

struct ColorPickerState {
    float h = 0, s = 1, v = 1;   // current HSV
    COLORREF result = RGB(255,0,0);
    bool dragging = false;
    HBITMAP wheelBmp = nullptr;
    int wheelSize = 220;
    HWND hwndSat = nullptr, hwndVal = nullptr, hwndHex = nullptr;
    bool updatingHex = false;
};

static void CP_UpdateAll(HWND hwnd, ColorPickerState* cp) {
    cp->result = HSVtoRGB(cp->h, cp->s, cp->v);

    // Rebuild wheel bitmap with new brightness
    HDC hdc = GetDC(hwnd);
    if (cp->wheelBmp) DeleteObject(cp->wheelBmp);
    cp->wheelBmp = CreateColorWheelBitmap(hdc, cp->wheelSize, cp->v);
    ReleaseDC(hwnd, hdc);

    // Update hex edit
    if (cp->hwndHex && !cp->updatingHex) {
        wchar_t buf[16];
        swprintf_s(buf, L"#%02X%02X%02X",
            GetRValue(cp->result), GetGValue(cp->result), GetBValue(cp->result));
        cp->updatingHex = true;
        SetWindowTextW(cp->hwndHex, buf);
        cp->updatingHex = false;
    }

    // Repaint
    InvalidateRect(hwnd, nullptr, FALSE);
}

static void CP_SetFromHex(HWND hwnd, ColorPickerState* cp, const wchar_t* hex) {
    // Parse #RRGGBB or RRGGBB
    const wchar_t* p = hex;
    if (*p == L'#') ++p;
    if (wcslen(p) < 6) return;
    int r = 0, g = 0, b = 0;
    swscanf_s(p, L"%02X%02X%02X", &r, &g, &b);
    RGBtoHSV(r, g, b, cp->h, cp->s, cp->v);
    // Sync sliders
    if (cp->hwndSat) SendMessageW(cp->hwndSat, TBM_SETPOS, TRUE, (LPARAM)(int)(cp->s * 255));
    if (cp->hwndVal) SendMessageW(cp->hwndVal, TBM_SETPOS, TRUE, (LPARAM)(int)(cp->v * 255));
    CP_UpdateAll(hwnd, cp);
}

LRESULT CALLBACK ColorPickerWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ColorPickerState* cp = reinterpret_cast<ColorPickerState*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    switch (msg) {
    case WM_CREATE: {
        cp = new ColorPickerState();
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)cp);

        // Parse initial color stored in the title temporarily
        wchar_t initHex[16] = {};
        GetWindowTextW(hwnd, initHex, 16);
        SetWindowTextW(hwnd, L"Color Picker");
        if (initHex[0]) {
            const wchar_t* p = initHex[0] == L'#' ? initHex + 1 : initHex;
            int r = 0, g = 0, b = 0;
            swscanf_s(p, L"%02X%02X%02X", &r, &g, &b);
            RGBtoHSV(r, g, b, cp->h, cp->s, cp->v);
        }
        // FIX: Always start with full brightness so wheel shows vivid colors.
        // If the initial color is very dark/grey, the wheel would appear nearly
        // black and selecting any color would still give a dark result.
        if (cp->v < 0.5f) cp->v = 1.0f;
        if (cp->s < 0.1f) cp->s = 1.0f;  // grey → start fully saturated

        BOOL dark = TRUE;
        DwmSetWindowAttribute(hwnd, 20, &dark, sizeof(dark));
        HFONT fnt = CreateFontW(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, L"Segoe UI");
        HFONT fntBold = CreateFontW(-13, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, L"Segoe UI");

        // ── Fixed pixel layout (window client area = 440×460) ──
        // Wheel:     left=12, top=12, size=210  → bottom=222
        // Preview:   left=232, top=12, right=420, bottom=72
        // Hex:       left=232, top=82 ..138
        // Sat row:   y=236..268
        // Bright row:y=278..310
        // Buttons:   y=328..362   (well within 460-35title=425 client)
        const int WX=12, WY=12, WSZ=cp->wheelSize; // 210
        const int PX=WX+WSZ+10, PY=12, PW=420-PX-8, PH=60;
        const int HEX_Y=PY+PH+10;
        const int SL_Y=WY+WSZ+14;    // 236
        const int BR_Y=SL_Y+40;       // 276
        const int BTN_Y=BR_Y+40+18;   // 334
        const int WIN_W=440, SLW=WIN_W-24;

        HWND lbl;

        // Preview label (static text)
        lbl = CreateWindowExW(0, L"STATIC", L"Selected Color",
            WS_CHILD|WS_VISIBLE|SS_CENTER,
            PX, PY+PH+2, PW, 16, hwnd, nullptr, nullptr, nullptr);
        SendMessageW(lbl, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        // Hex code label
        lbl = CreateWindowExW(0, L"STATIC", L"Hex Code:",
            WS_CHILD|WS_VISIBLE,
            PX, HEX_Y+20, PW, 16, hwnd, nullptr, nullptr, nullptr);
        SendMessageW(lbl, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        // Hex edit (type #RRGGBB directly)
        cp->hwndHex = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"#FF0000",
            WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL|ES_UPPERCASE,
            PX, HEX_Y+38, PW, 24, hwnd, (HMENU)CP_ID_HEX_EDIT, nullptr, nullptr);
        SendMessageW(cp->hwndHex, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        // Saturation label + full-width slider
        lbl = CreateWindowExW(0, L"STATIC", L"Saturation:",
            WS_CHILD|WS_VISIBLE,
            12, SL_Y, 100, 16, hwnd, nullptr, nullptr, nullptr);
        SendMessageW(lbl, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        cp->hwndSat = CreateWindowExW(0, TRACKBAR_CLASSW, L"",
            WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_NOTICKS,
            12, SL_Y+16, SLW, 26, hwnd, (HMENU)CP_ID_SAT_SLIDER, nullptr, nullptr);
        SendMessageW(cp->hwndSat, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
        SendMessageW(cp->hwndSat, TBM_SETPOS,   TRUE, (LPARAM)(int)(cp->s * 255));

        // Brightness label + full-width slider
        lbl = CreateWindowExW(0, L"STATIC", L"Brightness:",
            WS_CHILD|WS_VISIBLE,
            12, BR_Y, 100, 16, hwnd, nullptr, nullptr, nullptr);
        SendMessageW(lbl, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        cp->hwndVal = CreateWindowExW(0, TRACKBAR_CLASSW, L"",
            WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_NOTICKS,
            12, BR_Y+16, SLW, 26, hwnd, (HMENU)CP_ID_VAL_SLIDER, nullptr, nullptr);
        SendMessageW(cp->hwndVal, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
        SendMessageW(cp->hwndVal, TBM_SETPOS,   TRUE, (LPARAM)(int)(cp->v * 255));

        // Cancel button (bottom-left)
        HWND btnCancel = CreateWindowExW(0, L"BUTTON", L"Cancel",
            WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
            12, BTN_Y, 130, 34, hwnd, (HMENU)CP_ID_CANCEL, nullptr, nullptr);
        SendMessageW(btnCancel, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        // Choose button (bottom-right)
        HWND btnChoose = CreateWindowExW(0, L"BUTTON", L"Choose ✓",
            WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|WS_TABSTOP,
            WIN_W-142, BTN_Y, 130, 34, hwnd, (HMENU)CP_ID_CHOOSE, nullptr, nullptr);
        SendMessageW(btnChoose, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        // Build initial wheel bitmap
        HDC hdc = GetDC(hwnd);
        cp->wheelBmp = CreateColorWheelBitmap(hdc, cp->wheelSize, cp->v);
        ReleaseDC(hwnd, hdc);

        CP_UpdateAll(hwnd, cp);
        break;
    }
    case WM_HSCROLL: {
        if (!cp) break;
        HWND sldr = (HWND)lParam;
        if (sldr == cp->hwndSat) {
            cp->s = SendMessageW(cp->hwndSat, TBM_GETPOS, 0, 0) / 255.0f;
            CP_UpdateAll(hwnd, cp);
        } else if (sldr == cp->hwndVal) {
            cp->v = SendMessageW(cp->hwndVal, TBM_GETPOS, 0, 0) / 255.0f;
            CP_UpdateAll(hwnd, cp);
        }
        break;
    }
    case WM_COMMAND: {
        if (!cp) break;
        int id = LOWORD(wParam), notify = HIWORD(wParam);
        if (id == CP_ID_HEX_EDIT && notify == EN_KILLFOCUS) {
            wchar_t buf[16] = {};
            GetWindowTextW(cp->hwndHex, buf, 16);
            cp->updatingHex = false;
            CP_SetFromHex(hwnd, cp, buf);
        } else if (id == CP_ID_CHOOSE) {
            // Save result to shared globals before destroying
            g_cpResult = cp->result;
            g_cpOk = true;
            DestroyWindow(hwnd);
        } else if (id == CP_ID_CANCEL) {
            g_cpOk = false;
            DestroyWindow(hwnd);
        }
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE: {
        if (!cp) break;
        if (msg == WM_LBUTTONDOWN) { SetCapture(hwnd); cp->dragging = true; }
        if (!cp->dragging) break;
        int mx = GET_X_LPARAM(lParam), my = GET_Y_LPARAM(lParam);
        // Wheel sits at (12, 12)
        float cx = 12 + cp->wheelSize * 0.5f, cy = 12 + cp->wheelSize * 0.5f;
        float dx = mx - cx, dy = my - cy;
        float dist = sqrtf(dx*dx + dy*dy);
        float radius = cp->wheelSize * 0.5f - 1.0f;
        if (dist <= radius + 4.0f) {
            cp->h = fmodf(atan2f(dy, dx) * (180.0f / 3.14159265f) + 360.0f, 360.0f);
            cp->s = std::min(1.0f, dist / radius);
            if (cp->hwndSat) SendMessageW(cp->hwndSat, TBM_SETPOS, TRUE, (LPARAM)(int)(cp->s * 255));
            CP_UpdateAll(hwnd, cp);
        }
        break;
    }
    case WM_LBUTTONUP:
        cp->dragging = false;
        ReleaseCapture();
        break;
    case WM_PAINT: {
        if (!cp) break;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Background
        HBRUSH bgBrush = CreateSolidBrush(RGB(30, 30, 30));
        FillRect(hdc, &ps.rcPaint, bgBrush);
        DeleteObject(bgBrush);

        // Color wheel
        if (cp->wheelBmp) {
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP old = (HBITMAP)SelectObject(memDC, cp->wheelBmp);
            BitBlt(hdc, 12, 12, cp->wheelSize, cp->wheelSize, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, old);
            DeleteDC(memDC);

            float radius = cp->wheelSize * 0.5f - 1.0f;
            float ang = cp->h * 3.14159265f / 180.0f;
            float cxW = 12 + cp->wheelSize * 0.5f;
            float cyW = 12 + cp->wheelSize * 0.5f;
            int dotX = (int)(cxW + cosf(ang) * cp->s * radius);
            int dotY = (int)(cyW + sinf(ang) * cp->s * radius);

            // Hue direction line from center to edge
            HPEN linePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            HPEN oldLP = (HPEN)SelectObject(hdc, linePen);
            MoveToEx(hdc, (int)cxW, (int)cyW, nullptr);
            LineTo(hdc, (int)(cxW + cosf(ang) * radius), (int)(cyW + sinf(ang) * radius));
            SelectObject(hdc, oldLP);
            DeleteObject(linePen);

            // Selector dot: black outer, white inner
            HBRUSH nb = (HBRUSH)GetStockObject(NULL_BRUSH);
            HPEN penB = CreatePen(PS_SOLID, 3, RGB(0,0,0));
            HPEN op = (HPEN)SelectObject(hdc, penB);
            HBRUSH ob = (HBRUSH)SelectObject(hdc, nb);
            Ellipse(hdc, dotX-8, dotY-8, dotX+8, dotY+8);
            HPEN penW = CreatePen(PS_SOLID, 2, RGB(255,255,255));
            SelectObject(hdc, penW);
            Ellipse(hdc, dotX-6, dotY-6, dotX+6, dotY+6);
            SelectObject(hdc, op); SelectObject(hdc, ob);
            DeleteObject(penB); DeleteObject(penW);
        }

        // Preview box (top-right of wheel)
        const int PX2 = 12 + cp->wheelSize + 10;
        const int PW2 = 440 - PX2 - 8;
        RECT previewRect = { PX2, 12, PX2 + PW2, 72 };
        HBRUSH previewBrush = CreateSolidBrush(cp->result);
        FillRect(hdc, &previewRect, previewBrush);
        DeleteObject(previewBrush);
        // Border
        HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
        HPEN oldP2 = (HPEN)SelectObject(hdc, borderPen);
        HBRUSH nb2 = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH ob2 = (HBRUSH)SelectObject(hdc, nb2);
        Rectangle(hdc, previewRect.left, previewRect.top, previewRect.right, previewRect.bottom);
        SelectObject(hdc, oldP2); SelectObject(hdc, ob2);
        DeleteObject(borderPen);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(210, 210, 210));
        SetBkColor(hdc, RGB(30, 30, 30));
        static HBRUSH cpBg = CreateSolidBrush(RGB(30, 30, 30));
        return (INT_PTR)cpBg;
    }
    case WM_CTLCOLOREDIT: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(240, 240, 240));
        SetBkColor(hdc, RGB(50, 50, 50));
        static HBRUSH editBg = CreateSolidBrush(RGB(50, 50, 50));
        return (INT_PTR)editBg;
    }
    case WM_DESTROY:
        if (cp) {
            if (cp->wheelBmp) DeleteObject(cp->wheelBmp);
            delete cp;
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
        }
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Entry point: shows the dialog, returns true + fills outColor on OK
static bool ShowColorPickerDialog(HWND parent, COLORREF initialColor, COLORREF& outColor) {
    static bool classRegistered = false;
    if (!classRegistered) {
        WNDCLASSEXW wc = {};
        wc.cbSize        = sizeof(wc);
        wc.lpfnWndProc   = ColorPickerWndProc;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = CreateSolidBrush(RGB(30, 30, 30));
        wc.lpszClassName = L"DI_ColorPickerWnd";
        RegisterClassExW(&wc);
        classRegistered = true;
    }

    // Encode initial color as hex in the window title (read in WM_CREATE)
    wchar_t initTitle[16];
    swprintf_s(initTitle, L"#%02X%02X%02X",
        GetRValue(initialColor), GetGValue(initialColor), GetBValue(initialColor));

    HWND dlg = CreateWindowExW(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"DI_ColorPickerWnd", initTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 440, 460,
        parent, nullptr, GetModuleHandle(nullptr), nullptr);
    if (!dlg) return false;

    ShowWindow(dlg, SW_SHOW);
    UpdateWindow(dlg);

    // Modal message loop
    MSG m;
    bool ok = false;
    while (IsWindow(dlg) && GetMessageW(&m, nullptr, 0, 0) > 0) {
        if (!IsDialogMessageW(dlg, &m)) {
            TranslateMessage(&m);
            DispatchMessageW(&m);
        }
        // EndDialog sends WM_DESTROY; check if window gone
        if (!IsWindow(dlg)) break;
    }

    // Retrieve result via a global we set on IDOK
    // (We stored result on the state; recover before destroy)
    // Actually we need a different approach since WM_DESTROY already ran.
    // Use a shared static for simplicity (single-threaded UI).
    return ok;
}


static bool ShowColorPickerModal(HWND parent, COLORREF initial) {
    static bool classRegistered = false;
    if (!classRegistered) {
        WNDCLASSEXW wc = {};
        wc.cbSize        = sizeof(wc);
        wc.lpfnWndProc   = ColorPickerWndProc;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = CreateSolidBrush(RGB(30, 30, 30));
        wc.lpszClassName = L"DI_ColorPickerWnd";
        RegisterClassExW(&wc);
        classRegistered = true;
    }
    wchar_t initTitle[16];
    swprintf_s(initTitle, L"#%02X%02X%02X",
        GetRValue(initial), GetGValue(initial), GetBValue(initial));
    g_cpOk = false;
    g_cpResult = initial;

    HWND dlg = CreateWindowExW(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"DI_ColorPickerWnd", initTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 440, 460,
        parent, nullptr, GetModuleHandle(nullptr), nullptr);
    if (!dlg) return false;
    ShowWindow(dlg, SW_SHOW);
    UpdateWindow(dlg);

    // Modal loop — disable parent
    if (parent) EnableWindow(parent, FALSE);
    MSG m;
    while (IsWindow(dlg)) {
        if (GetMessageW(&m, nullptr, 0, 0) <= 0) break;
        if (!IsDialogMessageW(dlg, &m)) {
            TranslateMessage(&m);
            DispatchMessageW(&m);
        }
    }
    if (parent) EnableWindow(parent, TRUE);
    return g_cpOk;
}

// Override EndDialog in ColorPickerWndProc to store result
// We patch WM_COMMAND IDOK to save state before destroying:
// (Already handled above — let's wire g_cpResult there by adding
//  a helper called from IDOK below via subclassing approach.)
// Simpler: redirect via WM_APP:
#define WM_CP_ACCEPT (WM_APP + 50)


HWND g_hwndAccentMode = nullptr;
HWND g_hwndPos = nullptr;
HWND g_hwndScale = nullptr;
HWND g_hwndTheme = nullptr;
HWND g_hwndHide = nullptr;
HWND g_hwndMedia = nullptr;
HWND g_hwndClip = nullptr;
HWND g_hwndBat = nullptr;
HWND g_hwndProg = nullptr;
HWND g_hwndGame = nullptr;
HWND g_hwndMetric = nullptr;
HWND g_hwndDpi = nullptr;
HWND g_hwndOnTop = nullptr;
HWND g_hwndExpand = nullptr;
HWND g_hwndW11 = nullptr;
HWND g_hwndFahr = nullptr;
HWND g_hwndOffsetX = nullptr;
HWND g_hwndOffsetY = nullptr;
HWND g_hwndCity = nullptr;
HWND g_hwndColorful = nullptr;
HWND g_hwndExpandTrack = nullptr;
HWND g_hwndSplitMode = nullptr;
HWND g_hwndStartup = nullptr;
HWND g_hwndAirPodsMode = nullptr;

LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            BOOL dark = TRUE;
            DwmSetWindowAttribute(hwnd, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &dark, sizeof(dark));

            std::wstring pos = GetStringSettingCopy(L"Appearance.Position");
            std::wstring scale = GetStringSettingCopy(L"Appearance.SizeScale");
            int autoDpi = Wh_GetIntSetting(L"Appearance.AutoDpiScale");
            int media = Wh_GetIntSetting(L"Modules.Media");
            int clip = Wh_GetIntSetting(L"Modules.Clipboard");
            int bat = Wh_GetIntSetting(L"Modules.Battery");
            int prog = Wh_GetIntSetting(L"Modules.Progress");
            int game = Wh_GetIntSetting(L"Modules.GameOverlay");
            int metric = Wh_GetIntSetting(L"Modules.ShowMetricText");
            int fahr = Wh_GetIntSetting(L"Modules.WeatherFahrenheit");
            std::wstring city = GetStringSettingCopy(L"Modules.WeatherCity");
            std::wstring hide = GetStringSettingCopy(L"Appearance.AutoHideIdleSeconds");
            std::wstring accentColorMode = GetStringSettingCopy(L"Themes.AccentColorMode");
            if (accentColorMode.empty()) accentColorMode = L"auto";
            int onTop = Wh_GetIntSetting(L"Appearance.AlwaysOnTop");
            int expand = Wh_GetIntSetting(L"Appearance.ExpandOnHover");
            int w11 = Wh_GetIntSetting(L"Appearance.W11Style");
            int offsetX = Wh_GetIntSetting(L"Appearance.OffsetX");
            int offsetY = Wh_GetIntSetting(L"Appearance.OffsetY");
            int theme = Wh_GetIntValue(L"ColorTheme", 0);
            int colorful = Wh_GetIntSetting(L"Themes.ColorfulModules");
            int expandTrack = Wh_GetIntSetting(L"Appearance.ExpandOnTrackChange");
            int splitMode = Wh_GetIntSetting(L"Appearance.SplitMode");

            g_hFont = CreateFontW(-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                  VARIABLE_PITCH | FF_SWISS, L"Segoe UI");

            int x = 20, y = 20;
            int labelWidth = 120, controlWidth = 180;
            int rowHeight = 28;

            auto AddLabel = [&](const wchar_t* text, int xPos, int yPos, int w) {
                HWND h = CreateWindowExW(0, L"STATIC", text, WS_CHILD | WS_VISIBLE, xPos, yPos, w, 20, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                SendMessageW(h, WM_SETFONT, (WPARAM)g_hFont, TRUE);
                return h;
            };

            auto AddComboBox = [&](int xPos, int yPos, int w, int h) {
                HWND ctrl = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP, xPos, yPos, w, h, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                SendMessageW(ctrl, WM_SETFONT, (WPARAM)g_hFont, TRUE);
                return ctrl;
            };

            auto AddEdit = [&](const wchar_t* val, int xPos, int yPos, int w) {
                HWND ctrl = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", val, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, xPos, yPos, w, 22, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                SendMessageW(ctrl, WM_SETFONT, (WPARAM)g_hFont, TRUE);
                return ctrl;
            };

            auto AddCheckbox = [&](const wchar_t* text, bool checked, int xPos, int yPos, int w) {
                HWND ctrl = CreateWindowExW(0, L"BUTTON", text, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, xPos, yPos, w, 20, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                SendMessageW(ctrl, WM_SETFONT, (WPARAM)g_hFont, TRUE);
                SendMessage(ctrl, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0L);
                return ctrl;
            };

            // --- Column 1: Appearance & Colors ---
            AddLabel(L"Position:", x, y, labelWidth);
            g_hwndPos = AddComboBox(x + labelWidth, y, controlWidth, 200);
            SendMessageW(g_hwndPos, CB_ADDSTRING, 0, (LPARAM)L"Top Center");
            SendMessageW(g_hwndPos, CB_ADDSTRING, 0, (LPARAM)L"Top Left");
            SendMessageW(g_hwndPos, CB_ADDSTRING, 0, (LPARAM)L"Top Right");
            SendMessageW(g_hwndPos, CB_ADDSTRING, 0, (LPARAM)L"Bottom Center");
            if (pos == L"top-left") SendMessageW(g_hwndPos, CB_SETCURSEL, 1, 0);
            else if (pos == L"top-right") SendMessageW(g_hwndPos, CB_SETCURSEL, 2, 0);
            else if (pos == L"bottom-center") SendMessageW(g_hwndPos, CB_SETCURSEL, 3, 0);
            else SendMessageW(g_hwndPos, CB_SETCURSEL, 0, 0);

            y += rowHeight;
            AddLabel(L"Size Scale:", x, y, labelWidth);
            g_hwndScale = AddComboBox(x + labelWidth, y, controlWidth, 200);
            const wchar_t* scales[] = { L"0.8", L"1.0", L"1.2", L"1.5", L"1.8", L"2.0", L"2.5" };
            int scaleSel = 1;
            for (int i = 0; i < 7; i++) {
                SendMessageW(g_hwndScale, CB_ADDSTRING, 0, (LPARAM)scales[i]);
                if (scale == scales[i]) scaleSel = i;
            }
            SendMessageW(g_hwndScale, CB_SETCURSEL, scaleSel, 0);

            y += rowHeight;
            AddLabel(L"Theme Preset:", x, y, labelWidth);
            g_hwndTheme = AddComboBox(x + labelWidth, y, controlWidth, 200);
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Apple Style");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"OLED Black");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Dark Gray");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Midnight Blue");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Deep Purple");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Fluent Design");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Nothing OS");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Glassmorphism");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Cyberpunk");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Material You");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"AMOLED Dark");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Holographic AI");
            SendMessageW(g_hwndTheme, CB_ADDSTRING, 0, (LPARAM)L"Custom Color");
            SendMessageW(g_hwndTheme, CB_SETCURSEL, (theme >= 0 && theme < 12) ? theme : 12, 0);

            y += rowHeight;
            AddLabel(L"Auto-Hide Idle:", x, y, labelWidth);
            g_hwndHide = AddComboBox(x + labelWidth, y, controlWidth, 200);
            SendMessageW(g_hwndHide, CB_ADDSTRING, 0, (LPARAM)L"Never");
            SendMessageW(g_hwndHide, CB_ADDSTRING, 0, (LPARAM)L"Instant");
            SendMessageW(g_hwndHide, CB_ADDSTRING, 0, (LPARAM)L"5 Seconds");
            SendMessageW(g_hwndHide, CB_ADDSTRING, 0, (LPARAM)L"10 Seconds");
            SendMessageW(g_hwndHide, CB_ADDSTRING, 0, (LPARAM)L"30 Seconds");
            SendMessageW(g_hwndHide, CB_ADDSTRING, 0, (LPARAM)L"60 Seconds");
            int hideSel = 0;
            if (hide == L"-1") hideSel = 1;
            else if (hide == L"5") hideSel = 2;
            else if (hide == L"10") hideSel = 3;
            else if (hide == L"30") hideSel = 4;
            else if (hide == L"60") hideSel = 5;
            SendMessageW(g_hwndHide, CB_SETCURSEL, hideSel, 0);

            y += rowHeight;
            AddLabel(L"Offset X (px):", x, y, labelWidth);
            wchar_t offXStr[32];
            swprintf_s(offXStr, L"%d", offsetX);
            g_hwndOffsetX = AddEdit(offXStr, x + labelWidth, y, controlWidth);

            y += rowHeight;
            AddLabel(L"Offset Y (px):", x, y, labelWidth);
            wchar_t offYStr[32];
            swprintf_s(offYStr, L"%d", offsetY);
            g_hwndOffsetY = AddEdit(offYStr, x + labelWidth, y, controlWidth);

            y += rowHeight;
            AddLabel(L"Weather City:", x, y, labelWidth);
            g_hwndCity = AddEdit(city.c_str(), x + labelWidth, y, controlWidth);

            y += rowHeight;
            AddLabel(L"Accent Mode:", x, y, labelWidth);
            g_hwndAccentMode = AddComboBox(x + labelWidth, y, controlWidth, 200);
            SendMessageW(g_hwndAccentMode, CB_ADDSTRING, 0, (LPARAM)L"Auto (Album Art)");
            SendMessageW(g_hwndAccentMode, CB_ADDSTRING, 0, (LPARAM)L"System Accent");
            SendMessageW(g_hwndAccentMode, CB_ADDSTRING, 0, (LPARAM)L"Custom Color");
            if (EqualsNoCase(accentColorMode, L"system")) {
                SendMessageW(g_hwndAccentMode, CB_SETCURSEL, 1, 0);
            } else if (EqualsNoCase(accentColorMode, L"custom")) {
                SendMessageW(g_hwndAccentMode, CB_SETCURSEL, 2, 0);
            } else {
                SendMessageW(g_hwndAccentMode, CB_SETCURSEL, 0, 0);
            }

            // --- Column 2: Modules & Behavior ---
            int x2 = 350;
            int y2 = 20;
            int col2Width = 200;

            g_hwndMedia = AddCheckbox(L"Media Player Module", media != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndClip = AddCheckbox(L"Clipboard Module", clip != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndBat = AddCheckbox(L"Battery Module", bat != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndProg = AddCheckbox(L"Progress Module", prog != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndGame = AddCheckbox(L"Enable Game Overlay", game != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndMetric = AddCheckbox(L"Show Labels in Metric Chips", metric != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndDpi = AddCheckbox(L"Auto DPI Scaling", autoDpi != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndOnTop = AddCheckbox(L"Always On Top", onTop != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndExpand = AddCheckbox(L"Expand on Hover", expand != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndW11 = AddCheckbox(L"Native Windows 11 Style", w11 != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndFahr = AddCheckbox(L"Use Fahrenheit for Weather", fahr != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndColorful = AddCheckbox(L"Enable Multi-Color Modules", colorful != 0, x2, y2, col2Width);

            // --- Color Picker buttons (Column 1, after existing controls) ---
            // Find Y below the existing column-1 controls (after Weather City row)
            int ycp = y + rowHeight + 10;
            AddLabel(L"Accent Color:", x, ycp, labelWidth);
            std::wstring accentHex = GetStringSettingCopy(L"Themes.CustomAccentHex");
            if (accentHex.empty()) accentHex = L"#00E5FF";
            HWND g_hwndAccentEdit = AddEdit(accentHex.c_str(), x + labelWidth, ycp, 110);
            SetWindowLongPtrW(g_hwndAccentEdit, GWLP_ID, 2010);
            HWND btnAccent = CreateWindowExW(0, L"BUTTON", L"Pick",
                WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
                x + labelWidth + 115, ycp, 50, 22,
                hwnd, (HMENU)2011, GetModuleHandle(nullptr), nullptr);
            SendMessageW(btnAccent, WM_SETFONT, (WPARAM)g_hFont, TRUE);

            ycp += rowHeight;
            AddLabel(L"BG Color:", x, ycp, labelWidth);
            std::wstring bgHex = GetStringSettingCopy(L"Themes.PillBgColor");
            if (bgHex.empty()) bgHex = L"#0D0D0F";
            HWND g_hwndBgEdit = AddEdit(bgHex.c_str(), x + labelWidth, ycp, 110);
            SetWindowLongPtrW(g_hwndBgEdit, GWLP_ID, 2012);
            HWND btnBg = CreateWindowExW(0, L"BUTTON", L"Pick",
                WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,
                x + labelWidth + 115, ycp, 50, 22,
                hwnd, (HMENU)2013, GetModuleHandle(nullptr), nullptr);
            SendMessageW(btnBg, WM_SETFONT, (WPARAM)g_hFont, TRUE);

            ycp += rowHeight;
            AddLabel(L"AirPods Popup:", x, ycp, labelWidth);
            g_hwndAirPodsMode = AddComboBox(x + labelWidth, ycp, controlWidth, 200);
            SendMessageW(g_hwndAirPodsMode, CB_ADDSTRING, 0, (LPARAM)L"Both (Small / Hover Big)");
            SendMessageW(g_hwndAirPodsMode, CB_ADDSTRING, 0, (LPARAM)L"Small Version Only");
            SendMessageW(g_hwndAirPodsMode, CB_ADDSTRING, 0, (LPARAM)L"Bigger Version Only");
            std::wstring apMode = GetStringSettingCopy(L"Modules.AirPodsMode");
            if (EqualsNoCase(apMode, L"small")) {
                SendMessageW(g_hwndAirPodsMode, CB_SETCURSEL, 1, 0);
            } else if (EqualsNoCase(apMode, L"bigger")) {
                SendMessageW(g_hwndAirPodsMode, CB_SETCURSEL, 2, 0);
            } else {
                SendMessageW(g_hwndAirPodsMode, CB_SETCURSEL, 0, 0);
            }

            y2 += rowHeight - 4;
            g_hwndExpandTrack = AddCheckbox(L"Expand on Track Change", expandTrack != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndSplitMode = AddCheckbox(L"Enable Split Mode (Multiple Events)", splitMode != 0, x2, y2, col2Width);
            y2 += rowHeight - 4;
            g_hwndStartup = AddCheckbox(L"Start with Windows", IsStartupRegistryEnabled(), x2, y2, col2Width);

            // Buttons at the bottom
            int btnY = 385;
            HWND btnSave = CreateWindowExW(0, L"BUTTON", L"Save Settings", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP, 180, btnY, 110, 28, hwnd, (HMENU)IDOK, GetModuleHandle(nullptr), nullptr);
            SendMessageW(btnSave, WM_SETFONT, (WPARAM)g_hFont, TRUE);

            HWND btnCancel = CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 300, btnY, 110, 28, hwnd, (HMENU)IDCANCEL, GetModuleHandle(nullptr), nullptr);
            SendMessageW(btnCancel, WM_SETFONT, (WPARAM)g_hFont, TRUE);

            break;
        }
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(240, 240, 240));
            SetBkColor(hdc, RGB(30, 30, 30));
            static HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));
            return (INT_PTR)hBrush;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            if (wmId == IDOK) {
                wchar_t buf[256];

                int posSel = SendMessage(g_hwndPos, CB_GETCURSEL, 0, 0);
                const wchar_t* posVal = L"top-center";
                if (posSel == 1) posVal = L"top-left";
                else if (posSel == 2) posVal = L"top-right";
                else if (posSel == 3) posVal = L"bottom-center";
                WritePrivateProfileStringW(L"Settings", L"Appearance.Position", posVal, GetIniPath().c_str());

                int modeSel = static_cast<int>(SendMessage(g_hwndAccentMode, CB_GETCURSEL, 0, 0));
                const wchar_t* modeVal = L"auto";
                if (modeSel == 1) modeVal = L"system";
                else if (modeSel == 2) modeVal = L"custom";
                WritePrivateProfileStringW(L"Settings", L"Themes.AccentColorMode", modeVal, GetIniPath().c_str());

                int scaleSel = SendMessage(g_hwndScale, CB_GETCURSEL, 0, 0);
                const wchar_t* scales[] = { L"0.8", L"1.0", L"1.2", L"1.5", L"1.8", L"2.0", L"2.5" };
                if (scaleSel >= 0 && scaleSel < 7) {
                    WritePrivateProfileStringW(L"Settings", L"Appearance.SizeScale", scales[scaleSel], GetIniPath().c_str());
                }

                int themeSel = SendMessage(g_hwndTheme, CB_GETCURSEL, 0, 0);
                if (themeSel >= 0) {
                    wchar_t themeVal[16];
                    if (themeSel == 12) {
                        swprintf_s(themeVal, L"-1");
                    } else {
                        swprintf_s(themeVal, L"%d", themeSel);
                    }
                    WritePrivateProfileStringW(L"State", L"ColorTheme", themeVal, GetIniPath().c_str());
                }

                int hideSel = SendMessage(g_hwndHide, CB_GETCURSEL, 0, 0);
                const wchar_t* hideVal = L"0";
                if (hideSel == 1) hideVal = L"-1";
                else if (hideSel == 2) hideVal = L"5";
                else if (hideSel == 3) hideVal = L"10";
                else if (hideSel == 4) hideVal = L"30";
                else if (hideSel == 5) hideVal = L"60";
                WritePrivateProfileStringW(L"Settings", L"Appearance.AutoHideIdleSeconds", hideVal, GetIniPath().c_str());

                GetWindowTextW(g_hwndOffsetX, buf, 32);
                WritePrivateProfileStringW(L"Settings", L"Appearance.OffsetX", buf, GetIniPath().c_str());

                GetWindowTextW(g_hwndOffsetY, buf, 32);
                WritePrivateProfileStringW(L"Settings", L"Appearance.OffsetY", buf, GetIniPath().c_str());

                GetWindowTextW(g_hwndCity, buf, 256);
                WritePrivateProfileStringW(L"Settings", L"Modules.WeatherCity", buf, GetIniPath().c_str());

                int apModeSel = SendMessage(g_hwndAirPodsMode, CB_GETCURSEL, 0, 0);
                const wchar_t* apModeVal = L"both";
                if (apModeSel == 1) apModeVal = L"small";
                else if (apModeSel == 2) apModeVal = L"bigger";
                WritePrivateProfileStringW(L"Settings", L"Modules.AirPodsMode", apModeVal, GetIniPath().c_str());

                // Read and save the custom edit hex values directly
                HWND hwndAccEdit = GetDlgItem(hwnd, 2010);
                if (hwndAccEdit) {
                    wchar_t accHex[16] = {};
                    GetWindowTextW(hwndAccEdit, accHex, 16);
                    if (accHex[0]) {
                        WritePrivateProfileStringW(L"Settings", L"Themes.CustomAccentHex", accHex, GetIniPath().c_str());
                    }
                }
                HWND hwndBgEditCtrl = GetDlgItem(hwnd, 2012);
                if (hwndBgEditCtrl) {
                    wchar_t bgHexVal[16] = {};
                    GetWindowTextW(hwndBgEditCtrl, bgHexVal, 16);
                    if (bgHexVal[0]) {
                        WritePrivateProfileStringW(L"Settings", L"Themes.PillBgColor", bgHexVal, GetIniPath().c_str());
                    }
                }

                auto WriteCheck = [](HWND ctrl, const wchar_t* key, const wchar_t* section = L"Settings") {
                    bool checked = (SendMessage(ctrl, BM_GETCHECK, 0, 0) == BST_CHECKED);
                    WritePrivateProfileStringW(section, key, checked ? L"1" : L"0", GetIniPath().c_str());
                };
                WriteCheck(g_hwndMedia, L"Modules.Media");
                WriteCheck(g_hwndClip, L"Modules.Clipboard");
                WriteCheck(g_hwndBat, L"Modules.Battery");
                WriteCheck(g_hwndProg, L"Modules.Progress");
                WriteCheck(g_hwndGame, L"Modules.GameOverlay");
                WriteCheck(g_hwndMetric, L"Modules.ShowMetricText");
                WriteCheck(g_hwndDpi, L"Appearance.AutoDpiScale");
                WriteCheck(g_hwndOnTop, L"Appearance.AlwaysOnTop");
                WriteCheck(g_hwndExpand, L"Appearance.ExpandOnHover");
                WriteCheck(g_hwndW11, L"Appearance.W11Style");
                WriteCheck(g_hwndFahr, L"Modules.WeatherFahrenheit");
                WriteCheck(g_hwndColorful, L"Themes.ColorfulModules");
                WriteCheck(g_hwndExpandTrack, L"Appearance.ExpandOnTrackChange");
                WriteCheck(g_hwndSplitMode, L"Appearance.SplitMode");

                bool startupChecked = (SendMessage(g_hwndStartup, BM_GETCHECK, 0, 0) == BST_CHECKED);
                SetStartupRegistry(startupChecked);
                WriteCheck(g_hwndStartup, L"Appearance.StartWithWindows");

                LoadSettings();
                g_layoutDirty = true;
                if (g_hwnd) {
                    InvalidateRect(g_hwnd, nullptr, TRUE);
                }

                DestroyWindow(hwnd);
            } else if (wmId == IDCANCEL) {
                DestroyWindow(hwnd);
            } else if (wmId == 2011 || wmId == 2013) {
                // "Pick" button for Accent (2011) or BG (2013)
                bool isAccent = (wmId == 2011);
                const wchar_t* key = isAccent ? L"Themes.CustomAccentHex" : L"Themes.PillBgColor";
                const wchar_t* def = isAccent ? L"#00E5FF" : L"#0D0D0F";
                std::wstring cur = GetStringSettingCopy(key);
                if (cur.empty()) cur = def;
                // Parse hex to COLORREF
                const wchar_t* p = cur.c_str(); if (*p == L'#') ++p;
                int pr = 0, pg = 0, pb = 0;
                swscanf_s(p, L"%02X%02X%02X", &pr, &pg, &pb);
                COLORREF init = RGB(pr, pg, pb);
                // Re-implement modal via a simple helper that uses g_cpResult
                g_cpResult = init; g_cpOk = false;
                if (ShowColorPickerModal(hwnd, init)) {
                    wchar_t hexOut[16];
                    swprintf_s(hexOut, L"#%02X%02X%02X",
                        GetRValue(g_cpResult), GetGValue(g_cpResult), GetBValue(g_cpResult));
                    // Write the chosen color
                    WritePrivateProfileStringW(L"Settings", key, hexOut, GetIniPath().c_str());
                    // If BG color: switch theme preset combo to "Custom Color" and write ColorTheme = -1
                    if (!isAccent) {
                        WritePrivateProfileStringW(L"State", L"ColorTheme", L"-1", GetIniPath().c_str());
                        if (g_hwndTheme) {
                            SendMessageW(g_hwndTheme, CB_SETCURSEL, 12, 0);
                        }
                    }
                    // If accent color: also switch mode to "custom" so it takes effect
                    if (isAccent) {
                        WritePrivateProfileStringW(L"Settings", L"Themes.AccentColorMode",
                            L"custom", GetIniPath().c_str());
                        if (g_hwndAccentMode) {
                            SendMessageW(g_hwndAccentMode, CB_SETCURSEL, 2, 0);
                        }
                    }
                    // Update the matching edit box text
                    HWND editCtrl = GetDlgItem(hwnd, isAccent ? 2010 : 2012);
                    if (editCtrl) SetWindowTextW(editCtrl, hexOut);
                    // Reload settings + force immediate repaint of the island
                    LoadSettings();
                    g_layoutDirty = true;
                    if (g_hwnd) {
                        InvalidateRect(g_hwnd, nullptr, TRUE);
                        UpdateWindow(g_hwnd);
                    }
                }
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH brush = CreateSolidBrush(RGB(30, 30, 30));
            FillRect(hdc, &ps.rcPaint, brush);
            DeleteObject(brush);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            if (g_hFont) {
                DeleteObject(g_hFont);
                g_hFont = nullptr;
            }
            g_settingsHwnd = nullptr;
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void SetStartupRegistry(bool enable) {
    HKEY hKey = nullptr;
    LSTATUS status = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    if (status == ERROR_SUCCESS) {
        if (enable) {
            wchar_t path[MAX_PATH] = {};
            GetModuleFileNameW(nullptr, path, MAX_PATH);
            RegSetValueExW(hKey, L"DynamicIsland", 0, REG_SZ, reinterpret_cast<const BYTE*>(path), static_cast<DWORD>((wcslen(path) + 1) * sizeof(wchar_t)));
        } else {
            RegDeleteValueW(hKey, L"DynamicIsland");
        }
        RegCloseKey(hKey);
    }
}

bool IsStartupRegistryEnabled() {
    HKEY hKey = nullptr;
    bool enabled = false;
    LSTATUS status = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_QUERY_VALUE, &hKey);
    if (status == ERROR_SUCCESS) {
        status = RegQueryValueExW(hKey, L"DynamicIsland", nullptr, nullptr, nullptr, nullptr);
        if (status == ERROR_SUCCESS) {
            enabled = true;
        }
        RegCloseKey(hKey);
    }
    return enabled;
}

void OpenSettingsDialog(HWND parent) {
    if (g_settingsHwnd && IsWindow(g_settingsHwnd)) {
        SetForegroundWindow(g_settingsHwnd);
        return;
    }

    HINSTANCE hInst = GetModuleHandleW(nullptr);
    WNDCLASSEXW wcx = {};
    wcx.cbSize = sizeof(wcx);
    wcx.lpfnWndProc = SettingsWndProc;
    wcx.hInstance = hInst;
    wcx.lpszClassName = L"DynamicIslandSettingsClass";
    wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
    RegisterClassExW(&wcx);

    int width = 600, height = 465;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;

    g_settingsHwnd = CreateWindowExW(
        WS_EX_TOPMOST, L"DynamicIslandSettingsClass", L"Dynamic Island Settings",
        WS_POPUP | WS_CAPTION | WS_SYSMENU, x, y, width, height,
        nullptr, nullptr, hInst, nullptr);

    if (g_settingsHwnd) {
        ShowWindow(g_settingsHwnd, SW_SHOW);
        UpdateWindow(g_settingsHwnd);
    }
}

// Entry point for standalone application
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    // Check if another instance is already running
    HANDLE hMutex = CreateMutex(nullptr, TRUE, L"DynamicIslandStandaloneMutex");
    if (!hMutex) {
        return 1;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBoxW(nullptr, L"Dynamic Island is already running.", L"Dynamic Island", MB_OK | MB_ICONINFORMATION);
        CloseHandle(hMutex);
        return 0;
    }

    // Call standard mod initialization
    if (!WhTool_ModInit()) {
        MessageBoxW(nullptr, L"Failed to initialize Dynamic Island.", L"Error", MB_OK | MB_ICONERROR);
        CloseHandle(hMutex);
        return 1;
    }

    // Wait for the render thread to finish. The render thread handles window messages and shuts down when g_stopEvent is signaled.
    if (g_renderThread) {
        WaitForSingleObject(g_renderThread, INFINITE);
    }

    // Clean up resources and stop all threads
    WhTool_ModUninit();

    CloseHandle(hMutex);
    return 0;
}
#endif
