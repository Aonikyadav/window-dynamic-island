#include "contracts.h"
#include "event_bus.h"
#include "audio_capture.h"
#include "stt_service.h"
#include <iostream>
#include <string>

void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 3 Test ===");
    Log(L"Instructions: Hold down [Ctrl + Space] to record audio. Release to transcribe.");
    Log(L"Press [Escape] in this console to exit.");

    ai::EventBus eventBus;
    ai::WasapiAudioCapture recorder;
    ai::SapiSpeechToText sttService;

    // Connect Orchestration components to Event Bus
    eventBus.Subscribe(ai::EventType::STT_COMPLETED, [](const ai::Event& e) {
        Log(L"[Orchestrator Callback] STT_COMPLETED! Transcribed text: \"" + e.payload + L"\"");
    });

    // Install Push-to-Talk callbacks
    bool installed = ai::PushToTalkManager::Install(
        [&]() {
            Log(L"PTT Hook: Ctrl + Space Pressed! Recording...");
            recorder.StartRecording();
        },
        [&]() {
            Log(L"PTT Hook: Ctrl + Space Released! Transcribing captured audio buffer...");
            std::vector<unsigned char> audio = recorder.StopRecording();
            
            // Run transcription
            std::wstring text = sttService.Transcribe(audio);
            eventBus.Publish(ai::EventType::STT_COMPLETED, text);
        }
    );

    if (!installed) {
        Log(L"Error: Failed to install low-level keyboard hook!");
        return 1;
    }

    // Windows Message Loop
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
    Log(L"=== Milestone 3 Test Complete ===");
    return 0;
}
