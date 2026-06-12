# 12 Advanced - Object Pool, Projectile, Enemy Spawn System

This folder teaches object pooling, a common performance technique used in games that spawn many short-lived objects.

## What You Learn

- Reuse inactive objects instead of allocating new objects every spawn.
- Track active and inactive gameplay objects.
- Update pooled projectiles and enemies.
- Return expired objects to the pool safely.

## Why It Matters

Shooters, action RPGs, bullet hell games, enemy wave games, and VFX-heavy games often create many bullets, enemies, hit effects, and pickups. Object pools reduce runtime allocations and make spawn behavior easier to control.
