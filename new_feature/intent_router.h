#pragma once
#include "contracts.h"
#include "context_manager.h"
#include <algorithm>
#include <string>
#include <vector>

namespace ai {

// ============================================================================
// IntentRouter — Natural Speech Command Routing (Zero Network Delay)
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

public:
    IntentResult Route(const std::wstring& text, const SessionContext& context = SessionContext()) {
        IntentResult result;
        result.originalText = text;
        result.confidence = 1.0f;
        result.useLLM = false;

        std::wstring t = ToLower(text);

        // Trim leading/trailing whitespace & punctuation
        size_t s = t.find_first_not_of(L" \t\r\n.,!?");
        size_t e = t.find_last_not_of(L" \t\r\n.,!?");
        if (s != std::wstring::npos) {
            t = t.substr(s, e - s + 1);
        } else {
            t = L"";
        }

        if (t.empty()) {
            result.intent.name = L"UNKNOWN";
            return result;
        }

        // ── 0. Assistant Self-Controls ─────────────────────────────────────────
        if (t == L"stop speaking" || t == L"quiet" || t == L"be quiet" || t == L"shut up") {
            result.intent.name = L"ASSISTANT_CONTROL";
            result.intent.target = L"stop_speaking";
            return result;
        }
        if (t == L"stop listening" || t == L"cancel" || t == L"go to sleep") {
            result.intent.name = L"ASSISTANT_CONTROL";
            result.intent.target = L"stop_listening";
            return result;
        }

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
            L"close", L"exit", L"quit", L"kill", L"terminate"
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

        // ── 3. Volume Controls ────────────────────────────────────────────────
        if (Contains(t, L"volume") || t == L"mute" || t == L"unmute"
            || Contains(t, L"louder") || Contains(t, L"quieter") || Contains(t, L"silent")
            || Contains(t, L"turn sound")) {

            result.intent.name = L"VOLUME_CONTROL";
            result.intent.target = L"system_volume";

            if (Contains(t, L"unmute") || t == L"turn sound on") {
                result.intent.parameters[L"action"] = L"unmute";
            } else if (Contains(t, L"mute") || Contains(t, L"silent") || t == L"turn sound off") {
                result.intent.parameters[L"action"] = L"mute";
            } else if (Contains(t, L"up") || Contains(t, L"increase") || Contains(t, L"louder") || Contains(t, L"raise") || Contains(t, L"make it louder")) {
                result.intent.parameters[L"action"] = L"increase";
            } else if (Contains(t, L"down") || Contains(t, L"decrease") || Contains(t, L"quieter") || Contains(t, L"lower")) {
                result.intent.parameters[L"action"] = L"decrease";
            }
            return result;
        }

        // ── 4. Media Controls ─────────────────────────────────────────────────
        if (t == L"play" || t == L"resume" || t == L"continue") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"play";
            return result;
        }
        if (t == L"pause" || t == L"stop music" || t == L"stop song") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"pause";
            return result;
        }
        if (Contains(t, L"next") || t == L"skip") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"next";
            return result;
        }
        if (Contains(t, L"previous") || Contains(t, L"prev") || t == L"last song" || t == L"go back") {
            result.intent.name = L"MEDIA_CONTROL";
            result.intent.parameters[L"action"] = L"previous";
            return result;
        }

        // ── 5. Brightness Controls ─────────────────────────────────────────────
        if (Contains(t, L"brightness") || Contains(t, L"brighter") || Contains(t, L"dim screen")) {
            result.intent.name = L"BRIGHTNESS_CONTROL";
            if (Contains(t, L"up") || Contains(t, L"increase") || Contains(t, L"brighter")) {
                result.intent.parameters[L"action"] = L"increase";
            } else {
                result.intent.parameters[L"action"] = L"decrease";
            }
            return result;
        }

        // ── 6. Battery & Power Controls ────────────────────────────────────────
        if (Contains(t, L"battery") || Contains(t, L"charging") || Contains(t, L"laptop battery")) {
            result.intent.name = L"BATTERY_CONTROL";
            return result;
        }
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
        if (Contains(t, L"shutdown") || Contains(t, L"shut down") || Contains(t, L"turn off computer") || Contains(t, L"power off")) {
            result.intent.name = L"POWER_CONTROL";
            result.intent.target = L"shutdown";
            return result;
        }
        if (Contains(t, L"restart") || Contains(t, L"reboot")) {
            result.intent.name = L"POWER_CONTROL";
            result.intent.target = L"restart";
            return result;
        }

        // ── 7. Hardware & Network (Wi-Fi, Bluetooth, AirPods) ───────────────────
        if (Contains(t, L"wifi") || Contains(t, L"wi-fi")) {
            result.intent.name = L"NETWORK_CONTROL";
            result.intent.target = L"wifi";
            result.intent.parameters[L"action"] = (Contains(t, L"off") || Contains(t, L"disable")) ? L"off" : L"on";
            return result;
        }
        if (Contains(t, L"bluetooth")) {
            result.intent.name = L"NETWORK_CONTROL";
            result.intent.target = L"bluetooth";
            result.intent.parameters[L"action"] = (Contains(t, L"off") || Contains(t, L"disable")) ? L"off" : L"on";
            return result;
        }
        if (Contains(t, L"airpods")) {
            result.intent.name = L"AIRPODS_CONTROL";
            if (Contains(t, L"disconnect")) result.intent.parameters[L"action"] = L"disconnect";
            else if (Contains(t, L"connect")) result.intent.parameters[L"action"] = L"connect";
            else result.intent.parameters[L"action"] = L"status";
            return result;
        }

        // ── 8. Clipboard Controls ──────────────────────────────────────────────
        if (Contains(t, L"clipboard")) {
            result.intent.name = L"CLIPBOARD_CONTROL";
            if (Contains(t, L"clear")) result.intent.parameters[L"action"] = L"clear";
            else result.intent.parameters[L"action"] = L"show";
            return result;
        }

        // ── 9. Unrecognized — Return UNKNOWN (No Cloud API Delay) ──────────────
        result.intent.name = L"UNKNOWN";
        result.intent.target = L"";
        result.confidence = 0.0f;
        result.useLLM = false;
        return result;
    }
};

} // namespace ai
