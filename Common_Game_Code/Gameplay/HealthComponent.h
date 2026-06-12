#pragma once

#include <algorithm>

namespace game_ref
{
    class HealthComponent
    {
    public:
        explicit HealthComponent(int maxValue) : maxHealth(std::max(1, maxValue)), currentHealth(maxHealth) {}

        void ApplyDamage(int amount)
        {
            currentHealth = std::max(0, currentHealth - std::max(0, amount));
        }

        void Heal(int amount)
        {
            currentHealth = std::min(maxHealth, currentHealth + std::max(0, amount));
        }

        [[nodiscard]] bool IsAlive() const
        {
            return currentHealth > 0;
        }

        [[nodiscard]] int Current() const
        {
            return currentHealth;
        }

        [[nodiscard]] int Max() const
        {
            return maxHealth;
        }

    private:
        int maxHealth = 1;
        int currentHealth = 1;
    };
}
