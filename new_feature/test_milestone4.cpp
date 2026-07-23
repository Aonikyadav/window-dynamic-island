#include "contracts.h"
#include "intent_router.h"
#include "skills.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 4 Test ===");

    ai::IntentRouter router;
    
    std::vector<std::unique_ptr<ai::ISkill>> skills;
    skills.push_back(std::make_unique<ai::AppsSkill>());
    skills.push_back(std::make_unique<ai::SystemSkill>());

    std::vector<std::wstring> testInputs = {
        L"Open Chrome",
        L"Close Spotify",
        L"Mute Volume",
        L"Sleep PC",
        L"Explain polymorphism in Java"
    };

    for (const auto& text : testInputs) {
        Log(L"--------------------------------------------------");
        Log(L"User Voice Input: \"" + text + L"\"");

        // 1. Run local intent routing
        ai::IntentResult res = router.Route(text);
        
        Log(L"  Routed Intent: " + res.intent.name);
        Log(L"  Target:        " + res.intent.target);
        Log(L"  Confidence:    " + std::to_wstring(res.confidence));
        Log(L"  Use Cloud LLM: " + std::wstring(res.useLLM ? L"YES" : L"NO"));

        // 2. Dispatch to skill executor
        bool skillFound = false;
        for (const auto& skill : skills) {
            if (skill->Supports(res.intent)) {
                skillFound = true;
                ai::SkillResult skillRes = skill->Execute(res.intent);
                
                Log(L"  [Local Action executed]");
                Log(L"    Visual: " + skillRes.visualFeedback);
                Log(L"    Voice:  " + skillRes.voiceFeedback);
                break;
            }
        }

        if (!skillFound) {
            if (res.useLLM) {
                Log(L"  [Cloud Action dispatched]");
                Log(L"    Routing to OpenRouter cloud services...");
            } else {
                Log(L"  [Error] No local skill found to handle this local intent.");
            }
        }
    }

    Log(L"--------------------------------------------------");
    Log(L"=== Milestone 4 Test Complete ===");
    return 0;
}
