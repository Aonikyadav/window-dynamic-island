#pragma once
#include "contracts.h"
#include "skills.h"
#include <vector>
#include <memory>
#include <string>

namespace ai {

// ============================================================================
// Plugin Manager — dispatches intents to skills
// ============================================================================

class PluginManager {
private:
    std::vector<std::shared_ptr<ISkill>> skills_;

public:
    void RegisterSkill(std::shared_ptr<ISkill> skill) {
        skills_.push_back(skill);
    }

    const std::vector<std::shared_ptr<ISkill>>& GetSkills() const {
        return skills_;
    }

    // Resolves and dispatches an intent to the first skill that supports it
    SkillResult Dispatch(const Intent& intent) {
        for (const auto& skill : skills_) {
            if (skill->Supports(intent)) {
                return skill->Execute(intent);
            }
        }

        SkillResult failure;
        failure.success = false;
        failure.visualFeedback = L"Not understood";
        failure.voiceFeedback = L"Sorry, I don't understand that command.";
        return failure;
    }
};

} // namespace ai
