#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace ai {

// ============================================================================
// 1. Intent & Routing Data Contracts
// ============================================================================

struct Intent {
    std::wstring name;                                      // e.g., L"LAUNCH_APP", L"VOLUME_CONTROL", L"CHAT_QUERY"
    std::wstring target;                                    // e.g., L"google chrome", L"bluetooth"
    std::map<std::wstring, std::wstring> parameters;        // Additional key-value pairs (e.g., L"level" -> L"60")
};

struct IntentResult {
    Intent intent;
    float confidence = 0.0f;                                // 0.0 to 1.0 confidence score
    bool useLLM = false;                                    // Fallback to cloud LLM if local confidence is low
    std::wstring originalText;                              // Raw speech text transcribed
};

// ============================================================================
// 2. Action & Execution Data Contracts
// ============================================================================

struct Action {
    std::wstring type;                                      // L"LAUNCH_APP", L"CLOSE_APP", L"SET_VOLUME", L"SET_TIMER"
    std::wstring target;
    std::map<std::wstring, std::wstring> parameters;
};

struct ActionPlan {
    std::vector<Action> actions;
    std::wstring voiceResponse;                             // Text to be read by TTS
    std::wstring visualResponse;                            // Text to be shown in Dynamic Island UI
};

struct SkillResult {
    bool success = false;
    std::wstring voiceFeedback;
    std::wstring visualFeedback;
};

// ============================================================================
// 3. AI Provider Message Wrappers
// ============================================================================

struct AIRequest {
    std::wstring prompt;
    std::wstring context;
    std::vector<unsigned char> audioData;                   // For Speech-to-Text input
};

struct AIResponse {
    std::wstring text;                                      // AI output text response
    std::vector<unsigned char> audioData;                   // Optional TTS synthesized output bytes
};

// ============================================================================
// 4. Decoupled Event System Contracts
// ============================================================================

enum class EventType {
    VOICE_CAPTURED,                                         // User finished push-to-talk recording
    STT_COMPLETED,                                          // Audio transcribed to plain text
    INTENT_CLASSIFIED,                                      // Local Intent Router resolved category/intent
    COMMAND_EXECUTED,                                       // System skill finished executing
    AI_RESPONSE_READY,                                      // Cloud LLM response received
    SPEECH_STARTED,                                         // TTS voice playback started
    SPEECH_FINISHED,                                        // TTS voice playback finished
    ERROR_OCCURRED                                          // Handled error event
};

struct Event {
    EventType type;
    std::wstring payload;                                   // Payload details (e.g., transcribed text or JSON parameters)
    double timestamp;                                       // Timestamp (seconds) when event was fired
};

} // namespace ai
