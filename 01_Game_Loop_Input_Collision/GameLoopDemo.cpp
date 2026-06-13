#include "01_Game_Loop_Input_Collision/GameLoopDemo.h"

// Beginner lesson: game loop, input, movement, and collision.
//
// This file is intentionally over-commented for new learners.
// The comments explain not only what the code does, but also why each idea exists.
//
// If you are new:
// - "Frame" means one update step of the game.
// - "Input" means what the player wants to do this frame.
// - "Delta seconds" means how much time passed during this frame.
// - "Collider" means a simple shape used to test whether objects touch.

#include <algorithm> // Provides small utility functions used by many C++ programs.
#include <iostream>  // Provides std::cout, which prints text to the console.
#include <string>    // Provides std::string, the standard C++ text type.
#include <vector>    // Provides std::vector, a dynamic array that can store many items.

#include "Common_Game_Code/Math/Vector2.h" // Provides Vector2, a small 2D math type used for positions and directions.

namespace game_ref // Keeps all project symbols inside one namespace so names do not conflict with other libraries.
{
    namespace // Anonymous namespace means these helper types/functions are private to this .cpp file.
    {
        struct InputCommand // A struct groups related input data into one readable object.
        {
            // moveDirection stores where the player wants to move.
            // x = 1 means right, x = -1 means left.
            // y = 1 means up, y = -1 means down.
            Vector2 moveDirection;

            // sprint is false by default.
            // false means walk speed, true means sprint speed.
            bool sprint = false;
        };

        struct CircleCollider // A collider is a simple shape used for collision checks.
        {
            // center is the world position of the circle.
            Vector2 center;

            // radius is the size of the circle.
            // A bigger radius makes the object easier to hit.
            float radius = 0.5f;
        };

        struct Player // Player stores only the data needed for this lesson.
        {
            // position is where the player is in the 2D world.
            Vector2 position;

            // walkSpeed is used when sprint is false.
            float walkSpeed = 3.0f;

            // sprintSpeed is used when sprint is true.
            float sprintSpeed = 5.5f;

            // collider is attached to the player for collision checks.
            CircleCollider collider;
        };

        struct Hazard // Hazard means something in the world the player can collide with.
        {
            // name is printed when the player hits this hazard.
            std::string name;

            // collider defines the hazard's collision area.
            CircleCollider collider;
        };

        bool Overlaps(const CircleCollider& a, const CircleCollider& b)
        {
            // combinedRadius is the distance at which two circles just touch.
            const float combinedRadius = a.radius + b.radius;

            // Distance(a.center, b.center) measures how far apart the circle centers are.
            // If center distance is smaller than or equal to the combined radius, the circles overlap.
            return Distance(a.center, b.center) <= combinedRadius;
        }

        void ApplyInput(Player& player, const InputCommand& command, float deltaSeconds)
        {
            // Normalized() turns the input direction into length 1.
            // This prevents diagonal movement from being faster than straight movement.
            const Vector2 direction = command.moveDirection.Normalized();

            // The ternary operator reads like:
            // if command.sprint is true, use player.sprintSpeed; otherwise use player.walkSpeed.
            const float speed = command.sprint ? player.sprintSpeed : player.walkSpeed;

            // Movement formula:
            // new position = old position + direction * speed * time
            // direction says where to go, speed says how fast, deltaSeconds says how long.
            player.position += direction * speed * deltaSeconds;

            // The collider must follow the player.
            // If we forget this line, the visible/gameplay position and collision position become different.
            player.collider.center = player.position;
        }
    }

    void RunGameLoopDemo()
    {
        // Print the lesson title so the console output is easy to scan.
        std::cout << "[01] Game Loop, Input, Collision\n";

        // Create a player object with default speeds and default collider data.
        Player player;

        // Start the player at world position x=0, y=0.
        player.position = { 0.0f, 0.0f };

        // Attach a circle collider to the player.
        // The first value is the center position, the second value is the radius.
        player.collider = { player.position, 0.45f };

        // Create a list of hazards.
        // std::vector is used because a real game can have many hazards.
        const std::vector<Hazard> hazards = {
            // Hazard 1: name is "Spike Trap", position is (2.4, 0.0), radius is 0.5.
            { "Spike Trap", { { 2.4f, 0.0f }, 0.5f } },

            // Hazard 2: name is "Fire Zone", position is (4.4, 0.0), radius is 0.7.
            { "Fire Zone", { { 4.4f, 0.0f }, 0.7f } }
        };

        // This is scripted input, not keyboard input.
        // Scripted input makes the lesson repeatable: every run produces the same output.
        const std::vector<InputCommand> scriptedInput = {
            // Frame 0: move right, do not sprint.
            { { 1.0f, 0.0f }, false },

            // Frame 1: move right, sprint.
            { { 1.0f, 0.0f }, true },

            // Frame 2: move right, sprint again.
            { { 1.0f, 0.0f }, true },

            // Frame 3: move up, walk speed.
            { { 0.0f, 1.0f }, false },

            // Frame 4: move right, walk speed.
            { { 1.0f, 0.0f }, false }
        };

        // fixedDeltaSeconds means each frame pretends 0.16 seconds passed.
        // A fixed value is easier for beginners because the math is stable.
        constexpr float fixedDeltaSeconds = 0.16f;

        // This for loop is the "game loop" of the lesson.
        // frame starts at 0, runs while frame is smaller than the number of inputs, and increases by 1.
        for (std::size_t frame = 0; frame < scriptedInput.size(); ++frame)
        {
            // Apply the input for this frame to update player position and collider.
            ApplyInput(player, scriptedInput[frame], fixedDeltaSeconds);

            // Print the current frame number and player position.
            std::cout << "  Frame " << frame
                      << " position=(" << player.position.x << ", " << player.position.y << ")";

            // Check every hazard to see whether the player is touching it.
            for (const Hazard& hazard : hazards)
            {
                // If the player's circle overlaps the hazard's circle, print the hazard name.
                if (Overlaps(player.collider, hazard.collider))
                {
                    std::cout << " hit=" << hazard.name;
                }
            }

            // Print a newline after this frame's output.
            std::cout << '\n';
        }

        // Print one empty line so the next lesson starts with visual spacing.
        std::cout << '\n';
    }
}
