#include "04_Action_RPG_Ability_Inventory_Quest/ActionRPGDemo.h"

// Action RPG lesson: ability casting, cooldowns, mana, items, quests, and rewards.
//
// This example teaches how many RPG systems touch each other:
// abilities damage enemies, enemy defeat grants experience, quest progress reacts
// to kills, and inventory items can restore health after taking damage.

#include <iostream>
#include <string>
#include <vector>

#include "Common_Game_Code/Gameplay/CooldownTimer.h"
#include "Common_Game_Code/Gameplay/HealthComponent.h"
#include "Common_Game_Code/Gameplay/ResourcePool.h"

namespace game_ref
{
    namespace
    {
        struct Ability
        {
            // Ability data is separated from cast validation so designers can tune
            // damage, cost, and cooldown without changing CastAbility.
            std::string name;
            int damage = 0;
            float manaCost = 0.0f;
            float cooldownSeconds = 0.0f;
            CooldownTimer cooldown;
        };

        struct InventoryItem
        {
            std::string name;
            int quantity = 0;
            int healing = 0;
        };

        struct Quest
        {
            // This quest tracks one simple objective: defeat a required number of enemies.
            std::string name;
            int requiredKills = 0;
            int currentKills = 0;
            bool completed = false;

            void RegisterKill()
            {
                if (completed)
                {
                    return;
                }

                ++currentKills;
                completed = currentKills >= requiredKills;
            }
        };

        struct Hero
        {
            int level = 1;
            int experience = 0;
            HealthComponent health{ 100 };
            ResourcePool mana{ 80.0f, 12.0f };
            std::vector<Ability> abilities;
            std::vector<InventoryItem> inventory;
        };

        struct Enemy
        {
            std::string name;
            HealthComponent health{ 50 };
            int experienceReward = 10;
            bool rewardClaimed = false;
        };

        bool CastAbility(Hero& hero, Ability& ability, Enemy& enemy)
        {
            // Ability validation is kept in one function so UI, AI, and networking can share the same rule.
            if (!ability.cooldown.IsReady() || !hero.mana.TrySpend(ability.manaCost) || !enemy.health.IsAlive())
            {
                return false;
            }

            enemy.health.ApplyDamage(ability.damage);
            ability.cooldown.Start(ability.cooldownSeconds);
            return true;
        }

        bool UseHealingItem(Hero& hero, const std::string& itemName)
        {
            for (InventoryItem& item : hero.inventory)
            {
                if (item.name == itemName && item.quantity > 0)
                {
                    --item.quantity;
                    hero.health.Heal(item.healing);
                    return true;
                }
            }

            return false;
        }

        void AddExperience(Hero& hero, int amount)
        {
            hero.experience += amount;
            const int experienceNeeded = hero.level * 50;

            if (hero.experience >= experienceNeeded)
            {
                hero.experience -= experienceNeeded;
                ++hero.level;
                hero.health.Heal(hero.health.Max());
                std::cout << "    Level up! New level=" << hero.level << '\n';
            }
        }
    }

    void RunActionRPGDemo()
    {
        std::cout << "[04] Action RPG Ability, Inventory, Quest\n";

        Hero hero;
        hero.abilities.push_back({ "Arc Slash", 32, 14.0f, 0.5f });
        hero.abilities.push_back({ "Flame Burst", 55, 30.0f, 1.2f });
        hero.inventory.push_back({ "Small Potion", 2, 35 });

        Quest quest{ "Clear the Ruined Gate", 2 };
        std::vector<Enemy> enemies = {
            { "Hollow Guard", HealthComponent(45), 30 },
            { "Gate Brute", HealthComponent(80), 55 }
        };

        constexpr float deltaSeconds = 0.4f;
        for (int turn = 0; turn < 6; ++turn)
        {
            for (Ability& ability : hero.abilities)
            {
                ability.cooldown.Update(deltaSeconds);
            }
            hero.mana.Update(deltaSeconds);

            Enemy& enemy = enemies[turn < 3 ? 0 : 1];
            Ability& chosenAbility = hero.abilities[turn == 4 ? 1 : 0];

            const bool cast = CastAbility(hero, chosenAbility, enemy);
            std::cout << "  Turn " << turn << " cast " << chosenAbility.name
                      << (cast ? " success" : " failed")
                      << " enemyHP=" << enemy.health.Current()
                      << " mana=" << hero.mana.Current() << '\n';

            if (!enemy.health.IsAlive() && !enemy.rewardClaimed)
            {
                enemy.rewardClaimed = true;
                quest.RegisterKill();
                AddExperience(hero, enemy.experienceReward);
                std::cout << "    Defeated " << enemy.name
                          << " quest=" << quest.currentKills << '/' << quest.requiredKills << '\n';
            }

            if (turn == 3)
            {
                hero.health.ApplyDamage(50);
                const bool used = UseHealingItem(hero, "Small Potion");
                std::cout << "    Potion " << (used ? "used" : "not used")
                          << " heroHP=" << hero.health.Current() << '/' << hero.health.Max() << '\n';
            }
        }

        std::cout << "  Quest completed=" << (quest.completed ? "true" : "false") << '\n';
        std::cout << '\n';
    }
}
