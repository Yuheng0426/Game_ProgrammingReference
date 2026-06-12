#include "08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial/BeginnerPlayerControllerDemo.h"

// Beginner controller lesson.
//
// This file teaches the difference between desired velocity and current velocity.
// Instead of instantly snapping to full speed, the player accelerates toward the
// target velocity. That small detail makes movement feel smoother in many games.

#include <algorithm>
#include <iostream>
#include <vector>

#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        struct PlayerInput
        {
            Vector2 moveAxis;
            bool sprintHeld = false;
        };

        struct PlayerController
        {
            // Position is where the player is. Velocity is how fast the player is moving now.
            // Keeping both values makes acceleration and deceleration possible.
            Vector2 position;
            Vector2 velocity;
            float walkSpeed = 3.0f;
            float sprintSpeed = 5.5f;
            float acceleration = 16.0f;
        };

        struct FollowCamera
        {
            Vector2 position;
            float followSharpness = 5.0f;
        };

        float Clamp(float value, float minValue, float maxValue)
        {
            return std::max(minValue, std::min(value, maxValue));
        }

        Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistance)
        {
            const Vector2 offset = target - current;
            const float distance = offset.Length();
            if (distance <= maxDistance || distance <= 0.001f)
            {
                return target;
            }

            return current + offset.Normalized() * maxDistance;
        }

        void UpdatePlayer(PlayerController& player, const PlayerInput& input, float deltaSeconds)
        {
            const float targetSpeed = input.sprintHeld ? player.sprintSpeed : player.walkSpeed;
            const Vector2 desiredVelocity = input.moveAxis.Normalized() * targetSpeed;

            // Acceleration makes movement feel less robotic than instantly setting velocity.
            player.velocity = MoveTowards(player.velocity, desiredVelocity, player.acceleration * deltaSeconds);
            player.position += player.velocity * deltaSeconds;

            // World bounds are useful in tutorials because they make mistakes visible quickly.
            player.position.x = Clamp(player.position.x, -5.0f, 5.0f);
            player.position.y = Clamp(player.position.y, -3.0f, 3.0f);
        }

        void UpdateCamera(FollowCamera& camera, const PlayerController& player, float deltaSeconds)
        {
            const float blend = Clamp(camera.followSharpness * deltaSeconds, 0.0f, 1.0f);
            camera.position += (player.position - camera.position) * blend;
        }
    }

    void RunBeginnerPlayerControllerDemo()
    {
        std::cout << "[08] Beginner Tutorial: Player Controller, Movement, Camera Follow\n";

        PlayerController player;
        FollowCamera camera;

        const std::vector<PlayerInput> inputs = {
            { { 1.0f, 0.0f }, false },
            { { 1.0f, 0.0f }, false },
            { { 1.0f, 1.0f }, true },
            { { 0.0f, 1.0f }, true },
            { { -1.0f, 0.0f }, false },
            { { 0.0f, 0.0f }, false }
        };

        constexpr float deltaSeconds = 0.16f;
        for (std::size_t frame = 0; frame < inputs.size(); ++frame)
        {
            UpdatePlayer(player, inputs[frame], deltaSeconds);
            UpdateCamera(camera, player, deltaSeconds);

            std::cout << "  Frame " << frame
                      << " player=(" << player.position.x << ", " << player.position.y << ")"
                      << " camera=(" << camera.position.x << ", " << camera.position.y << ")\n";
        }

        std::cout << '\n';
    }
}
