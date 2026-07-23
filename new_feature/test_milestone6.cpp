#include "contracts.h"
#include "event_bus.h"
#include "audio_capture.h"
#include "stt_service.h"
#include "intent_router.h"
#include "skills.h"
#include "ai_service.h"
#include "tts_service.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Global logger helper
void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

// Global console animator helper
void RenderIslandState(const std::wstring& stateName, const std::wstring& detail) {
    std::wstring border = L"==================================================";
    std::wstring pill;
    if (stateName == L"IDLE") {
        pill = L"   [   (•)   ] (Dynamic Island)";
    } else if (stateName == L"LISTENING") {
        pill = L"   [  🎤 Listening... ░▒▓▒░  ]";
    } else if (stateName == L"PROCESSING") {
        pill = L"   [  🧠 Understanding... ● ○ ●  ]";
    } else if (stateName == L"SUCCESS") {
        pill = L"   [  ✅ " + detail + L"  ]";
    }
    
    Log(L"\n" + border);
    Log(L"  Visual State: " + stateName);
    Log(pill);
    Log(border + L"\n");
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 6 Test ===");
    Log(L"Instructions: Hold down [Ctrl + Space] to speak. Release to trigger complete pipeline.");
    Log(L"Press [Escape] in this console to exit.");

    ai::EventBus eventBus;
    ai::WasapiAudioCapture recorder;
    ai::SapiSpeechToText sttService;
    ai::IntentRouter router;
    ai::OpenRouterAIService aiService;
    ai::SapiTextToSpeech ttsService;

    std::vector<std::unique_ptr<ai::ISkill>> skills;
    skills.push_back(std::make_unique<ai::AppsSkill>());
    skills.push_back(std::make_unique<ai::SystemSkill>());

    // Initialize Default State
    RenderIslandState(L"IDLE", L"");

    // Event Bus Bindings representing Orchestrator coordination
    eventBus.Subscribe(ai::EventType::VOICE_CAPTURED, [&](const ai::Event& e) {
        RenderIslandState(L"LISTENING", L"");
    });

    eventBus.Subscribe(ai::EventType::STT_COMPLETED, [&](const ai::Event& e) {
        RenderIslandState(L"PROCESSING", L"");
        Log(L"Speech Transcribed: \"" + e.payload + L"\"");

        // Run Router
        ai::IntentResult routeResult = router.Route(e.payload);
        eventBus.Publish(ai::EventType::INTENT_CLASSIFIED, routeResult.intent.name);

        bool skillExecuted = false;
        if (!routeResult.useLLM) {
            // Find local skill
            for (const auto& skill : skills) {
                if (skill->Supports(routeResult.intent)) {
                    ai::SkillResult res = skill->Execute(routeResult.intent);
                    if (res.success) {
                        skillExecuted = true;
                        RenderIslandState(L"SUCCESS", res.visualFeedback);
                        ttsService.Speak(res.voiceFeedback);
                        eventBus.Publish(ai::EventType::COMMAND_EXECUTED, res.visualFeedback);
                    }
                    break;
                }
            }
        }

        if (!skillExecuted) {
            // Fall back to Cloud LLM
            Log(L"Routing request to OpenRouter LLM...");
            ai::AIRequest req;
            req.prompt = e.payload;
            ai::AIResponse aiRes = aiService.Chat(req);
            
            RenderIslandState(L"SUCCESS", L"AI Response Ready");
            ttsService.Speak(aiRes.text);
            Log(L"AI Response text: " + aiRes.text);
            eventBus.Publish(ai::EventType::AI_RESPONSE_READY, aiRes.text);
        }
    });

    // Install Push-to-Talk callbacks
    bool installed = ai::PushToTalkManager::Install(
        [&]() {
            eventBus.Publish(ai::EventType::VOICE_CAPTURED, L"started");
            recorder.StartRecording();
        },
        [&]() {
            std::vector<unsigned char> audio = recorder.StopRecording();
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
    Log(L"=== Milestone 6 Test Complete ===");
    return 0;
}
