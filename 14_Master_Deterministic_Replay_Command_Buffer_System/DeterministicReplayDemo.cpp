#include "14_Master_Deterministic_Replay_Command_Buffer_System/DeterministicReplayDemo.h"

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        enum class CommandType
        {
            MoveRight,
            MoveUp,
            Dash,
            Attack
        };

        struct GameplayCommand
        {
            int frame = 0;
            CommandType type = CommandType::MoveRight;
        };

        struct ReplayWorld
        {
            Vector2 playerPosition;
            int stamina = 100;
            int comboCounter = 0;
        };

        const char* CommandName(CommandType type)
        {
            switch (type)
            {
            case CommandType::MoveRight: return "MoveRight";
            case CommandType::MoveUp: return "MoveUp";
            case CommandType::Dash: return "Dash";
            case CommandType::Attack: return "Attack";
            }

            return "Unknown";
        }

        void ApplyCommand(ReplayWorld& world, CommandType command)
        {
            switch (command)
            {
            case CommandType::MoveRight:
                world.playerPosition.x += 1.0f;
                break;

            case CommandType::MoveUp:
                world.playerPosition.y += 1.0f;
                break;

            case CommandType::Dash:
                if (world.stamina >= 30)
                {
                    world.playerPosition.x += 2.0f;
                    world.stamina -= 30;
                }
                break;

            case CommandType::Attack:
                ++world.comboCounter;
                world.stamina = std::max(0, world.stamina - 10);
                break;
            }
        }

        int Quantize(float value)
        {
            return static_cast<int>(std::round(value * 100.0f));
        }

        int CalculateChecksum(const ReplayWorld& world)
        {
            // A real replay checksum would use a stronger hash, but this is enough to teach the idea.
            int checksum = 17;
            checksum = checksum * 31 + Quantize(world.playerPosition.x);
            checksum = checksum * 31 + Quantize(world.playerPosition.y);
            checksum = checksum * 31 + world.stamina;
            checksum = checksum * 31 + world.comboCounter;
            return checksum;
        }

        ReplayWorld SimulateReplay(const std::vector<GameplayCommand>& commandBuffer, int maxFrame)
        {
            ReplayWorld world;
            std::vector<GameplayCommand> sortedCommands = commandBuffer;
            std::stable_sort(sortedCommands.begin(), sortedCommands.end(), [](const GameplayCommand& a, const GameplayCommand& b)
            {
                return a.frame < b.frame;
            });

            std::size_t nextCommand = 0;

            for (int frame = 0; frame <= maxFrame; ++frame)
            {
                while (nextCommand < sortedCommands.size() && sortedCommands[nextCommand].frame == frame)
                {
                    ApplyCommand(world, sortedCommands[nextCommand].type);
                    ++nextCommand;
                }
            }

            return world;
        }

        void PrintCommandBuffer(const std::vector<GameplayCommand>& commandBuffer)
        {
            for (const GameplayCommand& command : commandBuffer)
            {
                std::cout << "    frame=" << command.frame
                          << " command=" << CommandName(command.type) << '\n';
            }
        }
    }

    void RunDeterministicReplayDemo()
    {
        std::cout << "[14] Master Tutorial: Deterministic Replay, Command Buffer System\n";

        const std::vector<GameplayCommand> commandBuffer = {
            { 0, CommandType::MoveRight },
            { 2, CommandType::Dash },
            { 1, CommandType::MoveRight },
            { 3, CommandType::Attack },
            { 4, CommandType::MoveUp },
            { 5, CommandType::Attack }
        };

        PrintCommandBuffer(commandBuffer);

        const ReplayWorld firstRun = SimulateReplay(commandBuffer, 5);
        const ReplayWorld secondRun = SimulateReplay(commandBuffer, 5);
        const int firstChecksum = CalculateChecksum(firstRun);
        const int secondChecksum = CalculateChecksum(secondRun);

        std::cout << "  First checksum=" << firstChecksum << '\n';
        std::cout << "  Second checksum=" << secondChecksum << '\n';
        std::cout << "  Deterministic=" << (firstChecksum == secondChecksum ? "true" : "false") << '\n';
        std::cout << '\n';
    }
}
