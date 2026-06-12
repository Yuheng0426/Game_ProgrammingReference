#include "99_Project_Self_Checks_For_Game_Systems/SelfCheckDemo.h"

// Project self-check lesson.
//
// A learning repository benefits from fast checks that run with the normal demo.
// If someone changes Vector2, HealthComponent, CooldownTimer, or ResourcePool,
// these checks catch obvious broken behavior before the examples become confusing.

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Common_Game_Code/Gameplay/CooldownTimer.h"
#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Gameplay/ResourcePool.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        void Require(bool condition, const std::string& message, int& passedChecks)
        {
            // Throwing stops the demo immediately, which makes a failed assumption hard to miss.
            if (!condition)
            {
                throw std::runtime_error("Self check failed: " + message);
            }

            ++passedChecks;
        }

        bool NearlyEqual(float a, float b, float tolerance = 0.001f)
        {
            return std::fabs(a - b) <= tolerance;
        }
    }

    void RunSelfCheckDemo()
    {
        std::cout << "[99] Project Self Checks For Game Systems\n";

        int passedChecks = 0;

        const Vector2 normalized = Vector2(3.0f, 4.0f).Normalized();
        Require(NearlyEqual(normalized.Length(), 1.0f), "Vector normalization should produce unit length", passedChecks);

        HealthComponent health(100);
        health.ApplyDamage(35);
        health.Heal(20);
        Require(health.Current() == 85, "Health should apply damage and healing with clamping", passedChecks);

        CooldownTimer cooldown;
        cooldown.Start(0.5f);
        cooldown.Update(0.2f);
        Require(!cooldown.IsReady(), "Cooldown should remain locked before the duration ends", passedChecks);
        cooldown.Update(0.3f);
        Require(cooldown.IsReady(), "Cooldown should unlock when time reaches zero", passedChecks);

        ResourcePool stamina(100.0f, 10.0f);
        Require(stamina.TrySpend(40.0f), "Resource pool should spend available points", passedChecks);
        stamina.Update(1.0f);
        Require(NearlyEqual(stamina.Current(), 70.0f), "Resource pool should recover over time", passedChecks);
        Require(!stamina.TrySpend(100.0f), "Resource pool should reject costs above current amount", passedChecks);

        std::cout << "  Passed checks=" << passedChecks << '\n';
        std::cout << '\n';
    }
}
