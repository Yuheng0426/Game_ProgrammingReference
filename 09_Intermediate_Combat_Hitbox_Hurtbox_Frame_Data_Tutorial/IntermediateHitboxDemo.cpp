#include "09_Intermediate_Combat_Hitbox_Hurtbox_Frame_Data_Tutorial/IntermediateHitboxDemo.h"

#include <iostream>
#include <string>
#include <unordered_set>

#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        struct Box
        {
            Vector2 center;
            Vector2 halfSize;
        };

        struct FrameData
        {
            int startupFrames = 0;
            int activeFrames = 0;
            int recoveryFrames = 0;
        };

        struct AttackInstance
        {
            std::string name;
            FrameData frameData;
            Box localHitbox;
            int damage = 0;
            int currentFrame = 0;
            std::unordered_set<std::string> alreadyHitTargets;
        };

        struct CombatBody
        {
            std::string id;
            Vector2 position;
            Box hurtbox;
            HealthComponent health{ 100 };
        };

        bool Overlaps(const Box& a, const Box& b)
        {
            const bool separatedX = a.center.x + a.halfSize.x < b.center.x - b.halfSize.x ||
                                    b.center.x + b.halfSize.x < a.center.x - a.halfSize.x;
            const bool separatedY = a.center.y + a.halfSize.y < b.center.y - b.halfSize.y ||
                                    b.center.y + b.halfSize.y < a.center.y - a.halfSize.y;
            return !separatedX && !separatedY;
        }

        bool IsActiveFrame(const AttackInstance& attack)
        {
            const int activeStart = attack.frameData.startupFrames;
            const int activeEnd = activeStart + attack.frameData.activeFrames;
            return attack.currentFrame >= activeStart && attack.currentFrame < activeEnd;
        }

        bool IsFinished(const AttackInstance& attack)
        {
            const int totalFrames = attack.frameData.startupFrames +
                                    attack.frameData.activeFrames +
                                    attack.frameData.recoveryFrames;
            return attack.currentFrame >= totalFrames;
        }

        Box ToWorldBox(const Box& localBox, const Vector2& ownerPosition)
        {
            return { ownerPosition + localBox.center, localBox.halfSize };
        }

        void UpdateAttack(AttackInstance& attack, const Vector2& attackerPosition, CombatBody& target)
        {
            if (IsActiveFrame(attack) && attack.alreadyHitTargets.count(target.id) == 0)
            {
                const Box worldHitbox = ToWorldBox(attack.localHitbox, attackerPosition);
                const Box worldHurtbox = ToWorldBox(target.hurtbox, target.position);

                if (Overlaps(worldHitbox, worldHurtbox))
                {
                    target.health.ApplyDamage(attack.damage);
                    attack.alreadyHitTargets.insert(target.id);
                    std::cout << "    " << attack.name << " hit " << target.id
                              << " for " << attack.damage
                              << " damage. HP=" << target.health.Current() << '/' << target.health.Max() << '\n';
                }
            }

            ++attack.currentFrame;
        }
    }

    void RunIntermediateHitboxDemo()
    {
        std::cout << "[09] Intermediate Tutorial: Combat Hitbox, Hurtbox, Frame Data\n";

        const Vector2 attackerPosition{ 0.0f, 0.0f };
        CombatBody target{ "TrainingDummy", { 1.1f, 0.0f }, { { 0.0f, 0.0f }, { 0.35f, 0.8f } }, HealthComponent(100) };
        AttackInstance attack{ "SwordSlash", { 2, 3, 2 }, { { 0.9f, 0.0f }, { 0.55f, 0.25f } }, 28 };

        while (!IsFinished(attack))
        {
            std::cout << "  Frame " << attack.currentFrame
                      << " active=" << (IsActiveFrame(attack) ? "true" : "false") << '\n';
            UpdateAttack(attack, attackerPosition, target);
        }

        std::cout << '\n';
    }
}
