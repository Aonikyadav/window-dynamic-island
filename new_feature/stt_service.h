#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <atomic>
#include <mutex>
#include <functional>
#include <algorithm>
#include <thread>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.SpeechRecognition.h>

// Forward declaration of the global Windhawk logging stub
extern "C" void Wh_Log(const wchar_t* format, ...);

namespace WinSpeech = winrt::Windows::Media::SpeechRecognition;

namespace ai {

// ============================================================================
// Speech-to-Text Interface
// ============================================================================

class ISpeechToTextService {
public:
    virtual ~ISpeechToTextService() = default;
    virtual bool StartListening() = 0;
    virtual std::wstring StopListening() = 0;
};

// ============================================================================
// WinRT SpeechRecognizer Implementation
// Requires: Settings → Privacy & Security → Speech → Online speech recognition = ON
// ============================================================================

class WinrtSpeechToText : public ISpeechToTextService {
private:
    WinSpeech::SpeechRecognizer recognizer_{ nullptr };
    winrt::Windows::Foundation::IAsyncOperation<WinSpeech::SpeechRecognitionResult> activeOp_{ nullptr };
    std::atomic<bool> started_{ false };
    std::mutex mtx_;
    winrt::event_token stateToken_;

public:
    WinrtSpeechToText() = default;

    ~WinrtSpeechToText() {
        try {
            std::lock_guard<std::mutex> lock(mtx_);
            if (activeOp_) {
                activeOp_.Cancel();
                activeOp_ = nullptr;
            }
            if (recognizer_) {
                try { recognizer_.StateChanged(stateToken_); } catch (...) {}
                try { recognizer_.StopRecognitionAsync().get(); } catch (...) {}
                recognizer_ = nullptr;
            }
        } catch (...) {}
    }

    bool StartListening() override {
        std::lock_guard<std::mutex> lock(mtx_);
        // Cancel any previous session first
        if (activeOp_) {
            try { activeOp_.Cancel(); } catch (...) {}
            activeOp_ = nullptr;
        }
        if (recognizer_) {
            try { recognizer_.StateChanged(stateToken_); } catch (...) {}
            try { recognizer_.StopRecognitionAsync().get(); } catch (...) {}
            recognizer_ = nullptr;
        }

        try {
            try { winrt::init_apartment(winrt::apartment_type::multi_threaded); } catch (...) {}

            recognizer_ = WinSpeech::SpeechRecognizer();

            // Track state transitions for diagnostic log visibility
            stateToken_ = recognizer_.StateChanged([](WinSpeech::SpeechRecognizer const&, WinSpeech::SpeechRecognizerStateChangedEventArgs const& args) {
                Wh_Log(L"WinRT STT State: %d", (int)args.State());
            });

            auto topicConstraint = WinSpeech::SpeechRecognitionTopicConstraint(
                WinSpeech::SpeechRecognitionScenario::Dictation, L"");
            recognizer_.Constraints().Append(topicConstraint);

            WinSpeech::SpeechRecognitionCompilationResult compileResult =
                recognizer_.CompileConstraintsAsync().get();

            if (compileResult.Status() != WinSpeech::SpeechRecognitionResultStatus::Success) {
                Wh_Log(L"WinRT STT: CompileConstraints failed. Status = %d", (int)compileResult.Status());
                recognizer_ = nullptr;
                return false;
            }

            activeOp_ = recognizer_.RecognizeAsync();
            started_ = true;
            Wh_Log(L"WinRT STT: Started listening.");
            return true;

        } catch (winrt::hresult_error const& ex) {
            HRESULT hr = ex.to_abi();
            Wh_Log(L"WinRT STT: StartListening failed. hr = 0x%08X", hr);
            if (hr == (HRESULT)0x80045509) {
                Wh_Log(L"WinRT STT: Privacy policy not accepted. Enable Speech in Settings -> Privacy & Security -> Speech.");
            }
            recognizer_ = nullptr;
            return false;
        } catch (...) {
            Wh_Log(L"WinRT STT: StartListening unknown exception.");
            return false;
        }
    }

    // Wait for the dictation to complete naturally (silence detection)
    // NOTE: Must NOT be called while holding the mutex externally.
    std::wstring WaitForCompletion() {
        // Snapshot the op under the lock, then wait outside the lock to avoid deadlock
        winrt::Windows::Foundation::IAsyncOperation<WinSpeech::SpeechRecognitionResult> op{ nullptr };
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (!started_ || !activeOp_) {
                return L"";
            }
            op = activeOp_;
        }

        std::wstring result = L"";
        try {
            // Blocks current thread until user stops speaking (silence detected by WinRT)
            WinSpeech::SpeechRecognitionResult recResult = op.get();
            if (recResult != nullptr) {
                result = recResult.Text().c_str();
                Wh_Log(L"WinRT STT (AutoSilence): Recognized: '%s'", result.c_str());
                if (recResult.Status() != WinSpeech::SpeechRecognitionResultStatus::Success) {
                    result = L"";
                }
            }
        } catch (...) {
            Wh_Log(L"WinRT STT (AutoSilence): Error waiting for completion.");
        }

        {
            std::lock_guard<std::mutex> lock(mtx_);
            activeOp_ = nullptr;
            recognizer_ = nullptr;
            started_ = false;
        }
        return result;
    }

    std::wstring StopListening() override {
        // Snapshot op and recognizer under the lock, then wait outside to avoid deadlock
        winrt::Windows::Foundation::IAsyncOperation<WinSpeech::SpeechRecognitionResult> op{ nullptr };
        WinSpeech::SpeechRecognizer rec{ nullptr };
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (!started_ || !recognizer_ || !activeOp_) {
                Wh_Log(L"WinRT STT: StopListening called but not started.");
                return L"";
            }
            op = activeOp_;
            rec = recognizer_;
            activeOp_ = nullptr;
            recognizer_ = nullptr;
            started_ = false;
        }

        std::wstring result = L"";
        try {
            // Wait for transition out of Idle state if needed (max 500ms)
            int retries = 0;
            while (rec.State() == WinSpeech::SpeechRecognizerState::Idle && retries < 50) {
                Sleep(10);
                retries++;
            }

            Wh_Log(L"WinRT STT: StopListening State: %d", (int)rec.State());

            // Only call StopRecognitionAsync if the state is active/capturing/processing
            if (rec.State() != WinSpeech::SpeechRecognizerState::Idle) {
                try {
                    rec.StopRecognitionAsync().get();
                } catch (winrt::hresult_error const& ex) {
                    Wh_Log(L"WinRT STT: StopRecognitionAsync failed. hr = 0x%08X", ex.to_abi());
                } catch (...) {}
            } else {
                Wh_Log(L"WinRT STT: Recognizer was still Idle, canceling operation.");
                try { op.Cancel(); } catch (...) {}
            }

            WinSpeech::SpeechRecognitionResult recResult{ nullptr };
            try {
                recResult = op.get();
            } catch (winrt::hresult_error const& ex) {
                Wh_Log(L"WinRT STT: op.get() failed/canceled. hr = 0x%08X", ex.to_abi());
            } catch (...) {}

            if (recResult != nullptr) {
                result = recResult.Text().c_str();
                Wh_Log(L"WinRT STT: Recognized: '%s' (confidence=%d, status=%d)",
                    result.c_str(),
                    (int)recResult.Confidence(),
                    (int)recResult.Status());

                if (recResult.Status() != WinSpeech::SpeechRecognitionResultStatus::Success) {
                    Wh_Log(L"WinRT STT: Status not Success (%d), discarding result.", (int)recResult.Status());
                    result = L"";
                }
            }
        } catch (winrt::hresult_error const& ex) {
            Wh_Log(L"WinRT STT: StopListening outer exception. hr = 0x%08X, msg = %s",
                ex.to_abi(), ex.message().c_str());
        } catch (...) {
            Wh_Log(L"WinRT STT: StopListening unknown exception.");
        }

        return result;
    }
};

// ============================================================================
// WinRT Continuous Wake Word Listener
// ============================================================================

class WakeWordListener {
private:
    WinSpeech::SpeechRecognizer recognizer_{ nullptr };
    winrt::event_token resultToken_;
    winrt::event_token stateToken_;
    std::atomic<bool> isListening_{ false };
    std::wstring wakeWord_;
    std::mutex mtx_;
    std::function<void()> callback_ = nullptr;

public:
    WakeWordListener() = default;

    ~WakeWordListener() {
        Stop();
    }

    void SetCallback(std::function<void()> cb) {
        std::lock_guard<std::mutex> lock(mtx_);
        callback_ = cb;
    }

    bool Start(const std::wstring& wakeWord) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (isListening_) return true;

        try {
            try { winrt::init_apartment(winrt::apartment_type::multi_threaded); } catch (...) {}

            recognizer_ = WinSpeech::SpeechRecognizer();
            wakeWord_ = wakeWord;

            // Track continuous recognizer state changes
            stateToken_ = recognizer_.StateChanged([](WinSpeech::SpeechRecognizer const&, WinSpeech::SpeechRecognizerStateChangedEventArgs const& args) {
                try {
                    Wh_Log(L"WinRT Wake Word State: %d", (int)args.State());
                } catch (...) {}
            });

            // Normalize wake word
            std::wstring w = wakeWord;
            std::transform(w.begin(), w.end(), w.begin(), ::tolower);

            // Add the phrase and several common accents/prefixes/variations
            std::vector<winrt::hstring> list;
            list.push_back(winrt::hstring(w));

            if (w == L"hey jarvis" || w == L"jarvis") {
                list.push_back(winrt::hstring(L"jarvis"));
                list.push_back(winrt::hstring(L"hey jarvis"));
                list.push_back(winrt::hstring(L"hi jarvis"));
                list.push_back(winrt::hstring(L"hello jarvis"));
                list.push_back(winrt::hstring(L"jarves"));
                list.push_back(winrt::hstring(L"hey jarves"));
                list.push_back(winrt::hstring(L"hey jarve"));
                list.push_back(winrt::hstring(L"hi jarves"));
            } else {
                list.push_back(winrt::hstring(L"hey " + w));
                list.push_back(winrt::hstring(L"hi " + w));
                list.push_back(winrt::hstring(L"hello " + w));
            }

            WinSpeech::SpeechRecognitionListConstraint listConstraint(list);
            recognizer_.Constraints().Append(listConstraint);

            WinSpeech::SpeechRecognitionCompilationResult compileResult =
                recognizer_.CompileConstraintsAsync().get();

            if (compileResult.Status() != WinSpeech::SpeechRecognitionResultStatus::Success) {
                Wh_Log(L"WinRT Wake Word: Compile constraints failed. Status = %d", (int)compileResult.Status());
                recognizer_ = nullptr;
                return false;
            }

            // Register result handler
            resultToken_ = recognizer_.ContinuousRecognitionSession().ResultGenerated(
                [this](WinSpeech::SpeechContinuousRecognitionSession const&,
                       WinSpeech::SpeechContinuousRecognitionResultGeneratedEventArgs const& args) {
                    try {
                        if (args.Result().Status() == WinSpeech::SpeechRecognitionResultStatus::Success) {
                            std::wstring text = args.Result().Text().c_str();
                            Wh_Log(L"WinRT Wake Word: Heard '%s'", text.c_str());

                            std::wstring lower = text;
                            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

                            if (!lower.empty() && (lower.find(L"jarvis") != std::wstring::npos || lower.find(L"hey") != std::wstring::npos)) {
                                std::function<void()> cb;
                                {
                                    std::lock_guard<std::mutex> lock(mtx_);
                                    cb = callback_;
                                }
                                if (cb) cb();
                            }
                        }
                    } catch (...) {
                        Wh_Log(L"WinRT Wake Word: Exception in ResultGenerated handler.");
                    }
                }
            );

            recognizer_.ContinuousRecognitionSession().StartAsync().get();
            isListening_ = true;
            Wh_Log(L"WinRT Wake Word: Started continuous listening for '%s'", wakeWord.c_str());
            return true;

        } catch (winrt::hresult_error const& ex) {
            Wh_Log(L"WinRT Wake Word: Start failed. hr = 0x%08X", ex.to_abi());
            recognizer_ = nullptr;
            return false;
        } catch (...) {
            Wh_Log(L"WinRT Wake Word: Start failed (unknown exception).");
            recognizer_ = nullptr;
            return false;
        }
    }

    void Stop() {
        WinSpeech::SpeechRecognizer rec{ nullptr };
        winrt::event_token tok;
        winrt::event_token stateTok;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (!isListening_ || !recognizer_) return;
            rec = recognizer_;
            tok = resultToken_;
            stateTok = stateToken_;
            recognizer_ = nullptr;
            isListening_ = false;
        }

        std::thread([rec, tok, stateTok]() {
            try { winrt::init_apartment(winrt::apartment_type::multi_threaded); } catch (...) {}
            try { rec.ContinuousRecognitionSession().ResultGenerated(tok); } catch (...) {}
            try { rec.StateChanged(stateTok); } catch (...) {}
            try { rec.ContinuousRecognitionSession().StopAsync().get(); } catch (...) {}
            Wh_Log(L"WinRT Wake Word: Stopped continuous listening.");
        }).detach();
    }
};

// ============================================================================
// Real-time Speech to Text Converter Service (Live Dictation with Hypothesis)
// ============================================================================

class SttLiveDictationService {
private:
    WinSpeech::SpeechRecognizer recognizer_{ nullptr };
    std::atomic<bool> isListening_{ false };
    std::mutex mtx_;
    std::wstring fullTranscript_;
    std::wstring liveHypothesis_;
    std::function<void(const std::wstring&)> updateCallback_;
    winrt::event_token resultToken_;
    winrt::event_token hypToken_;

public:
    SttLiveDictationService() = default;
    ~SttLiveDictationService() { Stop(); }

    void SetCallback(std::function<void(const std::wstring&)> cb) {
        std::lock_guard<std::mutex> lock(mtx_);
        updateCallback_ = cb;
    }

    bool Start() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (isListening_) return true;

        try {
            try { winrt::init_apartment(winrt::apartment_type::multi_threaded); } catch (...) {}
            recognizer_ = WinSpeech::SpeechRecognizer();

            auto dictation = WinSpeech::SpeechRecognitionTopicConstraint(
                WinSpeech::SpeechRecognitionScenario::Dictation, L"");
            recognizer_.Constraints().Append(dictation);

            auto compileResult = recognizer_.CompileConstraintsAsync().get();
            if (compileResult.Status() != WinSpeech::SpeechRecognitionResultStatus::Success) {
                Wh_Log(L"WinRT Live STT: CompileConstraints failed.");
                recognizer_ = nullptr;
                return false;
            }

            fullTranscript_ = L"";
            liveHypothesis_ = L"";

            hypToken_ = recognizer_.HypothesisGenerated(
                [this](auto const&, WinSpeech::SpeechRecognitionHypothesisGeneratedEventArgs const& args) {
                    try {
                        std::wstring hyp = args.Hypothesis().Text().c_str();
                        if (!hyp.empty()) {
                            std::wstring current;
                            {
                                std::lock_guard<std::mutex> lock(mtx_);
                                liveHypothesis_ = hyp;
                                current = fullTranscript_;
                                if (!current.empty() && !liveHypothesis_.empty()) current += L" ";
                                current += liveHypothesis_;
                                lastSpeechTick_ = GetTickCount64();
                            }
                            std::function<void(const std::wstring&)> cb;
                            {
                                std::lock_guard<std::mutex> lock(mtx_);
                                cb = updateCallback_;
                            }
                            if (cb) cb(current);
                        }
                    } catch (...) {}
                }
            );

            resultToken_ = recognizer_.ContinuousRecognitionSession().ResultGenerated(
                [this](auto const&, WinSpeech::SpeechContinuousRecognitionResultGeneratedEventArgs const& args) {
                    try {
                        if (args.Result().Status() == WinSpeech::SpeechRecognitionResultStatus::Success) {
                            std::wstring text = args.Result().Text().c_str();
                            if (!text.empty()) {
                                std::wstring current;
                                {
                                    std::lock_guard<std::mutex> lock(mtx_);
                                    liveHypothesis_ = L"";
                                    if (!fullTranscript_.empty()) fullTranscript_ += L" ";
                                    fullTranscript_ += text;
                                    current = fullTranscript_;
                                    lastSpeechTick_ = GetTickCount64();
                                }
                                Wh_Log(L"WinRT Live STT: Committed phrase '%s'", text.c_str());
                                std::function<void(const std::wstring&)> cb;
                                {
                                    std::lock_guard<std::mutex> lock(mtx_);
                                    cb = updateCallback_;
                                }
                                if (cb) cb(current);
                            }
                        }
                    } catch (...) {}
                }
            );

            recognizer_.ContinuousRecognitionSession().StartAsync().get();
            isListening_ = true;
            lastSpeechTick_ = GetTickCount64();
            Wh_Log(L"WinRT Live STT: Started real-time dictation with hypothesis streaming.");
            return true;
        } catch (winrt::hresult_error const& ex) {
            Wh_Log(L"WinRT Live STT: Start failed. hr = 0x%08X", ex.to_abi());
            recognizer_ = nullptr;
            return false;
        } catch (...) {
            Wh_Log(L"WinRT Live STT: Start failed.");
            recognizer_ = nullptr;
            return false;
        }
    }

    void Stop() {
        WinSpeech::SpeechRecognizer rec{ nullptr };
        winrt::event_token resTok;
        winrt::event_token hypTok;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (!isListening_ || !recognizer_) return;
            rec = recognizer_;
            resTok = resultToken_;
            hypTok = hypToken_;
            recognizer_ = nullptr;
            isListening_ = false;
        }

        std::thread([rec, resTok, hypTok]() {
            try { winrt::init_apartment(winrt::apartment_type::multi_threaded); } catch (...) {}
            try { rec.ContinuousRecognitionSession().ResultGenerated(resTok); } catch (...) {}
            try { rec.HypothesisGenerated(hypTok); } catch (...) {}
            try { rec.ContinuousRecognitionSession().StopAsync().get(); } catch (...) {}
            Wh_Log(L"WinRT Live STT: Stopped real-time dictation.");
        }).detach();
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mtx_);
        fullTranscript_ = L"";
        liveHypothesis_ = L"";
        lastSpeechTick_ = GetTickCount64();
        if (updateCallback_) updateCallback_(L"");
    }

    std::wstring GetText() {
        std::lock_guard<std::mutex> lock(mtx_);
        return fullTranscript_;
    }

    ULONGLONG GetLastSpeechTick() {
        std::lock_guard<std::mutex> lock(mtx_);
        return lastSpeechTick_;
    }

private:
    ULONGLONG lastSpeechTick_{ 0 };
};

// Legacy alias so any remaining code that uses SapiSpeechToText still compiles
using SapiSpeechToText = WinrtSpeechToText;

} // namespace ai
