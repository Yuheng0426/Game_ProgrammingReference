#pragma once

// Generic recoverable resource pool.
//
// Stamina, mana, energy, posture, and heat meters often share the same behavior:
// they have a maximum value, can be spent, and recover over time. This helper
// lets lessons reuse that idea without duplicating the same code in every folder.

#include <algorithm>

namespace game_ref
{
    class ResourcePool
    {
    public:
        ResourcePool(float maxValue, float recoveryPerSecond)
            : maxAmount(std::max(1.0f, maxValue)),
              currentAmount(maxAmount),
              recoveryRate(std::max(0.0f, recoveryPerSecond))
        {
        }

        void Update(float deltaSeconds)
        {
            currentAmount = std::min(maxAmount, currentAmount + recoveryRate * deltaSeconds);
        }

        [[nodiscard]] bool TrySpend(float amount)
        {
            const float safeAmount = std::max(0.0f, amount);
            if (currentAmount < safeAmount)
            {
                return false;
            }

            currentAmount -= safeAmount;
            return true;
        }

        [[nodiscard]] float Current() const
        {
            return currentAmount;
        }

        [[nodiscard]] float Max() const
        {
            return maxAmount;
        }

    private:
        float maxAmount = 1.0f;
        float currentAmount = 1.0f;
        float recoveryRate = 0.0f;
    };
}
