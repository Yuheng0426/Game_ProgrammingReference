#include "05_Enemy_AI_State_Machine_Patrol_Chase_Attack/EnemyAIDemo.h"

// Intermediate AI lesson: a readable finite state machine.
//
// The enemy has four states:
// Patrol: follow waypoints.
// Chase: move toward a visible player.
// Attack: damage the player when close enough and off cooldown.
// ReturnToPost: go back to spawn when the player is lost.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Common_Game_Code/Gameplay/CooldownTimer.h"
#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        enum class AIState
        {
            Patrol,
            Chase,
            Attack,
            ReturnToPost
        };

        struct PlayerTarget
        {
            Vector2 position;
            HealthComponent health{ 100 };
        };

        struct EnemyAI
        {
            // AI tuning values are kept on the enemy so designers can make
            // different guards faster, slower, more aggressive, or more aware.
            std::string name;
            Vector2 spawnPosition;
            Vector2 position;
            std::vector<Vector2> patrolPoints;
            std::size_t patrolIndex = 0;
            AIState state = AIState::Patrol;
            CooldownTimer attackCooldown;
            float sightRange = 4.0f;
            float attackRange = 0.85f;
            float patrolSpeed = 1.2f;
            float chaseSpeed = 2.4f;
            int attackDamage = 14;
        };

        const char* StateName(AIState state)
        {
            switch (state)
            {
            case AIState::Patrol: return "Patrol";
            case AIState::Chase: return "Chase";
            case AIState::Attack: return "Attack";
            case AIState::ReturnToPost: return "ReturnToPost";
            }

            return "Unknown";
        }

        void MoveTowards(Vector2& position, const Vector2& destination, float speed, float deltaSeconds)
        {
            const Vector2 offset = destination - position;
            const float distance = offset.Length();
            if (distance <= 0.001f)
            {
                return;
            }

            const float step = std::min(distance, speed * deltaSeconds);
            position += offset.Normalized() * step;
        }

        bool CanSeePlayer(const EnemyAI& enemy, const PlayerTarget& player)
        {
            return Distance(enemy.position, player.position) <= enemy.sightRange;
        }

        void UpdateEnemyAI(EnemyAI& enemy, PlayerTarget& player, float deltaSeconds)
        {
            enemy.attackCooldown.Update(deltaSeconds);

            switch (enemy.state)
            {
            case AIState::Patrol:
                if (CanSeePlayer(enemy, player))
                {
                    enemy.state = AIState::Chase;
                    break;
                }

                if (!enemy.patrolPoints.empty())
                {
                    const Vector2 waypoint = enemy.patrolPoints[enemy.patrolIndex];
                    MoveTowards(enemy.position, waypoint, enemy.patrolSpeed, deltaSeconds);

                    if (Distance(enemy.position, waypoint) <= 0.05f)
                    {
                        enemy.patrolIndex = (enemy.patrolIndex + 1) % enemy.patrolPoints.size();
                    }
                }
                break;

            case AIState::Chase:
                if (!CanSeePlayer(enemy, player))
                {
                    enemy.state = AIState::ReturnToPost;
                    break;
                }

                if (Distance(enemy.position, player.position) <= enemy.attackRange)
                {
                    enemy.state = AIState::Attack;
                    break;
                }

                MoveTowards(enemy.position, player.position, enemy.chaseSpeed, deltaSeconds);
                break;

            case AIState::Attack:
                if (Distance(enemy.position, player.position) > enemy.attackRange)
                {
                    enemy.state = AIState::Chase;
                    break;
                }

                if (enemy.attackCooldown.IsReady())
                {
                    player.health.ApplyDamage(enemy.attackDamage);
                    enemy.attackCooldown.Start(0.8f);
                    std::cout << "    " << enemy.name << " attacks player. PlayerHP="
                              << player.health.Current() << '/' << player.health.Max() << '\n';
                }
                break;

            case AIState::ReturnToPost:
                if (CanSeePlayer(enemy, player))
                {
                    enemy.state = AIState::Chase;
                    break;
                }

                MoveTowards(enemy.position, enemy.spawnPosition, enemy.patrolSpeed, deltaSeconds);
                if (Distance(enemy.position, enemy.spawnPosition) <= 0.05f)
                {
                    enemy.state = AIState::Patrol;
                }
                break;
            }
        }
    }

    void RunEnemyAIDemo()
    {
        std::cout << "[05] Enemy AI State Machine: Patrol, Chase, Attack\n";

        PlayerTarget player{ { 3.2f, 0.0f }, HealthComponent(100) };
        EnemyAI guard;
        guard.name = "Gate Guard";
        guard.spawnPosition = { 0.0f, 0.0f };
        guard.position = guard.spawnPosition;
        guard.patrolPoints = { { 0.0f, 0.0f }, { 1.5f, 0.0f }, { 1.5f, 1.0f } };

        constexpr float deltaSeconds = 0.25f;
        for (int frame = 0; frame < 12; ++frame)
        {
            if (frame == 7)
            {
                player.position = { 8.0f, 0.0f };
            }

            UpdateEnemyAI(guard, player, deltaSeconds);

            std::cout << "  Frame " << frame
                      << " state=" << StateName(guard.state)
                      << " enemy=(" << guard.position.x << ", " << guard.position.y << ")"
                      << " player=(" << player.position.x << ", " << player.position.y << ")\n";
        }

        std::cout << '\n';
    }
}
