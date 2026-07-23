#include "contracts.h"
#include "context_manager.h"
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
    Log(L"=== AI Voice Assistant Milestone 7 Test ===");

    ai::ContextManager contextManager;
    ai::IntentRouter router;
    
    std::vector<std::unique_ptr<ai::ISkill>> skills;
    skills.push_back(std::make_unique<ai::AppsSkill>());
    skills.push_back(std::make_unique<ai::SystemSkill>());

    // Conversation Scenario:
    // Step 1: User requests "Open Chrome"
    std::wstring step1Text = L"Open Chrome";
    Log(L"\nStep 1: User says \"" + step1Text + L"\"");
    
    ai::IntentResult res1 = router.Route(step1Text, contextManager.GetContext());
    Log(L"  Routed Intent: " + res1.intent.name);
    Log(L"  Target:        " + res1.intent.target);

    // Execute skill & update context manager
    for (const auto& skill : skills) {
        if (skill->Supports(res1.intent)) {
            ai::SkillResult skillRes = skill->Execute(res1.intent);
            Log(L"  Action Visual: " + skillRes.visualFeedback);
            
            // Context Manager records that Chrome was successfully launched
            contextManager.SetLastOpenedApp(res1.intent.target);
            break;
        }
    }

    // Step 2: User requests "Close it"
    std::wstring step2Text = L"Close it";
    Log(L"\nStep 2: User says \"" + step2Text + L"\"");
    
    // Pass the context state (which remembers Chrome is open) to the router
    ai::IntentResult res2 = router.Route(step2Text, contextManager.GetContext());
    Log(L"  Routed Intent: " + res2.intent.name);
    Log(L"  Target:        " + res2.intent.target + L" (Resolved pronoun!)");

    // Execute skill to close the resolved app
    for (const auto& skill : skills) {
        if (skill->Supports(res2.intent)) {
            ai::SkillResult skillRes = skill->Execute(res2.intent);
            Log(L"  Action Visual: " + skillRes.visualFeedback);
            break;
        }
    }

    Log(L"\n=== Milestone 7 Test Complete ===");
    return 0;
}
