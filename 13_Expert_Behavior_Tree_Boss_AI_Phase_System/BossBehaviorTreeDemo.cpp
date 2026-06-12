#include "13_Expert_Behavior_Tree_Boss_AI_Phase_System/BossBehaviorTreeDemo.h"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Common_Game_Code/Gameplay/CooldownTimer.h"
#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Math/Vector2.h"

namespace game_ref
{
    namespace
    {
        enum class BehaviorResult
        {
            Success,
            Failure,
            Running
        };

        struct BossBlackboard
        {
            Vector2 bossPosition;
            Vector2 playerPosition;
            HealthComponent bossHealth{ 300 };
            CooldownTimer heavyAttackCooldown;
            CooldownTimer enrageAttackCooldown;
            bool enraged = false;
            std::string lastDecision;
        };

        class BehaviorNode
        {
        public:
            virtual ~BehaviorNode() = default;
            virtual BehaviorResult Tick(BossBlackboard& blackboard, float deltaSeconds) = 0;
        };

        class ActionNode final : public BehaviorNode
        {
        public:
            using Action = std::function<BehaviorResult(BossBlackboard&, float)>;

            explicit ActionNode(Action action) : actionFunction(std::move(action)) {}

            BehaviorResult Tick(BossBlackboard& blackboard, float deltaSeconds) override
            {
                return actionFunction(blackboard, deltaSeconds);
            }

        private:
            Action actionFunction;
        };

        class SequenceNode final : public BehaviorNode
        {
        public:
            explicit SequenceNode(std::vector<std::unique_ptr<BehaviorNode>> children)
                : childNodes(std::move(children))
            {
            }

            BehaviorResult Tick(BossBlackboard& blackboard, float deltaSeconds) override
            {
                for (const std::unique_ptr<BehaviorNode>& child : childNodes)
                {
                    const BehaviorResult result = child->Tick(blackboard, deltaSeconds);
                    if (result != BehaviorResult::Success)
                    {
                        return result;
                    }
                }

                return BehaviorResult::Success;
            }

        private:
            std::vector<std::unique_ptr<BehaviorNode>> childNodes;
        };

        class SelectorNode final : public BehaviorNode
        {
        public:
            explicit SelectorNode(std::vector<std::unique_ptr<BehaviorNode>> children)
                : childNodes(std::move(children))
            {
            }

            BehaviorResult Tick(BossBlackboard& blackboard, float deltaSeconds) override
            {
                for (const std::unique_ptr<BehaviorNode>& child : childNodes)
                {
                    const BehaviorResult result = child->Tick(blackboard, deltaSeconds);
                    if (result != BehaviorResult::Failure)
                    {
                        return result;
                    }
                }

                return BehaviorResult::Failure;
            }

        private:
            std::vector<std::unique_ptr<BehaviorNode>> childNodes;
        };

        std::unique_ptr<BehaviorNode> MakeAction(ActionNode::Action action)
        {
            return std::make_unique<ActionNode>(std::move(action));
        }

        template <typename... Nodes>
        std::unique_ptr<BehaviorNode> MakeSequence(Nodes&&... nodes)
        {
            std::vector<std::unique_ptr<BehaviorNode>> children;
            children.reserve(sizeof...(Nodes));

            // Move-only nodes cannot be copied through initializer lists, so build the vector explicitly.
            (children.push_back(std::move(nodes)), ...);
            return std::make_unique<SequenceNode>(std::move(children));
        }

        float HealthPercent(const BossBlackboard& blackboard)
        {
            return static_cast<float>(blackboard.bossHealth.Current()) /
                   static_cast<float>(blackboard.bossHealth.Max());
        }

        std::unique_ptr<BehaviorNode> CreateBossBehaviorTree()
        {
            std::vector<std::unique_ptr<BehaviorNode>> rootChoices;

            rootChoices.push_back(MakeSequence(
                MakeAction([](BossBlackboard& blackboard, float)
                {
                    if (blackboard.enraged || HealthPercent(blackboard) > 0.5f)
                    {
                        return BehaviorResult::Failure;
                    }

                    blackboard.enraged = true;
                    blackboard.lastDecision = "EnterPhaseTwo";
                    return BehaviorResult::Success;
                }),
                MakeAction([](BossBlackboard& blackboard, float)
                {
                    blackboard.enrageAttackCooldown.Start(0.4f);
                    blackboard.lastDecision += " -> Roar";
                    return BehaviorResult::Success;
                })
            ));

            rootChoices.push_back(MakeSequence(
                MakeAction([](BossBlackboard& blackboard, float)
                {
                    return blackboard.enraged && blackboard.enrageAttackCooldown.IsReady()
                        ? BehaviorResult::Success
                        : BehaviorResult::Failure;
                }),
                MakeAction([](BossBlackboard& blackboard, float)
                {
                    blackboard.enrageAttackCooldown.Start(1.0f);
                    blackboard.lastDecision = "PhaseTwoFlameWave";
                    return BehaviorResult::Success;
                })
            ));

            rootChoices.push_back(MakeSequence(
                MakeAction([](BossBlackboard& blackboard, float)
                {
                    return Distance(blackboard.bossPosition, blackboard.playerPosition) <= 1.5f &&
                           blackboard.heavyAttackCooldown.IsReady()
                        ? BehaviorResult::Success
                        : BehaviorResult::Failure;
                }),
                MakeAction([](BossBlackboard& blackboard, float)
                {
                    blackboard.heavyAttackCooldown.Start(0.8f);
                    blackboard.lastDecision = "HeavyMeleeAttack";
                    return BehaviorResult::Success;
                })
            ));

            rootChoices.push_back(MakeAction([](BossBlackboard& blackboard, float deltaSeconds)
            {
                const Vector2 toPlayer = blackboard.playerPosition - blackboard.bossPosition;
                blackboard.bossPosition += toPlayer.Normalized() * 1.2f * deltaSeconds;
                blackboard.lastDecision = "AdvanceTowardPlayer";
                return BehaviorResult::Running;
            }));

            return std::make_unique<SelectorNode>(std::move(rootChoices));
        }
    }

    void RunBossBehaviorTreeDemo()
    {
        std::cout << "[13] Expert Tutorial: Behavior Tree, Boss AI, Phase System\n";

        BossBlackboard blackboard;
        blackboard.bossPosition = { 4.0f, 0.0f };
        blackboard.playerPosition = { 0.0f, 0.0f };

        std::unique_ptr<BehaviorNode> bossTree = CreateBossBehaviorTree();

        constexpr float deltaSeconds = 0.25f;
        for (int frame = 0; frame < 8; ++frame)
        {
            blackboard.heavyAttackCooldown.Update(deltaSeconds);
            blackboard.enrageAttackCooldown.Update(deltaSeconds);

            if (frame == 3)
            {
                blackboard.bossHealth.ApplyDamage(170);
            }

            bossTree->Tick(blackboard, deltaSeconds);

            std::cout << "  Frame " << frame
                      << " hp=" << blackboard.bossHealth.Current() << '/' << blackboard.bossHealth.Max()
                      << " enraged=" << (blackboard.enraged ? "true" : "false")
                      << " decision=" << blackboard.lastDecision
                      << " bossX=" << blackboard.bossPosition.x << '\n';
        }

        std::cout << '\n';
    }
}
