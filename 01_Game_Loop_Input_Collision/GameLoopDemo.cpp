#include "01_Game_Loop_Input_Collision/GameLoopDemo.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        struct InputCommand
        {
            Vector2 moveDirection;
            bool sprint = false;
        };

        struct CircleCollider
        {
            Vector2 center;
            float radius = 0.5f;
        };

        struct Player
        {
            Vector2 position;
            float walkSpeed = 3.0f;
            float sprintSpeed = 5.5f;
            CircleCollider collider;
        };

        struct Hazard
        {
            std::string name;
            CircleCollider collider;
        };

        bool Overlaps(const CircleCollider& a, const CircleCollider& b)
        {
            const float combinedRadius = a.radius + b.radius;
            return Distance(a.center, b.center) <= combinedRadius;
        }

        void ApplyInput(Player& player, const InputCommand& command, float deltaSeconds)
        {
            // Input is converted into a normalized direction so diagonal movement is not faster.
            const Vector2 direction = command.moveDirection.Normalized();
            const float speed = command.sprint ? player.sprintSpeed : player.walkSpeed;
            player.position += direction * speed * deltaSeconds;

            // Keep the collision shape attached to the gameplay position.
            player.collider.center = player.position;
        }
    }

    void RunGameLoopDemo()
    {
        std::cout << "[01] Game Loop, Input, Collision\n";

        Player player;
        player.position = { 0.0f, 0.0f };
        player.collider = { player.position, 0.45f };

        const std::vector<Hazard> hazards = {
            { "Spike Trap", { { 2.4f, 0.0f }, 0.5f } },
            { "Fire Zone", { { 4.4f, 0.0f }, 0.7f } }
        };

        const std::vector<InputCommand> scriptedInput = {
            { { 1.0f, 0.0f }, false },
            { { 1.0f, 0.0f }, true },
            { { 1.0f, 0.0f }, true },
            { { 0.0f, 1.0f }, false },
            { { 1.0f, 0.0f }, false }
        };

        constexpr float fixedDeltaSeconds = 0.16f;
        for (std::size_t frame = 0; frame < scriptedInput.size(); ++frame)
        {
            ApplyInput(player, scriptedInput[frame], fixedDeltaSeconds);

            std::cout << "  Frame " << frame
                      << " position=(" << player.position.x << ", " << player.position.y << ")";

            for (const Hazard& hazard : hazards)
            {
                if (Overlaps(player.collider, hazard.collider))
                {
                    std::cout << " hit=" << hazard.name;
                }
            }

            std::cout << '\n';
        }

        std::cout << '\n';
    }
}
