#include "10_Advanced_Skill_Tree_Progression_Unlock_System/AdvancedSkillTreeDemo.h"

// Advanced progression lesson: skill definitions, prerequisites, and unlock rules.
//
// The central teaching point is validation. A skill should unlock only through
// one rule path, so the UI, save loader, debug tools, and gameplay rewards all
// agree on what "can unlock" means.

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace game_ref
{
    namespace
    {
        struct SkillDefinition
        {
            // SkillDefinition is static data: what the skill is and what it requires.
            std::string id;
            std::string displayName;
            int requiredLevel = 1;
            int pointCost = 1;
            std::vector<std::string> prerequisites;
        };

        struct PlayerProgression
        {
            // PlayerProgression is runtime data: what this player currently owns.
            int level = 1;
            int availableSkillPoints = 0;
            std::unordered_map<std::string, bool> unlockedSkills;
        };

        class SkillTree
        {
        public:
            explicit SkillTree(std::vector<SkillDefinition> definitions)
            {
                for (SkillDefinition& definition : definitions)
                {
                    const std::string skillId = definition.id;
                    skills.emplace(skillId, std::move(definition));
                }
            }

            [[nodiscard]] bool CanUnlock(const PlayerProgression& player, const std::string& skillId, std::string& reason) const
            {
                const auto skillIt = skills.find(skillId);
                if (skillIt == skills.end())
                {
                    reason = "skill does not exist";
                    return false;
                }

                if (player.unlockedSkills.count(skillId) != 0)
                {
                    reason = "already unlocked";
                    return false;
                }

                const SkillDefinition& skill = skillIt->second;
                if (player.level < skill.requiredLevel)
                {
                    reason = "level is too low";
                    return false;
                }

                if (player.availableSkillPoints < skill.pointCost)
                {
                    reason = "not enough skill points";
                    return false;
                }

                for (const std::string& prerequisite : skill.prerequisites)
                {
                    if (player.unlockedSkills.count(prerequisite) == 0)
                    {
                        reason = "missing prerequisite " + prerequisite;
                        return false;
                    }
                }

                reason = "ready";
                return true;
            }

            bool TryUnlock(PlayerProgression& player, const std::string& skillId)
            {
                std::string reason;
                if (!CanUnlock(player, skillId, reason))
                {
                    std::cout << "    Cannot unlock " << skillId << ": " << reason << '\n';
                    return false;
                }

                const SkillDefinition& skill = skills.at(skillId);
                player.availableSkillPoints -= skill.pointCost;
                player.unlockedSkills.emplace(skillId, true);
                std::cout << "    Unlocked " << skill.displayName
                          << " pointsLeft=" << player.availableSkillPoints << '\n';
                return true;
            }

        private:
            std::unordered_map<std::string, SkillDefinition> skills;
        };
    }

    void RunAdvancedSkillTreeDemo()
    {
        std::cout << "[10] Advanced Tutorial: Skill Tree, Progression, Unlock System\n";

        SkillTree skillTree({
            { "dash_attack", "Dash Attack", 2, 1, {} },
            { "perfect_dodge", "Perfect Dodge", 3, 1, { "dash_attack" } },
            { "storm_counter", "Storm Counter", 5, 2, { "perfect_dodge" } }
        });

        PlayerProgression player{ 3, 2, {} };
        skillTree.TryUnlock(player, "storm_counter");
        skillTree.TryUnlock(player, "dash_attack");
        skillTree.TryUnlock(player, "perfect_dodge");
        skillTree.TryUnlock(player, "storm_counter");

        player.level = 5;
        player.availableSkillPoints += 2;
        skillTree.TryUnlock(player, "storm_counter");

        std::cout << '\n';
    }
}
