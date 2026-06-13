#include "17_Grandmaster_Data_Driven_Loot_Table_Difficulty_Scaling_System/LootTableDemo.h"

// Grandmaster systems lesson: data-driven loot tables.
//
// A beginner loot system might write "if random < 5%, drop sword" directly in
// gameplay code. That becomes hard to balance. This lesson keeps item data in a
// table and evaluates that table with clear rules: difficulty scaling, pity
// protection, deterministic random numbers, and debug-friendly roll results.

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace game_ref
{
    namespace
    {
        enum class LootRarity
        {
            Common,
            Rare,
            Epic
        };

        struct LootEntry
        {
            // baseWeight is designer-authored data. Higher weight means the item is more likely.
            std::string itemId;
            LootRarity rarity = LootRarity::Common;
            int baseWeight = 1;
            int minimumDifficulty = 1;
        };

        struct ScaledLootEntry
        {
            std::string itemId;
            LootRarity rarity = LootRarity::Common;
            int finalWeight = 0;
        };

        struct LootContext
        {
            // The context describes the current roll. It can include difficulty,
            // player luck, pity counters, boss flags, world tier, or event bonuses.
            int difficultyLevel = 1;
            int playerLuck = 0;
            int rareMissStreak = 0;
        };

        struct LootRollResult
        {
            std::string itemId;
            int randomValue = 0;
            int totalWeight = 0;
            bool pityApplied = false;
        };

        const char* RarityName(LootRarity rarity)
        {
            switch (rarity)
            {
            case LootRarity::Common: return "Common";
            case LootRarity::Rare: return "Rare";
            case LootRarity::Epic: return "Epic";
            }

            return "Unknown";
        }

        class DeterministicRandom
        {
        public:
            explicit DeterministicRandom(std::uint32_t seed) : state(seed) {}

            int RangeInclusive(int minValue, int maxValue)
            {
                // Linear congruential generators are not ideal for real loot security,
                // but they are tiny and deterministic, which makes them useful for lessons.
                state = state * 1664525u + 1013904223u;
                const std::uint32_t span = static_cast<std::uint32_t>(maxValue - minValue + 1);
                return minValue + static_cast<int>(state % span);
            }

        private:
            std::uint32_t state = 0;
        };

        int DifficultyBonus(const LootEntry& entry, const LootContext& context)
        {
            if (context.difficultyLevel < entry.minimumDifficulty)
            {
                return 0;
            }

            const int difficultyAboveMinimum = context.difficultyLevel - entry.minimumDifficulty;
            if (entry.rarity == LootRarity::Epic)
            {
                return difficultyAboveMinimum * 2;
            }

            if (entry.rarity == LootRarity::Rare)
            {
                return difficultyAboveMinimum;
            }

            return 0;
        }

        int PityBonus(const LootEntry& entry, const LootContext& context)
        {
            // Pity protection helps unlucky players by increasing rare or epic weights
            // after repeated rolls without a high-rarity reward.
            if (entry.rarity == LootRarity::Common)
            {
                return 0;
            }

            return context.rareMissStreak >= 3 ? context.rareMissStreak : 0;
        }

        std::vector<ScaledLootEntry> BuildScaledTable(
            const std::vector<LootEntry>& table,
            const LootContext& context)
        {
            std::vector<ScaledLootEntry> scaledTable;

            for (const LootEntry& entry : table)
            {
                if (context.difficultyLevel < entry.minimumDifficulty)
                {
                    continue;
                }

                const int luckBonus = entry.rarity == LootRarity::Common ? 0 : context.playerLuck;
                const int finalWeight = std::max(0,
                    entry.baseWeight +
                    DifficultyBonus(entry, context) +
                    PityBonus(entry, context) +
                    luckBonus);

                if (finalWeight > 0)
                {
                    scaledTable.push_back({ entry.itemId, entry.rarity, finalWeight });
                }
            }

            return scaledTable;
        }

        LootRollResult RollLoot(
            const std::vector<LootEntry>& table,
            const LootContext& context,
            DeterministicRandom& random)
        {
            const std::vector<ScaledLootEntry> scaledTable = BuildScaledTable(table, context);
            const int totalWeight = std::accumulate(
                scaledTable.begin(),
                scaledTable.end(),
                0,
                [](int sum, const ScaledLootEntry& entry)
                {
                    return sum + entry.finalWeight;
                });

            if (totalWeight <= 0)
            {
                return { "NoDrop", 0, 0, false };
            }

            const int roll = random.RangeInclusive(1, totalWeight);
            int runningWeight = 0;

            for (const ScaledLootEntry& entry : scaledTable)
            {
                runningWeight += entry.finalWeight;
                if (roll <= runningWeight)
                {
                    return {
                        entry.itemId,
                        roll,
                        totalWeight,
                        context.rareMissStreak >= 3 && entry.rarity != LootRarity::Common
                    };
                }
            }

            return { "NoDrop", roll, totalWeight, false };
        }

        void PrintScaledTable(const std::vector<LootEntry>& table, const LootContext& context)
        {
            const std::vector<ScaledLootEntry> scaledTable = BuildScaledTable(table, context);
            std::cout << "  Scaled loot table difficulty=" << context.difficultyLevel
                      << " luck=" << context.playerLuck
                      << " missStreak=" << context.rareMissStreak << '\n';

            for (const ScaledLootEntry& entry : scaledTable)
            {
                std::cout << "    " << std::setw(14) << entry.itemId
                          << " rarity=" << RarityName(entry.rarity)
                          << " weight=" << entry.finalWeight << '\n';
            }
        }
    }

    void RunLootTableDemo()
    {
        std::cout << "[17] Grandmaster Tutorial: Data-Driven Loot Table, Difficulty Scaling System\n";

        const std::vector<LootEntry> lootTable = {
            { "IronOre", LootRarity::Common, 60, 1 },
            { "HealthPotion", LootRarity::Common, 30, 1 },
            { "RareGem", LootRarity::Rare, 8, 2 },
            { "AncientCore", LootRarity::Epic, 2, 4 }
        };

        LootContext context;
        context.difficultyLevel = 4;
        context.playerLuck = 1;
        context.rareMissStreak = 4;

        DeterministicRandom random(1337u);

        PrintScaledTable(lootTable, context);

        for (int rollIndex = 0; rollIndex < 4; ++rollIndex)
        {
            const LootRollResult result = RollLoot(lootTable, context, random);
            std::cout << "  Roll " << rollIndex
                      << " item=" << result.itemId
                      << " random=" << result.randomValue << '/' << result.totalWeight
                      << " pity=" << (result.pityApplied ? "true" : "false") << '\n';
        }

        std::cout << '\n';
    }
}
