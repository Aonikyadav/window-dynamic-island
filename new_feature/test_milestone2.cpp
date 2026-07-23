#include "contracts.h"
#include "event_bus.h"
#include "audio_capture.h"
#include <iostream>
#include <string>

void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 2 Test ===");
    Log(L"Instructions: Hold down [Ctrl + Space] to record audio. Release to stop.");
    Log(L"Press [Escape] in this console to exit.");

    ai::EventBus eventBus;
    ai::WasapiAudioCapture recorder;

    // Install Push-to-Talk callbacks
    bool installed = ai::PushToTalkManager::Install(
        [&]() {
            Log(L"PTT Triggered: Ctrl + Space Pressed! Starting WASAPI recording...");
            if (recorder.StartRecording()) {
                eventBus.Publish(ai::EventType::VOICE_CAPTURED, L"recording_started");
            } else {
                Log(L"Failed to start recording (WASAPI client error).");
            }
        },
        [&]() {
            Log(L"PTT Released: Ctrl + Space Keyup! Stopping WASAPI recording...");
            std::vector<unsigned char> audio = recorder.StopRecording();
            std::wstring sizeMsg = L"Recording finished! Captured bytes: " + std::to_wstring(audio.size());
            Log(sizeMsg);
            eventBus.Publish(ai::EventType::STT_COMPLETED, L"transcribing_mock_input");
        }
    );

    if (!installed) {
        Log(L"Error: Failed to install low-level keyboard hook!");
        return 1;
    }

    // Windows Message Loop (Required for Low-Level Keyboard Hooks to receive events)
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) {
            break;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    ai::PushToTalkManager::Uninstall();
    Log(L"PTT Hook uninstalled.");
    Log(L"=== Milestone 2 Test Complete ===");
    return 0;
}
