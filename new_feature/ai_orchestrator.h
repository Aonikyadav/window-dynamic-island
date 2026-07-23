#pragma once

#include "contracts.h"
#include "event_bus.h"
#include <iostream>
#include <string>

namespace ai {

inline void OrchestratorLog(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

class AIOrchestrator {
protected:
    EventBus& eventBus_;

public:
    AIOrchestrator(EventBus& eventBus) : eventBus_(eventBus) {
        // Subscribe the orchestrator coordinator to core events
        eventBus_.Subscribe(EventType::VOICE_CAPTURED, [this](const Event& e) {
            OnVoiceCaptured(e.payload);
        });
        eventBus_.Subscribe(EventType::STT_COMPLETED, [this](const Event& e) {
            OnSpeechTranscribed(e.payload);
        });
        eventBus_.Subscribe(EventType::INTENT_CLASSIFIED, [this](const Event& e) {
            OnIntentClassified(e.payload);
        });
    }

    virtual ~AIOrchestrator() = default;

protected:
    virtual void OnVoiceCaptured(const std::wstring& audioFilePath) {
        OrchestratorLog(L"[Orchestrator] VOICE_CAPTURED event received. File: " + audioFilePath);
        // In Milestone 1, we simulate triggering the next step (transcription) directly
        eventBus_.Publish(EventType::STT_COMPLETED, L"open chrome");
    }

    virtual void OnSpeechTranscribed(const std::wstring& text) {
        OrchestratorLog(L"[Orchestrator] STT_COMPLETED event received. Text: " + text);
        // Forwarding to router event
        eventBus_.Publish(EventType::INTENT_CLASSIFIED, L"LAUNCH_APP:chrome");
    }

    virtual void OnIntentClassified(const std::wstring& intentPayload) {
        OrchestratorLog(L"[Orchestrator] INTENT_CLASSIFIED event received. Payload: " + intentPayload);
        // Forwarding to command execution event
        eventBus_.Publish(EventType::COMMAND_EXECUTED, L"chrome opened successfully");
    }
};

} // namespace ai
