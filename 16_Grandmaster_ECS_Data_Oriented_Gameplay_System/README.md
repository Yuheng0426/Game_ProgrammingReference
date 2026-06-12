# 16 Grandmaster - ECS, Data-Oriented Gameplay System

This folder teaches a small Entity Component System (ECS) style gameplay architecture.

## What You Learn

- Treat an entity as an ID instead of a large inheritance-based object.
- Store gameplay data in separate component arrays.
- Write systems that process only the components they need.
- Apply damage through events so combat code does not directly own health storage.
- Remove dead entities safely after systems finish their work.

## Why It Is Harder

Traditional beginner code often creates a `Player` class, an `Enemy` class, and a `Projectile` class. That is easy to start with, but it can become hard to scale when hundreds or thousands of objects share the same operations.

ECS is more advanced because you must think in terms of data layout and system passes:

1. `Entity` is only an integer ID.
2. `PositionComponent` stores position data.
3. `VelocityComponent` stores movement data.
4. `HealthComponentData` stores health data.
5. Systems combine matching components to update the world.

## Reading Order

1. Read the component structs first.
2. Read `ECSWorld` to see where data is stored.
3. Read `MovementSystem`.
4. Read `DamageSystem`.
5. Read `CleanupDestroyedEntities`.
6. Run the demo and compare active entity counts before and after cleanup.

## Practice

- Add a `TeamComponent` so projectiles can ignore friendly targets.
- Add a `LifetimeComponent` and remove expired entities.
- Add a `RenderNameComponent` for debug printing.
- Change `DamageSystem` so armor reduces incoming damage.
