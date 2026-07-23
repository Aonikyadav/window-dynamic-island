#include "contracts.h"
#include "intent_router.h"
#include "skills.h"
#include "plugin_manager.h"
#include <iostream>
#include <memory>
#include <string>

void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 8 Test ===");
    Log(L"Demonstrating Dynamic Plugin Registration & Execution:");

    ai::IntentRouter router;
    ai::PluginManager pluginManager;

    // 1. Register Core Skills
    pluginManager.RegisterSkill(std::make_shared<ai::AppsSkill>());
    pluginManager.RegisterSkill(std::make_shared<ai::SystemSkill>());

    // 2. Register Third-Party Extension Skill dynamically
    pluginManager.RegisterSkill(std::make_shared<ai::CustomSpotifySkill>());

    // 3. Test list containing Core Intents and Third-Party Intents
    std::vector<std::wstring> testInputs = {
        L"Open Chrome",                                     // Core AppsSkill
        L"Mute Volume",                                     // Core SystemSkill
        L"Play Next Track"                                  // Third-Party CustomSpotifySkill!
    };

    // We manually map "play next track" locally to MUSIC_CONTROL for the mock router test
    auto CustomRoute = [&](const std::wstring& text) -> ai::IntentResult {
        if (text == L"Play Next Track") {
            ai::IntentResult res;
            res.intent.name = L"MUSIC_CONTROL";
            res.intent.parameters[L"action"] = L"next";
            res.confidence = 1.0f;
            res.useLLM = false;
            return res;
        }
        return router.Route(text);
    };

    for (const auto& text : testInputs) {
        Log(L"--------------------------------------------------");
        Log(L"Input Command: \"" + text + L"\"");

        ai::IntentResult routeRes = CustomRoute(text);
        Log(L"  Resolved Intent: " + routeRes.intent.name);

        // Dispatch intent through Plugin Manager
        ai::SkillResult execRes = pluginManager.Dispatch(routeRes.intent);

        if (execRes.success) {
            Log(L"  [Execution Success]");
            Log(L"    Visual: " + execRes.visualFeedback);
            Log(L"    Voice:  " + execRes.voiceFeedback);
        } else {
            Log(L"  [Execution Failed] " + execRes.voiceFeedback);
        }
    }

    Log(L"--------------------------------------------------");
    Log(L"=== Milestone 8 Test Complete ===");
    return 0;
}
