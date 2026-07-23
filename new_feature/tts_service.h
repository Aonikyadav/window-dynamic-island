#pragma once
#include <string>
#include <windows.h>
#include <sapi.h>
#include <mutex>

namespace ai {

// Define local GUIDs to prevent MinGW-w64 linker errors for SpVoice
static const GUID CLSID_SpVoice_Local = {0x96749377, 0x3391, 0x11D2, {0x9E, 0xE3, 0x00, 0xC0, 0x4F, 0x79, 0x73, 0x96}};
static const GUID IID_ISpVoice_Local = {0x6C44DF74, 0x72B9, 0x4992, {0xA1, 0xEC, 0xEF, 0x99, 0x6E, 0x04, 0x22, 0xD4}};

// ============================================================================
// Text-to-Speech Service Interface
// ============================================================================

class ITextToSpeechService {
public:
    virtual ~ITextToSpeechService() = default;
    virtual void Speak(const std::wstring& text) = 0;
    virtual void Stop() = 0;
};

// ============================================================================
// Windows SAPI Text-to-Speech Implementation
// ============================================================================

class SapiTextToSpeech : public ITextToSpeechService {
private:
    ISpVoice* voice_ = nullptr;
    std::mutex mtx_;

public:
    SapiTextToSpeech() {
        // Initialize voice instance once so it persists and can be interrupted
        HRESULT hr = CoCreateInstance(CLSID_SpVoice_Local, nullptr, CLSCTX_ALL, IID_ISpVoice_Local, (void**)&voice_);
        if (FAILED(hr)) {
            voice_ = nullptr;
        }
    }

    ~SapiTextToSpeech() override {
        if (voice_) {
            voice_->Release();
            voice_ = nullptr;
        }
    }

    void Speak(const std::wstring& text) override {
        if (text.empty() || !voice_) return;

        // Lock is intentionally not held during blocking Speak() call
        // because Stop() also needs the lock to purge it.
        // ISpVoice supports cross-thread Speak/Purge via COM free-threading.
        voice_->Speak(text.c_str(), 0, nullptr);
    }

    void Stop() override {
        if (voice_) {
            // SPF_PURGEBEFORESPEAK cancels current + queued speech immediately.
            // ISpVoice::Speak is thread-safe via COM when using CLSCTX_ALL.
            voice_->Speak(nullptr, SPF_PURGEBEFORESPEAK, nullptr);
        }
    }
};

} // namespace ai
