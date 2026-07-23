#pragma once
#include <windows.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <iostream>

#include <mutex>

namespace ai {

// ============================================================================
// 1. Audio Capture Interface & WASAPI Implementation
// ============================================================================

class IAudioCapture {
public:
    virtual ~IAudioCapture() = default;
    virtual bool StartRecording() = 0;
    virtual std::vector<unsigned char> StopRecording() = 0;
    virtual bool IsRecording() const = 0;
};

static const GUID KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_Local = {
    0x00000003,
    0x0000,
    0x0010,
    {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
};

class WasapiAudioCapture : public IAudioCapture {
private:
    std::atomic<bool> isRecording_{false};
    std::atomic<UINT32> capturedSampleRate_{48000};
    std::vector<float> capturedSamples_;
    std::mutex samplesMutex_;
    std::thread recordThread_;
    std::function<void(float)> pushWaveformCallback_;

    void RecordLoop() {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr)) return;

        IMMDeviceEnumerator* enumerator = nullptr;
        IMMDevice* device = nullptr;
        IAudioClient* audioClient = nullptr;
        IAudioCaptureClient* captureClient = nullptr;

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);
        if (FAILED(hr)) {
            CoUninitialize();
            return;
        }

        UINT32 sampleRate = 48000;
        WORD channels = 2;
        WORD bitsPerSample = 32;
        bool isFloat = true;

        hr = enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &device);
        if (SUCCEEDED(hr)) {
            hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&audioClient);
            if (SUCCEEDED(hr)) {
                WAVEFORMATEX* pwfx = nullptr;
                hr = audioClient->GetMixFormat(&pwfx);
                if (SUCCEEDED(hr)) {
                    sampleRate = pwfx->nSamplesPerSec;
                    channels = pwfx->nChannels;
                    bitsPerSample = pwfx->wBitsPerSample;
                    capturedSampleRate_ = sampleRate;
                    
                    if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
                        WAVEFORMATEXTENSIBLE* pExt = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(pwfx);
                        if (IsEqualGUID(pExt->SubFormat, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_Local)) {
                            isFloat = true;
                        } else {
                            isFloat = false;
                        }
                    } else if (pwfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
                        isFloat = true;
                    } else {
                        isFloat = false;
                    }

                    hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, pwfx, nullptr);
                    if (SUCCEEDED(hr)) {
                        hr = audioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&captureClient);
                    }
                    CoTaskMemFree(pwfx);
                }
            }
            device->Release();
        }
        enumerator->Release();

        if (FAILED(hr) || !audioClient || !captureClient) {
            if (captureClient) captureClient->Release();
            if (audioClient) audioClient->Release();
            CoUninitialize();
            return;
        }

        hr = audioClient->Start();
        if (FAILED(hr)) {
            captureClient->Release();
            audioClient->Release();
            CoUninitialize();
            return;
        }

        UINT32 bytesPerFrame = (channels * bitsPerSample) / 8;

        while (isRecording_) {
            UINT32 packetLength = 0;
            hr = captureClient->GetNextPacketSize(&packetLength);
            if (FAILED(hr)) break;

            while (packetLength > 0) {
                BYTE* pData = nullptr;
                UINT32 numFramesRead = 0;
                DWORD flags = 0;

                hr = captureClient->GetBuffer(&pData, &numFramesRead, &flags, nullptr, nullptr);
                if (FAILED(hr)) break;

                UINT32 byteCount = numFramesRead * bytesPerFrame;

                if (byteCount > 0 && pData) {
                    std::lock_guard<std::mutex> lock(samplesMutex_);
                    if (isFloat && bitsPerSample == 32) {
                        float* pFloatData = reinterpret_cast<float*>(pData);
                        for (UINT32 i = 0; i < numFramesRead; ++i) {
                            float sum = 0.0f;
                            for (WORD c = 0; c < channels; ++c) {
                                sum += pFloatData[i * channels + c];
                            }
                            capturedSamples_.push_back(sum / channels);
                        }
                    } else if (!isFloat && bitsPerSample == 16) {
                        short* pShortData = reinterpret_cast<short*>(pData);
                        for (UINT32 i = 0; i < numFramesRead; ++i) {
                            float sum = 0.0f;
                            for (WORD c = 0; c < channels; ++c) {
                                sum += pShortData[i * channels + c] / 32768.0f;
                            }
                            capturedSamples_.push_back(sum / channels);
                        }
                    } else if (!isFloat && bitsPerSample == 32) {
                        int32_t* pIntData = reinterpret_cast<int32_t*>(pData);
                        for (UINT32 i = 0; i < numFramesRead; ++i) {
                            float sum = 0.0f;
                            for (WORD c = 0; c < channels; ++c) {
                                sum += pIntData[i * channels + c] / 2147483648.0f;
                            }
                            capturedSamples_.push_back(sum / channels);
                        }
                    }

                    // Calculate real-time amplitude with Automatic Gain Control (AGC)
                    float rms = 0.0f;
                    float sumSq = 0.0f;
                    UINT32 startIdx = (capturedSamples_.size() >= numFramesRead) ? (capturedSamples_.size() - numFramesRead) : 0;
                    UINT32 counted = 0;
                    for (UINT32 i = startIdx; i < capturedSamples_.size(); ++i) {
                        float val = capturedSamples_[i];
                        sumSq += val * val;
                        counted++;
                    }
                    if (counted > 0) {
                        rms = std::sqrt(sumSq / counted);
                    }

                    float amplitude = 0.0f;
                    if (rms > 0.0001f) {
                        static float maxRms = 0.05f;
                        maxRms = maxRms * 0.98f + rms * 0.02f;
                        if (rms > maxRms) maxRms = rms;
                        if (maxRms < 0.01f) maxRms = 0.01f;

                        amplitude = rms / maxRms;
                        if (amplitude < 0.15f) amplitude = 0.15f;
                    } else {
                        amplitude = 0.05f;
                    }
                    if (amplitude > 1.0f) amplitude = 1.0f;
                    
                    if (pushWaveformCallback_) {
                        pushWaveformCallback_(amplitude);
                    }
                }

                captureClient->ReleaseBuffer(numFramesRead);
                hr = captureClient->GetNextPacketSize(&packetLength);
                if (FAILED(hr)) break;
            }
            Sleep(10);
        }

        audioClient->Stop();
        captureClient->Release();
        audioClient->Release();
        CoUninitialize();
    }

public:
    WasapiAudioCapture() = default;

    ~WasapiAudioCapture() override {
        if (isRecording_) {
            StopRecording();
        }
    }

    void SetPushWaveformCallback(std::function<void(float)> callback) {
        pushWaveformCallback_ = callback;
    }

    bool StartRecording() override {
        if (isRecording_) return false;

        {
            std::lock_guard<std::mutex> lock(samplesMutex_);
            capturedSamples_.clear();
        }
        isRecording_ = true;
        recordThread_ = std::thread(&WasapiAudioCapture::RecordLoop, this);
        return true;
    }

    std::vector<unsigned char> StopRecording() override {
        if (isRecording_) {
            isRecording_ = false;
            if (recordThread_.joinable()) {
                recordThread_.join();
            }
        }

        std::lock_guard<std::mutex> lock(samplesMutex_);
        std::vector<unsigned char> pcm16;
        if (capturedSamples_.empty() || capturedSampleRate_ == 0) {
            return pcm16;
        }

        double ratio = static_cast<double>(capturedSampleRate_.load()) / 16000.0;
        size_t outputLength = static_cast<size_t>(capturedSamples_.size() / ratio);
        if (outputLength == 0) return pcm16;

        pcm16.resize(outputLength * sizeof(int16_t));
        int16_t* pOut = reinterpret_cast<int16_t*>(pcm16.data());

        for (size_t i = 0; i < outputLength; ++i) {
            double srcIndex = i * ratio;
            size_t low = static_cast<size_t>(srcIndex);
            size_t high = low + 1;
            if (high >= capturedSamples_.size()) {
                high = capturedSamples_.size() - 1;
            }

            float sample = 0.0f;
            if (low < capturedSamples_.size()) {
                float t = static_cast<float>(srcIndex - low);
                sample = (1.0f - t) * capturedSamples_[low] + t * capturedSamples_[high];
            }

            if (sample > 1.0f) sample = 1.0f;
            if (sample < -1.0f) sample = -1.0f;
            pOut[i] = static_cast<int16_t>(sample * 32767.0f);
        }

        return pcm16;
    }

    bool IsRecording() const override {
        return isRecording_;
    }
};

// ============================================================================
// 2. Global Hotkey Push-to-Talk Keyboard Hook Manager
// ============================================================================

class PushToTalkManager {
private:
    static inline HHOOK hook_ = nullptr;
    static inline std::function<void()> onTrigger_;
    static inline std::function<void()> onRelease_;
    static inline bool isTriggered_ = false;

    static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0) {
            auto* kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            if (kbd->vkCode == VK_SPACE) {
                // Check if Ctrl key is held down
                bool ctrlHeld = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
                
                if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
                    if (ctrlHeld && !isTriggered_) {
                        isTriggered_ = true;
                        if (onTrigger_) onTrigger_();
                        return 1; // Swallow the spacebar press
                    }
                }
                else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
                    if (isTriggered_) {
                        isTriggered_ = false;
                        if (onRelease_) onRelease_();
                        return 1; // Swallow the spacebar release
                    }
                }
            }
        }
        return CallNextHookEx(hook_, nCode, wParam, lParam);
    }

public:
    static bool Install(std::function<void()> onTrigger, std::function<void()> onRelease) {
        if (hook_) return false;
        onTrigger_ = onTrigger;
        onRelease_ = onRelease;
        isTriggered_ = false;
        hook_ = SetWindowsHookExW(WH_KEYBOARD_LL, HookProc, nullptr, 0);
        return (hook_ != nullptr);
    }

    static void Uninstall() {
        if (hook_) {
            UnhookWindowsHookEx(hook_);
            hook_ = nullptr;
        }
        onTrigger_ = nullptr;
        onRelease_ = nullptr;
        isTriggered_ = false;
    }
};

} // namespace ai
