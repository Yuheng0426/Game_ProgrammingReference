#include "11_Expert_Gameplay_Event_Bus_Achievement_System/ExpertEventBusDemo.h"

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace game_ref
{
    namespace
    {
        enum class GameplayEventType
        {
            EnemyDefeated,
            ItemCollected,
            CheckpointReached
        };

        struct GameplayEvent
        {
            GameplayEventType type;
            std::string name;
            int amount = 1;
        };

        class GameplayEventBus
        {
        public:
            using Listener = std::function<void(const GameplayEvent&)>;

            void Subscribe(GameplayEventType type, Listener listener)
            {
                listeners[type].push_back(std::move(listener));
            }

            void Publish(const GameplayEvent& event) const
            {
                const auto listenerIt = listeners.find(event.type);
                if (listenerIt == listeners.end())
                {
                    return;
                }

                for (const Listener& listener : listenerIt->second)
                {
                    listener(event);
                }
            }

        private:
            std::unordered_map<GameplayEventType, std::vector<Listener>> listeners;
        };

        class AchievementTracker
        {
        public:
            void Register(GameplayEventBus& eventBus)
            {
                eventBus.Subscribe(GameplayEventType::EnemyDefeated, [this](const GameplayEvent& event)
                {
                    defeatedEnemies += event.amount;
                    if (defeatedEnemies >= 3)
                    {
                        Unlock("First Hunt");
                    }
                });

                eventBus.Subscribe(GameplayEventType::ItemCollected, [this](const GameplayEvent& event)
                {
                    if (event.name == "Ancient Relic")
                    {
                        Unlock("Relic Finder");
                    }
                });

                eventBus.Subscribe(GameplayEventType::CheckpointReached, [this](const GameplayEvent& event)
                {
                    if (event.name == "Ruined Gate")
                    {
                        Unlock("Gate Reached");
                    }
                });
            }

        private:
            void Unlock(const std::string& achievementName)
            {
                if (unlockedAchievements[achievementName])
                {
                    return;
                }

                unlockedAchievements[achievementName] = true;
                std::cout << "    Achievement unlocked: " << achievementName << '\n';
            }

            int defeatedEnemies = 0;
            std::unordered_map<std::string, bool> unlockedAchievements;
        };
    }

    void RunExpertEventBusDemo()
    {
        std::cout << "[11] Expert Tutorial: Gameplay Event Bus, Achievement System\n";

        GameplayEventBus eventBus;
        AchievementTracker achievements;
        achievements.Register(eventBus);

        eventBus.Publish({ GameplayEventType::CheckpointReached, "Ruined Gate", 1 });
        eventBus.Publish({ GameplayEventType::EnemyDefeated, "Hollow Guard", 1 });
        eventBus.Publish({ GameplayEventType::EnemyDefeated, "Hollow Archer", 1 });
        eventBus.Publish({ GameplayEventType::ItemCollected, "Ancient Relic", 1 });
        eventBus.Publish({ GameplayEventType::EnemyDefeated, "Gate Brute", 1 });

        std::cout << '\n';
    }
}
