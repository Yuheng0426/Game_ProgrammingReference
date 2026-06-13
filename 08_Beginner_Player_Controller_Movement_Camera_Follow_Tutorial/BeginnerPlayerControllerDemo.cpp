#include "08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial/BeginnerPlayerControllerDemo.h"

// Beginner controller lesson.
//
// This file is heavily commented for new C++ and game programming learners.
// The goal is to explain the tiny pieces that form a smooth player controller:
// input, desired speed, acceleration, position update, world bounds, and camera follow.

#include <algorithm> // Provides std::max and std::min, used by Clamp.
#include <iostream>  // Provides std::cout for printing tutorial output.
#include <vector>    // Provides std::vector for storing a list of scripted inputs.

#include "Common_Game_Code/Math/Vector2.h" // Provides Vector2 for 2D positions, directions, and velocities.

namespace game_ref // Project namespace.
{
    namespace // Private helper area for this lesson file.
    {
        struct PlayerInput // Stores what the player wants to do this frame.
        {
            // moveAxis is usually created from keyboard, controller stick, or touch input.
            // In this demo we script the values so every run is predictable.
            Vector2 moveAxis;

            // sprintHeld means the sprint button is being held.
            // false uses walk speed, true uses sprint speed.
            bool sprintHeld = false;
        };

        struct PlayerController // Stores movement-related player data.
        {
            // position is the player's current location in the world.
            Vector2 position;

            // velocity is the player's current movement speed and direction.
            // Example: velocity (3, 0) means moving right at speed 3.
            Vector2 velocity;

            // walkSpeed is the target speed when sprint is not held.
            float walkSpeed = 3.0f;

            // sprintSpeed is the target speed when sprint is held.
            float sprintSpeed = 5.5f;

            // acceleration controls how quickly velocity changes.
            // Higher values feel more responsive; lower values feel smoother/heavier.
            float acceleration = 16.0f;
        };

        struct FollowCamera // Stores only the camera data needed by this lesson.
        {
            // position is where the camera is currently looking.
            Vector2 position;

            // followSharpness controls how quickly the camera catches up to the player.
            float followSharpness = 5.0f;
        };

        float Clamp(float value, float minValue, float maxValue)
        {
            // std::min(value, maxValue) prevents value from going above maxValue.
            // std::max(minValue, ...) prevents the result from going below minValue.
            return std::max(minValue, std::min(value, maxValue));
        }

        Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistance)
        {
            // offset is the vector from current to target.
            const Vector2 offset = target - current;

            // distance is how far current is from target.
            const float distance = offset.Length();

            // If the target is already close enough, return the target directly.
            // The tiny 0.001 check prevents division by almost-zero when normalizing.
            if (distance <= maxDistance || distance <= 0.001f)
            {
                return target;
            }

            // Move from current toward target by maxDistance.
            // offset.Normalized() gives the direction only.
            return current + offset.Normalized() * maxDistance;
        }

        void UpdatePlayer(PlayerController& player, const PlayerInput& input, float deltaSeconds)
        {
            // Choose target speed based on whether sprint is held.
            const float targetSpeed = input.sprintHeld ? player.sprintSpeed : player.walkSpeed;

            // Convert input into desired velocity.
            // Normalized() prevents diagonal input from being faster.
            // Multiplying by targetSpeed turns direction into movement speed.
            const Vector2 desiredVelocity = input.moveAxis.Normalized() * targetSpeed;

            // Acceleration changes velocity gradually instead of instantly.
            // player.acceleration * deltaSeconds is the maximum velocity change this frame.
            player.velocity = MoveTowards(player.velocity, desiredVelocity, player.acceleration * deltaSeconds);

            // Position changes based on velocity and time.
            // This is the same basic movement formula used in many real games.
            player.position += player.velocity * deltaSeconds;

            // Clamp x so the player cannot leave the tutorial area's left/right bounds.
            player.position.x = Clamp(player.position.x, -5.0f, 5.0f);

            // Clamp y so the player cannot leave the tutorial area's bottom/top bounds.
            player.position.y = Clamp(player.position.y, -3.0f, 3.0f);
        }

        void UpdateCamera(FollowCamera& camera, const PlayerController& player, float deltaSeconds)
        {
            // blend is a value from 0 to 1.
            // 0 means camera does not move, 1 means camera snaps directly to player.
            const float blend = Clamp(camera.followSharpness * deltaSeconds, 0.0f, 1.0f);

            // player.position - camera.position is the direction from camera to player.
            // Multiplying by blend moves only part of that distance, creating smooth follow.
            camera.position += (player.position - camera.position) * blend;
        }
    }

    void RunBeginnerPlayerControllerDemo()
    {
        // Print the lesson title.
        std::cout << "[08] Beginner Tutorial: Player Controller, Movement, Camera Follow\n";

        // Create the player controller with default position, velocity, speeds, and acceleration.
        PlayerController player;

        // Create the camera with default position and follow sharpness.
        FollowCamera camera;

        // Scripted inputs make this lesson deterministic and easy to compare.
        const std::vector<PlayerInput> inputs = {
            // Frame 0: move right at walk speed.
            { { 1.0f, 0.0f }, false },

            // Frame 1: keep moving right at walk speed.
            { { 1.0f, 0.0f }, false },

            // Frame 2: move diagonally up-right while sprinting.
            { { 1.0f, 1.0f }, true },

            // Frame 3: move up while sprinting.
            { { 0.0f, 1.0f }, true },

            // Frame 4: move left at walk speed.
            { { -1.0f, 0.0f }, false },

            // Frame 5: no input, so velocity moves toward zero.
            { { 0.0f, 0.0f }, false }
        };

        // Each frame uses the same time step so the output is stable.
        constexpr float deltaSeconds = 0.16f;

        // Loop through every scripted input frame.
        for (std::size_t frame = 0; frame < inputs.size(); ++frame)
        {
            // Update player movement using this frame's input.
            UpdatePlayer(player, inputs[frame], deltaSeconds);

            // Update camera after player movement so the camera follows the newest position.
            UpdateCamera(camera, player, deltaSeconds);

            // Print player and camera positions so learners can see the result of the math.
            std::cout << "  Frame " << frame
                      << " player=(" << player.position.x << ", " << player.position.y << ")"
                      << " camera=(" << camera.position.x << ", " << camera.position.y << ")\n";
        }

        // Print an empty line before the next lesson.
        std::cout << '\n';
    }
}
