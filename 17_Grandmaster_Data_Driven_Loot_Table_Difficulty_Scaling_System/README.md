# 17 Grandmaster - Data-Driven Loot Table, Difficulty Scaling System

This folder teaches a data-driven loot table with weighted rolls, difficulty scaling, and pity protection.

## What You Learn

- Separate loot data from loot evaluation code.
- Use deterministic random numbers for stable tests.
- Scale item weights based on difficulty.
- Add pity protection so rare drops become more likely after repeated failures.
- Return explainable roll results for debugging and balancing.

## Why It Is Harder

Loot systems are not just random numbers. Real games often need fairness, tuning, logs, repeatable tests, rarity rules, difficulty modifiers, and player-facing trust. This example keeps the data small, but the structure is close to what larger games need.

## Reading Order

1. Read `LootEntry` to understand item data.
2. Read `LootContext` to understand player and encounter state.
3. Read `BuildScaledTable` to see how difficulty changes weights.
4. Read `RollLoot` to see the weighted roll.
5. Read the pity logic and observe how it protects unlucky players.

## Practice

- Add a `Legendary` rarity.
- Add a boss-only item.
- Add a modifier that improves loot when the player has low health.
- Print the final scaled weights for every item.
