#pragma once

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
