#pragma once
#include "contracts.h"
#include "context_manager.h"
#include <algorithm>
#include <string>

namespace ai {

// ============================================================================
// IntentRouter — fast keyword matching, ZERO network calls, instant results
//
// Trigger words recognized:
//
//  LAUNCH_APP:
//    "open <app>", "launch <app>", "start <app>", "run <app>", "show me <app>"
//    "go to youtube", "take me to google"
//
//  CLOSE_APP:
//    "close <app>", "exit <app>", "quit <app>", "kill <app>", "close it"
//
//  VOLUME_CONTROL:
//    "mute", "unmute", "volume up", "volume down",
//    "increase volume", "decrease volume", "louder", "quieter"
//
//  MEDIA_CONTROL:
//    "play", "pause", "next song", "skip", "previous", "next track"
//
//  POWER_CONTROL:
//    "sleep", "sleep pc", "sleep computer",
//    "shutdown", "shut down", "turn off",
//    "restart", "reboot"
//
//  Unknown → shows "I don't understand" message (NO API call)
// ============================================================================

class IntentRouter {
private:
    std::wstring ToLower(std::wstring str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    bool Contains(const std::wstring& str, const std::wstring& search) {
        return str.find(search) != std::wstring::npos;
    }

    bool StartsWith(const std::wstring& str, const std::wstring& prefix) {
        return str.rfind(prefix, 0) == 0;
    }

    // Strip a leading verb prefix and return the remainder
    std::wstring StripVerb(const std::wstring& text, const std::vector<std::wstring>& verbs) {
        for (const auto& verb : verbs) {
            if (StartsWith(text, verb + L" ")) {
                return text.substr(verb.size() + 1);
            }
        }
        return text;
    }

public:
    IntentResult Route(const std::wstring& text, const SessionContext& context = SessionContext()) {
        IntentResult result;
        result.originalText = text;
        result.confidence = 1.0f;
        result.useLLM = false;

        std::wstring t = ToLower(text);
        // Trim leading/trailing spaces
        size_t s = t.find_first_not_of(L' ');
        size_t e = t.find_last_not_of(L' ');
        if (s != std::wstring::npos) t = t.substr(s, e - s + 1);

        // ── 1. App Launching ──────────────────────────────────────────────────
        const std::vector<std::wstring> launchVerbs = {
            L"open", L"launch", L"start", L"run",
            L"show me", L"go to", L"take me to"
        };
        for (const auto& verb : launchVerbs) {
            if (StartsWith(t, verb + L" ")) {
                std::wstring target = t.substr(verb.size() + 1);
                result.intent.name = L"LAUNCH_APP";
                result.intent.target = target;
                return result;
            }
        }

        // ── 2. App Closing ────────────────────────────────────────────────────
        const std::vector<std::wstring> closeVerbs = {
            L"close", L"exit", L"quit", L"kill", L"shut"
        };
        for (const auto& verb : closeVerbs) {
            if (StartsWith(t, verb + L" ")) {
                std::wstring target = t.substr(verb.size() + 1);
                if ((target == L"it" || target == L"this" || target == L"that") &&
                    !context.lastOpenedApp.empty()) {
                    target = context.lastOpenedApp;
                }
                result.intent.name = L"CLOSE_APP";
                result.intent.target = target;
                return result;
            }
        }

        // ── 3. Volume Control ─────────────────────────────────────────────────
        if (Contains(t, L"volume") || t == L"mute" || t == L"unmute"
            || Contains(t, L"louder") || Contains(t, L"quieter")
            || Contains(t, L"silent")) {

            result.intent.name = L"VOLUME_CONTROL";
            result.intent.target = L"system_volume";

            if (Contains(t, L"mute") || Contains(t, L"silent")) {
                result.intent.parameters[L"action"] = L"mute";
            } else if (Contains(t, L"unmute")) {
                result.intent.parameters[L"action"] = L"mute"; // toggle
            } else if (Contains(t, L"up") || Contains(t, L"increase")
                || Contains(t, L"louder") || Contains(t, L"raise")) {
                result.intent.parameters[L"action"] = L"increase";
            } else if (Contains(t, L"down") || Contains(t, L"decrease")
                || Contains(t, L"quieter") || Contains(t, L"lower")) {
                result.intent.parameters[L"action"] = L"decrease";
            }
            return result;
        }

        // ── 4. Media Controls ─────────────────────────────────────────────────
        if (t == L"play" || t == L"resume") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"play";
            return result;
        }
        if (t == L"pause" || t == L"stop music" || t == L"stop song") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"pause";
            return result;
        }
        if (Contains(t, L"next") && (Contains(t, L"song") || Contains(t, L"track") || Contains(t, L"music"))
            || t == L"next" || t == L"skip") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"next";
            return result;
        }
        if (Contains(t, L"previous") || Contains(t, L"prev") || Contains(t, L"last song")) {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"previous";
            return result;
        }

        // ── 5. Power Controls ─────────────────────────────────────────────────
        if (Contains(t, L"lock")) {
            result.intent.name = L"POWER_CONTROL";
            result.intent.target = L"lock";
            return result;
        }
        if (Contains(t, L"sleep")) {
            result.intent.name = L"POWER_CONTROL";
            result.intent.target = L"sleep";
            return result;
        }
        if (Contains(t, L"shutdown") || Contains(t, L"shut down")
            || Contains(t, L"turn off") || Contains(t, L"power off")) {
            result.intent.name = L"POWER_CONTROL";
            result.intent.target = L"shutdown";
            return result;
        }
        if (Contains(t, L"restart") || Contains(t, L"reboot")) {
            result.intent.name = L"POWER_CONTROL";
            result.intent.target = L"restart";
            return result;
        }

        // ── 6. Unrecognized — NO cloud API call ───────────────────────────────
        result.intent.name = L"UNKNOWN";
        result.intent.target = L"";
        result.confidence = 0.0f;
        result.useLLM = false;  // ← was true before; disabled to stop API calls
        return result;
    }
};

} // namespace ai
