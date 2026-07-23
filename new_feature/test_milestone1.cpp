#include "contracts.h"
#include "event_bus.h"
#include "ai_orchestrator.h"
#include <iostream>
#include <string>

void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 1 Test ===");

    ai::EventBus eventBus;
    ai::AIOrchestrator orchestrator(eventBus);

    // Register a mock UI subscription to verify the complete event chain
    eventBus.Subscribe(ai::EventType::COMMAND_EXECUTED, [](const ai::Event& e) {
        Log(L"[UI Event Listener] Received COMMAND_EXECUTED! Payload: " + e.payload);
    });

    // Fire the initial voice capture complete event
    Log(L"Firing VOICE_CAPTURED...");
    eventBus.Publish(ai::EventType::VOICE_CAPTURED, L"ptt_audio_capture.raw");

    Log(L"=== Milestone 1 Test Complete ===");
    return 0;
}
