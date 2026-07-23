#pragma once
#include "contracts.h"
#include <windows.h>
#include <string>
#include <shellapi.h>
#include <map>

#include <functional>

namespace ai {

// ============================================================================
// ISkill Interface
// ============================================================================

class ISkill {
public:
    virtual ~ISkill() = default;
    virtual bool Supports(const Intent& intent) = 0;
    virtual SkillResult Execute(const Intent& intent) = 0;
};

// ============================================================================
// AppsSkill - Launches/closes apps by name
// Supported commands:
//   "open chrome", "launch chrome"
//   "open spotify", "launch spotify"
//   "open notepad", "open calculator", "open settings", "open task manager"
//   "open vs code", "open code", "open explorer", "open files"
//   "open youtube", "open google"
//   "close chrome", "close it"
// ============================================================================

class AppsSkill : public ISkill {
private:
    struct AppInfo {
        std::wstring exe;    // executable to run with ShellExecute
        std::wstring uri;    // optional URI (e.g. for browser sites)
    };

    // Maps recognized app name → launch command
    // Key is lowercase alias. Searched from longest to first match.
    std::map<std::wstring, AppInfo> apps_ = {
        // Browsers
        {L"chrome",             {L"chrome.exe",         L""}},
        {L"google chrome",      {L"chrome.exe",         L""}},
        {L"web browser",        {L"chrome.exe",         L""}},
        {L"browser",            {L"chrome.exe",         L""}},
        {L"internet",           {L"chrome.exe",         L""}},
        {L"firefox",            {L"firefox.exe",        L""}},
        {L"edge",               {L"msedge.exe",         L""}},
        {L"microsoft edge",     {L"msedge.exe",         L""}},
        {L"brave",              {L"brave.exe",          L""}},

        // Web shortcuts (launch in default browser)
        {L"youtube",            {L"",                   L"https://www.youtube.com"}},
        {L"google",             {L"",                   L"https://www.google.com"}},
        {L"github",             {L"",                   L"https://github.com"}},
        {L"chatgpt",            {L"",                   L"https://chat.openai.com"}},
        {L"chat gpt",           {L"",                   L"https://chat.openai.com"}},

        // Media & Music
        {L"spotify",            {L"spotify.exe",        L""}},
        {L"music",              {L"spotify.exe",        L""}},
        {L"music player",       {L"spotify.exe",        L""}},
        {L"audio player",       {L"spotify.exe",        L""}},

        // Development tools
        {L"vs code",            {L"code.cmd",           L""}},
        {L"vscode",             {L"code.cmd",           L""}},
        {L"visual studio code", {L"code.cmd",           L""}},
        {L"code editor",        {L"code.cmd",           L""}},
        {L"code",               {L"code.cmd",           L""}},
        {L"visual studio",      {L"devenv.exe",         L""}},
        {L"notepad",            {L"notepad.exe",        L""}},
        {L"notepad plus",       {L"notepad++.exe",      L""}},
        {L"notepad++",          {L"notepad++.exe",      L""}},
        {L"cmd",                {L"cmd.exe",            L""}},
        {L"command prompt",     {L"cmd.exe",            L""}},
        {L"terminal",           {L"wt.exe",             L""}},
        {L"console",            {L"cmd.exe",            L""}},
        {L"powershell",         {L"powershell.exe",     L""}},

        // Windows built-ins & System Tools
        {L"calculator",         {L"calc.exe",           L""}},
        {L"calc",               {L"calc.exe",           L""}},
        {L"math",               {L"calc.exe",           L""}},
        {L"paint",              {L"mspaint.exe",        L""}},
        {L"word pad",           {L"wordpad.exe",        L""}},
        {L"wordpad",            {L"wordpad.exe",        L""}},
        {L"explorer",           {L"explorer.exe",       L""}},
        {L"file explorer",      {L"explorer.exe",       L""}},
        {L"files",              {L"explorer.exe",       L""}},
        {L"my computer",        {L"explorer.exe",       L""}},
        {L"this pc",            {L"explorer.exe",       L""}},
        {L"settings",           {L"ms-settings:",       L""}},
        {L"windows settings",   {L"ms-settings:",       L""}},
        {L"system settings",    {L"ms-settings:",       L""}},
        {L"task manager",       {L"taskmgr.exe",        L""}},
        {L"taskmgr",            {L"taskmgr.exe",        L""}},
        {L"activity monitor",   {L"taskmgr.exe",        L""}},
        {L"system monitor",     {L"taskmgr.exe",        L""}},
        {L"clock",              {L"ms-clock:",          L""}},
        {L"camera",             {L"microsoft.windows.camera:", L""}},
        {L"store",              {L"ms-windows-store:",  L""}},
        {L"microsoft store",    {L"ms-windows-store:",  L""}},
        {L"mail",               {L"outlookmail:",       L""}},
        {L"snipping tool",      {L"snippingtool.exe",   L""}},
        {L"snip",               {L"snippingtool.exe",   L""}},
        {L"control panel",      {L"control.exe",        L""}},
        {L"disk management",    {L"diskmgmt.msc",       L""}},
        {L"device manager",     {L"devmgmt.msc",        L""}},
        {L"registry editor",    {L"regedit.exe",        L""}},
        {L"regedit",            {L"regedit.exe",        L""}},
    };

    std::wstring ToLower(std::wstring s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:
    inline static std::function<void()> onStartStt = nullptr;
    inline static std::function<void(int)> onSetMiniPill = nullptr;

    bool Supports(const Intent& intent) override {
        return (intent.name == L"LAUNCH_APP" || intent.name == L"CLOSE_APP");
    }

    SkillResult Execute(const Intent& intent) override {
        SkillResult result;
        result.success = false;

        if (intent.name == L"LAUNCH_APP") {
            std::wstring app = ToLower(intent.target);

            if (app == L"speech to text" || app == L"stt" || app == L"speech-to-text") {
                if (onStartStt) onStartStt();
                result.success = true;
                result.voiceFeedback = L"Opening Speech to Text";
                result.visualFeedback = L"Speech-to-Text";
                return result;
            }
            if (app == L"calendar") {
                if (onSetMiniPill) onSetMiniPill(6);
                result.success = true;
                result.voiceFeedback = L"Opening Calendar";
                result.visualFeedback = L"Calendar";
                return result;
            }
            if (app == L"weather") {
                if (onSetMiniPill) onSetMiniPill(7);
                result.success = true;
                result.voiceFeedback = L"Opening Weather";
                result.visualFeedback = L"Weather";
                return result;
            }
            if (app == L"battery") {
                if (onSetMiniPill) onSetMiniPill(3);
                result.success = true;
                result.voiceFeedback = L"Opening Battery";
                result.visualFeedback = L"Battery";
                return result;
            }

            // Find best match (longest key match wins)
            const AppInfo* best = nullptr;
            size_t bestLen = 0;
            for (auto& [key, info] : apps_) {
                if (app.find(key) != std::wstring::npos && key.size() > bestLen) {
                    best = &info;
                    bestLen = key.size();
                }
            }

            if (best) {
                HINSTANCE inst;
                if (!best->uri.empty()) {
                    // Open URL in default browser
                    inst = ShellExecuteW(nullptr, L"open", best->uri.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                } else {
                    inst = ShellExecuteW(nullptr, L"open", best->exe.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                }

                if ((INT_PTR)inst > 32) {
                    result.success = true;
                    result.voiceFeedback = L"Opening " + intent.target;
                    result.visualFeedback = intent.target;
                } else {
                    result.success = false;
                    result.voiceFeedback = L"Sorry, I couldn't find " + intent.target;
                    result.visualFeedback = L"Not found";
                }
            } else {
                // Unknown app — try launching it by name directly
                std::wstring exeName = app + L".exe";
                HINSTANCE inst = ShellExecuteW(nullptr, L"open", exeName.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                if ((INT_PTR)inst > 32) {
                    result.success = true;
                    result.voiceFeedback = L"Opening " + intent.target;
                    result.visualFeedback = intent.target;
                } else {
                    result.success = false;
                    result.voiceFeedback = L"I don't know how to open " + intent.target + L". Try saying the exact app name.";
                    result.visualFeedback = L"App not found";
                }
            }
        }
        else if (intent.name == L"CLOSE_APP") {
            struct CloseWindowHelper {
                std::wstring target;
                bool closedAny = false;

                static BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam) {
                    CloseWindowHelper* self = reinterpret_cast<CloseWindowHelper*>(lParam);
                    if (!IsWindowVisible(hwnd)) return TRUE;

                    wchar_t title[512] = {};
                    if (GetWindowTextW(hwnd, title, 512) > 0) {
                        std::wstring wTitle(title);
                        std::transform(wTitle.begin(), wTitle.end(), wTitle.begin(), ::tolower);
                        if (wTitle.find(self->target) != std::wstring::npos) {
                            PostMessageW(hwnd, WM_CLOSE, 0, 0);
                            self->closedAny = true;
                        }
                    }
                    return TRUE;
                }
            };

            std::wstring app = ToLower(intent.target);
            CloseWindowHelper helper;
            helper.target = app;
            
            if (app == L"vs code" || app == L"vscode") helper.target = L"visual studio code";
            else if (app == L"chrome") helper.target = L"google chrome";
            else if (app == L"files" || app == L"explorer") helper.target = L"file explorer";

            EnumWindows(CloseWindowHelper::EnumProc, reinterpret_cast<LPARAM>(&helper));

            if (helper.closedAny) {
                result.success = true;
                result.voiceFeedback = L"Closed " + intent.target;
                result.visualFeedback = L"Closed";
            } else {
                std::wstring exeName;
                auto it = apps_.find(app);
                if (it != apps_.end() && !it->second.exe.empty()) {
                    exeName = it->second.exe;
                } else {
                    exeName = app + L".exe";
                }

                std::wstring cmd = L"taskkill /F /IM " + exeName;
                int ret = _wsystem(cmd.c_str());
                if (ret == 0) {
                    result.success = true;
                    result.voiceFeedback = L"Closed " + intent.target;
                    result.visualFeedback = L"Closed";
                } else {
                    result.success = false;
                    result.voiceFeedback = L"Couldn't find any running " + intent.target + L" to close.";
                    result.visualFeedback = L"Not running";
                }
            }
        }

        return result;
    }
};

// ============================================================================
// SystemSkill - Volume, brightness, power controls
// Supported commands:
//   "mute", "unmute", "volume up", "volume down", "increase volume", "decrease volume"
//   "sleep", "shutdown", "restart"
// ============================================================================

class SystemSkill : public ISkill {
public:
    bool Supports(const Intent& intent) override {
        return (intent.name == L"VOLUME_CONTROL" || intent.name == L"POWER_CONTROL");
    }

    SkillResult Execute(const Intent& intent) override {
        SkillResult result;
        result.success = true;

        if (intent.name == L"VOLUME_CONTROL") {
            auto it = intent.parameters.find(L"action");
            std::wstring action = (it != intent.parameters.end()) ? it->second : L"change";

            if (action == L"mute") {
                // Send mute key
                keybd_event(VK_VOLUME_MUTE, 0, 0, 0);
                keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_KEYUP, 0);
                result.voiceFeedback = L"Muted";
                result.visualFeedback = L"Muted";
            } else if (action == L"increase") {
                // Press volume up 5 times for a noticeable step
                for (int i = 0; i < 5; ++i) {
                    keybd_event(VK_VOLUME_UP, 0, 0, 0);
                    keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_KEYUP, 0);
                }
                result.voiceFeedback = L"Volume increased";
                result.visualFeedback = L"Volume Up";
            } else if (action == L"decrease") {
                for (int i = 0; i < 5; ++i) {
                    keybd_event(VK_VOLUME_DOWN, 0, 0, 0);
                    keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_KEYUP, 0);
                }
                result.voiceFeedback = L"Volume decreased";
                result.visualFeedback = L"Volume Down";
            }
        }
        else if (intent.name == L"POWER_CONTROL") {
            std::wstring state = intent.target;
            if (state == L"lock") {
                result.voiceFeedback = L"Locking PC";
                result.visualFeedback = L"Lock";
                LockWorkStation();
            } else if (state == L"sleep") {
                result.voiceFeedback = L"Going to sleep";
                result.visualFeedback = L"Sleep";
                system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");
            } else if (state == L"shutdown") {
                result.voiceFeedback = L"Shutting down";
                result.visualFeedback = L"Shutdown";
                system("shutdown /s /t 5");
            } else if (state == L"restart") {
                result.voiceFeedback = L"Restarting";
                result.visualFeedback = L"Restart";
                system("shutdown /r /t 5");
            }
        }

        return result;
    }
};

// ============================================================================
// CustomSpotifySkill - Spotify playback controls
// Supported commands:
//   "play", "pause", "next song", "previous song", "skip"
// ============================================================================

class CustomSpotifySkill : public ISkill {
public:
    bool Supports(const Intent& intent) override {
        return intent.name == L"MEDIA_CONTROL";
    }

    SkillResult Execute(const Intent& intent) override {
        SkillResult result;
        result.success = true;

        auto it = intent.parameters.find(L"action");
        std::wstring action = (it != intent.parameters.end()) ? it->second : L"";

        if (action == L"play" || action == L"pause") {
            keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0);
            keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0);
            result.voiceFeedback = (action == L"play") ? L"Playing" : L"Paused";
            result.visualFeedback = (action == L"play") ? L"Play" : L"Pause";
        } else if (action == L"next") {
            keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);
            keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0);
            result.voiceFeedback = L"Next track";
            result.visualFeedback = L"Next Track";
        } else if (action == L"previous") {
            keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0);
            keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0);
            result.voiceFeedback = L"Previous track";
            result.visualFeedback = L"Previous Track";
        } else {
            result.success = false;
        }

        return result;
    }
};

} // namespace ai
