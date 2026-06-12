#include "06_Action_Game_Combo_LockOn_Targeting_System/ComboLockOnDemo.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        struct CombatTarget
        {
            std::string name;
            Vector2 position;
            HealthComponent health{ 100 };
        };

        struct ComboStep
        {
            std::string animationName;
            int damage = 0;
            float inputWindowSeconds = 0.0f;
        };

        class ComboController
        {
        public:
            explicit ComboController(std::vector<ComboStep> steps) : comboSteps(std::move(steps)) {}

            void Update(float deltaSeconds)
            {
                inputWindowRemaining = std::max(0.0f, inputWindowRemaining - deltaSeconds);
            }

            [[nodiscard]] const ComboStep* TryAttack()
            {
                if (comboSteps.empty())
                {
                    return nullptr;
                }

                if (currentStep != 0 && inputWindowRemaining <= 0.0f)
                {
                    currentStep = 0;
                }

                const ComboStep& step = comboSteps[currentStep];
                currentStep = (currentStep + 1) % comboSteps.size();
                inputWindowRemaining = step.inputWindowSeconds;
                return &step;
            }

        private:
            std::vector<ComboStep> comboSteps;
            std::size_t currentStep = 0;
            float inputWindowRemaining = 0.0f;
        };

        float Dot(const Vector2& a, const Vector2& b)
        {
            return a.x * b.x + a.y * b.y;
        }

        CombatTarget* SelectLockOnTarget(
            const Vector2& playerPosition,
            const Vector2& facingDirection,
            std::vector<CombatTarget>& targets)
        {
            CombatTarget* bestTarget = nullptr;
            float bestScore = -1.0f;
            const Vector2 facing = facingDirection.Normalized();

            for (CombatTarget& target : targets)
            {
                if (!target.health.IsAlive())
                {
                    continue;
                }

                const Vector2 toTarget = target.position - playerPosition;
                const float distance = toTarget.Length();
                if (distance > 6.0f)
                {
                    continue;
                }

                // Higher dot means the target is closer to the player's forward direction.
                const float directionScore = Dot(facing, toTarget.Normalized());
                const float distanceScore = 1.0f - distance / 6.0f;
                const float totalScore = directionScore * 0.75f + distanceScore * 0.25f;

                if (totalScore > bestScore)
                {
                    bestScore = totalScore;
                    bestTarget = &target;
                }
            }

            return bestTarget;
        }
    }

    void RunComboLockOnDemo()
    {
        std::cout << "[06] Action Game Combo and Lock-On Targeting System\n";

        Vector2 playerPosition{ 0.0f, 0.0f };
        Vector2 playerFacing{ 1.0f, 0.0f };
        std::vector<CombatTarget> targets = {
            { "Raider", { 2.2f, 0.4f }, HealthComponent(90) },
            { "Archer", { 3.5f, -1.3f }, HealthComponent(70) },
            { "Far Mage", { 8.0f, 0.0f }, HealthComponent(60) }
        };

        ComboController combo({
            { "LightSlash_01", 18, 0.45f },
            { "LightSlash_02", 22, 0.40f },
            { "HeavyFinisher", 38, 0.0f }
        });

        constexpr float deltaSeconds = 0.25f;
        for (int frame = 0; frame < 6; ++frame)
        {
            combo.Update(deltaSeconds);
            CombatTarget* target = SelectLockOnTarget(playerPosition, playerFacing, targets);

            if ((frame == 0 || frame == 1 || frame == 2 || frame == 5) && target != nullptr)
            {
                const ComboStep* step = combo.TryAttack();
                if (step != nullptr)
                {
                    target->health.ApplyDamage(step->damage);
                    std::cout << "  Frame " << frame << " " << step->animationName
                              << " hit " << target->name
                              << " HP=" << target->health.Current() << '/' << target->health.Max() << '\n';
                }
            }
            else if (target != nullptr)
            {
                std::cout << "  Frame " << frame << " locked on " << target->name << '\n';
            }
        }

        std::cout << '\n';
    }
}
