#pragma once
#include <string>

namespace ai {

struct SessionContext {
    std::wstring lastOpenedApp;
    std::wstring lastExecutedSkill;
    std::wstring lastAIResponse;
};

class ContextManager {
private:
    SessionContext context_;

public:
    void SetLastOpenedApp(const std::wstring& appName) {
        context_.lastOpenedApp = appName;
        context_.lastExecutedSkill = L"LAUNCH_APP";
    }

    void SetLastExecutedSkill(const std::wstring& skillName) {
        context_.lastExecutedSkill = skillName;
    }

    void SetLastAIResponse(const std::wstring& response) {
        context_.lastAIResponse = response;
    }

    SessionContext GetContext() const {
        return context_;
    }

    void Clear() {
        context_ = SessionContext();
    }
};

} // namespace ai
