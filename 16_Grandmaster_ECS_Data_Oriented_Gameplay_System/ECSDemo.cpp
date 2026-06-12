#include "16_Grandmaster_ECS_Data_Oriented_Gameplay_System/ECSDemo.h"

// Grandmaster architecture lesson: Entity Component System (ECS).
//
// Instead of large inheritance hierarchies such as Player -> Character -> Actor,
// this file stores data in component collections and runs systems over matching
// components. This is a simplified teaching version, but it introduces the same
// mental model used by many high-performance gameplay architectures.

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        using Entity = std::uint32_t;

        struct PositionComponent
        {
            // Components should usually store data, not behavior.
            // Systems decide how the data changes.
            Vector2 value;
        };

        struct VelocityComponent
        {
            Vector2 value;
        };

        struct HealthComponentData
        {
            int current = 1;
            int max = 1;
        };

        struct DamageEvent
        {
            // Events let systems request work without immediately mutating every component.
            // This makes update order more explicit and easier to debug.
            Entity target = 0;
            int amount = 0;
            std::string source;
        };

        class ECSWorld
        {
        public:
            // Entity creation returns an ID. The ID becomes meaningful only when
            // components are attached to it.
            Entity CreateEntity()
            {
                const Entity entity = nextEntity;
                ++nextEntity;
                livingEntities.push_back(entity);
                return entity;
            }

            void AddPosition(Entity entity, const Vector2& position)
            {
                positions[entity] = PositionComponent{ position };
            }

            void AddVelocity(Entity entity, const Vector2& velocity)
            {
                velocities[entity] = VelocityComponent{ velocity };
            }

            void AddHealth(Entity entity, int maxHealth)
            {
                const int safeMaxHealth = std::max(1, maxHealth);
                healthValues[entity] = HealthComponentData{ safeMaxHealth, safeMaxHealth };
            }

            void QueueDamage(Entity target, int amount, const std::string& source)
            {
                // Damage is queued as an event so multiple systems can create damage without directly changing health.
                damageEvents.push_back(DamageEvent{ target, std::max(0, amount), source });
            }

            std::unordered_map<Entity, PositionComponent>& Positions()
            {
                return positions;
            }

            const std::unordered_map<Entity, PositionComponent>& Positions() const
            {
                return positions;
            }

            std::unordered_map<Entity, VelocityComponent>& Velocities()
            {
                return velocities;
            }

            const std::unordered_map<Entity, VelocityComponent>& Velocities() const
            {
                return velocities;
            }

            std::unordered_map<Entity, HealthComponentData>& HealthValues()
            {
                return healthValues;
            }

            const std::unordered_map<Entity, HealthComponentData>& HealthValues() const
            {
                return healthValues;
            }

            std::vector<DamageEvent>& DamageEvents()
            {
                return damageEvents;
            }

            const std::vector<Entity>& LivingEntities() const
            {
                return livingEntities;
            }

            void DestroyEntity(Entity entity)
            {
                // Removing every component keeps later systems from accidentally processing a dead entity.
                positions.erase(entity);
                velocities.erase(entity);
                healthValues.erase(entity);

                livingEntities.erase(
                    std::remove(livingEntities.begin(), livingEntities.end(), entity),
                    livingEntities.end());
            }

        private:
            Entity nextEntity = 1;
            std::vector<Entity> livingEntities;
            std::unordered_map<Entity, PositionComponent> positions;
            std::unordered_map<Entity, VelocityComponent> velocities;
            std::unordered_map<Entity, HealthComponentData> healthValues;
            std::vector<DamageEvent> damageEvents;
        };

        void MovementSystem(ECSWorld& world, float deltaSeconds)
        {
            for (auto& positionEntry : world.Positions())
            {
                const Entity entity = positionEntry.first;
                PositionComponent& position = positionEntry.second;

                const auto velocityIt = world.Velocities().find(entity);
                if (velocityIt == world.Velocities().end())
                {
                    continue;
                }

                // This system only needs Position and Velocity. It does not care whether the entity is a player, enemy, or projectile.
                const VelocityComponent& velocity = velocityIt->second;
                position.value += velocity.value * deltaSeconds;
            }
        }

        void DamageSystem(ECSWorld& world)
        {
            for (const DamageEvent& event : world.DamageEvents())
            {
                auto healthIt = world.HealthValues().find(event.target);
                if (healthIt == world.HealthValues().end())
                {
                    continue;
                }

                HealthComponentData& health = healthIt->second;
                health.current = std::max(0, health.current - event.amount);

                std::cout << "    Damage source=" << event.source
                          << " target=" << event.target
                          << " amount=" << event.amount
                          << " hp=" << health.current << '/' << health.max << '\n';
            }

            world.DamageEvents().clear();
        }

        void CleanupDestroyedEntities(ECSWorld& world)
        {
            std::vector<Entity> entitiesToDestroy;

            for (const auto& healthEntry : world.HealthValues())
            {
                if (healthEntry.second.current <= 0)
                {
                    entitiesToDestroy.push_back(healthEntry.first);
                }
            }

            for (Entity entity : entitiesToDestroy)
            {
                std::cout << "    Destroy entity=" << entity << '\n';
                world.DestroyEntity(entity);
            }
        }

        void PrintPositions(const ECSWorld& world, const std::string& label)
        {
            std::cout << "  " << label << '\n';

            for (Entity entity : world.LivingEntities())
            {
                const auto positionIt = world.Positions().find(entity);
                if (positionIt == world.Positions().end())
                {
                    continue;
                }

                std::cout << "    entity=" << entity
                          << " position=(" << positionIt->second.value.x
                          << ", " << positionIt->second.value.y << ")\n";
            }
        }
    }

    void RunECSDemo()
    {
        std::cout << "[16] Grandmaster Tutorial: ECS, Data-Oriented Gameplay System\n";

        ECSWorld world;

        const Entity player = world.CreateEntity();
        world.AddPosition(player, { 0.0f, 0.0f });
        world.AddVelocity(player, { 1.0f, 0.0f });
        world.AddHealth(player, 120);

        const Entity enemy = world.CreateEntity();
        world.AddPosition(enemy, { 3.0f, 0.0f });
        world.AddVelocity(enemy, { -0.5f, 0.0f });
        world.AddHealth(enemy, 45);

        const Entity projectile = world.CreateEntity();
        world.AddPosition(projectile, { 1.0f, 0.0f });
        world.AddVelocity(projectile, { 3.0f, 0.0f });

        PrintPositions(world, "Before systems");

        constexpr float deltaSeconds = 0.5f;
        MovementSystem(world, deltaSeconds);
        world.QueueDamage(enemy, 25, "PlayerProjectile");
        world.QueueDamage(enemy, 30, "ExplosionOverlap");
        DamageSystem(world);
        CleanupDestroyedEntities(world);

        PrintPositions(world, "After systems");
        std::cout << "  Living entities=" << world.LivingEntities().size() << '\n';
        std::cout << '\n';
    }
}
