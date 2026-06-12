#include "03_Soulslike_Stamina_Dodge_Parry_Combat/SoulslikeDemo.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Gameplay/ResourcePool.h"

namespace game_ref
{
    namespace
    {
        enum class CombatState
        {
            Idle,
            Attacking,
            Dodging,
            Parrying,
            Staggered
        };

        struct Fighter
        {
            std::string name;
            HealthComponent health{ 100 };
            ResourcePool stamina{ 100.0f, 18.0f };
            CombatState state = CombatState::Idle;
            float stateTimer = 0.0f;
            float invincibleTimer = 0.0f;
            float parryWindowTimer = 0.0f;
        };

        const char* StateName(CombatState state)
        {
            switch (state)
            {
            case CombatState::Idle: return "Idle";
            case CombatState::Attacking: return "Attacking";
            case CombatState::Dodging: return "Dodging";
            case CombatState::Parrying: return "Parrying";
            case CombatState::Staggered: return "Staggered";
            }

            return "Unknown";
        }

        void EnterState(Fighter& fighter, CombatState state, float durationSeconds)
        {
            fighter.state = state;
            fighter.stateTimer = durationSeconds;
        }

        void UpdateFighter(Fighter& fighter, float deltaSeconds)
        {
            fighter.stamina.Update(deltaSeconds);
            fighter.stateTimer = std::max(0.0f, fighter.stateTimer - deltaSeconds);
            fighter.invincibleTimer = std::max(0.0f, fighter.invincibleTimer - deltaSeconds);
            fighter.parryWindowTimer = std::max(0.0f, fighter.parryWindowTimer - deltaSeconds);

            if (fighter.state != CombatState::Idle && fighter.stateTimer <= 0.0f)
            {
                fighter.state = CombatState::Idle;
            }
        }

        bool TryLightAttack(Fighter& attacker)
        {
            if (attacker.state != CombatState::Idle || !attacker.stamina.TrySpend(22.0f))
            {
                return false;
            }

            EnterState(attacker, CombatState::Attacking, 0.45f);
            return true;
        }

        bool TryDodge(Fighter& fighter)
        {
            if (fighter.state != CombatState::Idle || !fighter.stamina.TrySpend(30.0f))
            {
                return false;
            }

            // Invincibility frames should be short and intentional, not the entire roll animation.
            fighter.invincibleTimer = 0.22f;
            EnterState(fighter, CombatState::Dodging, 0.55f);
            return true;
        }

        bool TryParry(Fighter& fighter)
        {
            if (fighter.state != CombatState::Idle || !fighter.stamina.TrySpend(18.0f))
            {
                return false;
            }

            // The parry window is shorter than the full parry animation to reward timing.
            fighter.parryWindowTimer = 0.18f;
            EnterState(fighter, CombatState::Parrying, 0.5f);
            return true;
        }

        void ResolveIncomingHit(Fighter& defender, Fighter& attacker, int damage)
        {
            if (defender.invincibleTimer > 0.0f)
            {
                std::cout << "    " << defender.name << " dodged through the attack\n";
                return;
            }

            if (defender.parryWindowTimer > 0.0f)
            {
                EnterState(attacker, CombatState::Staggered, 0.9f);
                std::cout << "    " << defender.name << " parried " << attacker.name << '\n';
                return;
            }

            defender.health.ApplyDamage(damage);
            std::cout << "    " << defender.name << " took " << damage
                      << " damage. HP=" << defender.health.Current() << '/' << defender.health.Max() << '\n';
        }
    }

    void RunSoulslikeDemo()
    {
        std::cout << "[03] Soulslike Stamina, Dodge, Parry Combat\n";

        Fighter player{ "Knight", HealthComponent(120), ResourcePool(100.0f, 20.0f) };
        Fighter boss{ "Ash Warden", HealthComponent(220), ResourcePool(140.0f, 10.0f) };

        constexpr float deltaSeconds = 0.1f;
        for (int frame = 0; frame < 16; ++frame)
        {
            UpdateFighter(player, deltaSeconds);
            UpdateFighter(boss, deltaSeconds);

            if (frame == 1)
            {
                std::cout << "  Player dodge: " << (TryDodge(player) ? "success" : "failed") << '\n';
            }

            if (frame == 2)
            {
                std::cout << "  Boss attack connects\n";
                ResolveIncomingHit(player, boss, 38);
            }

            if (frame == 7)
            {
                std::cout << "  Player parry: " << (TryParry(player) ? "success" : "failed") << '\n';
            }

            if (frame == 8)
            {
                std::cout << "  Boss attack connects\n";
                ResolveIncomingHit(player, boss, 42);
            }

            if (frame == 13)
            {
                std::cout << "  Player attack: " << (TryLightAttack(player) ? "success" : "failed") << '\n';
            }

            std::cout << "    State player=" << StateName(player.state)
                      << " stamina=" << player.stamina.Current()
                      << " boss=" << StateName(boss.state) << '\n';
        }

        std::cout << '\n';
    }
}
