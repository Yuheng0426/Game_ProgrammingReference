#pragma once

// Reusable cooldown timer for weapons, abilities, AI attacks, and boss logic.
//
// The timer stores "remaining time" instead of "elapsed time" because gameplay
// code usually asks one simple question: "Is this action ready now?"
// Start() locks the action for a duration, Update() counts down, and IsReady()
// becomes true when the duration reaches zero.

#include <algorithm>

namespace game_ref
{
    class CooldownTimer
    {
    public:
        void Start(float durationSeconds)
        {
            remainingSeconds = std::max(0.0f, durationSeconds);
        }

        void Update(float deltaSeconds)
        {
            remainingSeconds = std::max(0.0f, remainingSeconds - deltaSeconds);
        }

        [[nodiscard]] bool IsReady() const
        {
            return remainingSeconds <= 0.0f;
        }

        [[nodiscard]] float RemainingSeconds() const
        {
            return remainingSeconds;
        }

    private:
        float remainingSeconds = 0.0f;
    };
}
