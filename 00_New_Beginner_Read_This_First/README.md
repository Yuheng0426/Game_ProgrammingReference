# New Beginner Read This First

If you are new to C++ game programming, start here.

This repository has many advanced folders, but you do not need to understand all of them today. The best way to learn is to read one small lesson, change one number, run the program, and observe what changed.

## What To Read First

Read in this order:

1. `README.md`
2. `00_Start_Here_Beginner_To_Expert_Learning_Roadmap`
3. `00_New_Beginner_Read_This_First`
4. `01_Game_Loop_Input_Collision`
5. `08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial`
6. `02_Shooter_Weapon_Bullet_System`
7. `03_Soulslike_Stamina_Dodge_Parry_Combat`
8. `04_Action_RPG_Ability_Inventory_Quest`

Do not start with rollback networking, ECS, behavior trees, or loot table scaling. Those are useful later, but they are not the first step.

## How To Read A Code File

Use this method:

1. Read the file-level comment at the top.
2. Read each `struct` and ask: what data does this store?
3. Read each function name and ask: what job does this function do?
4. Ignore advanced syntax at first.
5. Run the demo.
6. Change one number.
7. Run again.

## Words New Learners Should Know

`Frame`

A single update step of the game. If a game runs at 60 FPS, it updates about 60 frames per second.

`Delta seconds`

The amount of time that passed during one frame. Movement usually uses speed multiplied by delta seconds.

`Vector2`

A pair of numbers, usually x and y. In this project it represents positions, directions, and velocities.

`Position`

Where something is in the game world.

`Velocity`

How fast and in what direction something is moving.

`Collider`

A simple shape used to check whether two objects touch.

`Cooldown`

A timer that prevents an action from happening again immediately.

`Resource`

A value such as stamina, mana, or energy that can be spent and recovered.

## Beginner Practice

Try these small changes:

- In `01_Game_Loop_Input_Collision`, change `walkSpeed` from `3.0f` to `2.0f`.
- In `01_Game_Loop_Input_Collision`, move the `Spike Trap` farther away.
- In `08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial`, change `acceleration`.
- In `08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial`, change `followSharpness`.
- Run the program after each change.

## Do Not Worry If You Do Not Understand Everything

That is normal. Game programming combines math, logic, timing, tools, and design. The goal is not to memorize everything. The goal is to understand one small system at a time.
