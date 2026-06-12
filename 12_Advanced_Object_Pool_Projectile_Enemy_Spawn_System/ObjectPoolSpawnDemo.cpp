#include "12_Advanced_Object_Pool_Projectile_Enemy_Spawn_System/ObjectPoolSpawnDemo.h"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        struct Projectile
        {
            bool active = false;
            Vector2 position;
            Vector2 velocity;
            float lifetimeSeconds = 0.0f;
            int damage = 0;
        };

        struct SpawnedEnemy
        {
            bool active = false;
            std::string archetype;
            Vector2 position;
            HealthComponent health{ 1 };
        };

        template <typename T>
        class ObjectPool
        {
        public:
            explicit ObjectPool(std::size_t capacity) : objects(capacity) {}

            T* Acquire()
            {
                for (T& object : objects)
                {
                    if (!object.active)
                    {
                        object.active = true;
                        return &object;
                    }
                }

                return nullptr;
            }

            [[nodiscard]] std::size_t ActiveCount() const
            {
                std::size_t count = 0;
                for (const T& object : objects)
                {
                    if (object.active)
                    {
                        ++count;
                    }
                }

                return count;
            }

            std::vector<T>& Items()
            {
                return objects;
            }

        private:
            std::vector<T> objects;
        };

        void SpawnProjectile(ObjectPool<Projectile>& pool, const Vector2& position, const Vector2& direction)
        {
            Projectile* projectile = pool.Acquire();
            if (projectile == nullptr)
            {
                std::cout << "    Projectile pool is full. Spawn request ignored.\n";
                return;
            }

            projectile->position = position;
            projectile->velocity = direction.Normalized() * 8.0f;
            projectile->lifetimeSeconds = 0.75f;
            projectile->damage = 20;
        }

        void SpawnEnemy(ObjectPool<SpawnedEnemy>& pool, const std::string& archetype, const Vector2& position, int health)
        {
            SpawnedEnemy* enemy = pool.Acquire();
            if (enemy == nullptr)
            {
                std::cout << "    Enemy pool is full. Spawn request ignored.\n";
                return;
            }

            enemy->archetype = archetype;
            enemy->position = position;
            enemy->health = HealthComponent(health);
        }

        void UpdateProjectiles(ObjectPool<Projectile>& pool, float deltaSeconds)
        {
            for (Projectile& projectile : pool.Items())
            {
                if (!projectile.active)
                {
                    continue;
                }

                projectile.position += projectile.velocity * deltaSeconds;
                projectile.lifetimeSeconds -= deltaSeconds;

                if (projectile.lifetimeSeconds <= 0.0f)
                {
                    projectile.active = false;
                }
            }
        }

        void UpdateEnemies(ObjectPool<SpawnedEnemy>& pool, const Vector2& playerPosition, float deltaSeconds)
        {
            for (SpawnedEnemy& enemy : pool.Items())
            {
                if (!enemy.active)
                {
                    continue;
                }

                const Vector2 toPlayer = playerPosition - enemy.position;
                enemy.position += toPlayer.Normalized() * 1.5f * deltaSeconds;

                if (!enemy.health.IsAlive())
                {
                    enemy.active = false;
                }
            }
        }
    }

    void RunObjectPoolSpawnDemo()
    {
        std::cout << "[12] Advanced Tutorial: Object Pool, Projectile, Enemy Spawn System\n";

        ObjectPool<Projectile> projectilePool(3);
        ObjectPool<SpawnedEnemy> enemyPool(2);
        const Vector2 playerPosition{ 0.0f, 0.0f };

        SpawnEnemy(enemyPool, "Skeleton", { 4.0f, 0.0f }, 40);
        SpawnEnemy(enemyPool, "Archer", { 5.0f, 1.0f }, 30);
        SpawnEnemy(enemyPool, "ExtraEnemy", { 6.0f, 0.0f }, 20);

        constexpr float deltaSeconds = 0.25f;
        for (int frame = 0; frame < 5; ++frame)
        {
            SpawnProjectile(projectilePool, playerPosition, { 1.0f, 0.2f });
            UpdateProjectiles(projectilePool, deltaSeconds);
            UpdateEnemies(enemyPool, playerPosition, deltaSeconds);

            std::cout << "  Frame " << frame
                      << " activeProjectiles=" << projectilePool.ActiveCount()
                      << " activeEnemies=" << enemyPool.ActiveCount() << '\n';
        }

        std::cout << '\n';
    }
}
