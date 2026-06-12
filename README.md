# C++ Game Programming Reference

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/build-CMake-informational)
![Visual Studio](https://img.shields.io/badge/IDE-Visual%20Studio-purple)
![Learning Project](https://img.shields.io/badge/purpose-gameplay%20learning-green)
![Beginner Friendly](https://img.shields.io/badge/path-beginner%20to%20expert-brightgreen)
![CMake Build](https://github.com/Yuheng0426/Game_ProgrammingReference/actions/workflows/cmake.yml/badge.svg)

A practical C++ reference project for learning common gameplay systems used in shooter games, soulslike games, action RPGs, and general real-time games.

The project uses standard C++17 console demos first. This keeps the gameplay logic easy to read, easy to debug, and easy to move later into Unreal Engine, Unity native plugins, Raylib, SDL, or a custom engine.

## Start Here

New learners should open `00_Start_Here_Beginner_To_Expert_Learning_Roadmap` first. It gives a clean path from basic movement to advanced gameplay architecture.

Recommended order:

1. Read the roadmap.
2. Run the console program once.
3. Open one numbered folder at a time.
4. Read the folder README.
5. Read the `.cpp` file and follow the English comments.
6. Change one value, rebuild, and observe the output.

## Repository Tags

`cpp` `game-development` `gameplay-programming` `beginner-friendly` `shooter` `soulslike` `action-rpg` `ai-state-machine` `behavior-tree` `combat-system` `ecs` `data-oriented-design` `hitbox` `skill-tree` `object-pool` `replay-system` `rollback-netcode` `save-system` `event-bus` `learning-project`

## Folder Guide

Every major folder name describes the game system inside it. This is intentional so beginners can browse the repository without guessing what each folder means.

| Folder | Content |
| --- | --- |
| `00_Start_Here_Beginner_To_Expert_Learning_Roadmap` | A guided learning path that explains what to study first and how each folder builds on the last. |
| `Common_Game_Code` | Shared gameplay helpers such as vectors, health, cooldown timers, and resource pools. |
| `01_Game_Loop_Input_Collision` | Fixed update style loop, scripted input, movement, and simple collision checks. |
| `02_Shooter_Weapon_Bullet_System` | Weapon fire rules, magazine ammo, reload timing, bullet travel, and target hits. |
| `03_Soulslike_Stamina_Dodge_Parry_Combat` | Stamina spending, dodge invincibility frames, parry windows, damage, and stagger state. |
| `04_Action_RPG_Ability_Inventory_Quest` | Abilities, mana costs, cooldowns, potions, inventory items, quests, rewards, and leveling. |
| `05_Enemy_AI_State_Machine_Patrol_Chase_Attack` | Enemy AI states for patrolling, seeing the player, chasing, attacking, and returning to post. |
| `06_Action_Game_Combo_LockOn_Targeting_System` | Combo attack chains, input windows, target scoring, and lock-on target selection. |
| `07_Game_Save_Load_Player_Profile_Settings` | Save/load logic for player profile data, checkpoint name, level, experience, and settings. |
| `08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial` | Beginner tutorial for player movement, acceleration, world bounds, and camera follow. |
| `09_Intermediate_Combat_Hitbox_Hurtbox_Frame_Data_Tutorial` | Intermediate tutorial for active frames, hitboxes, hurtboxes, and one-hit-per-target attack rules. |
| `10_Advanced_Skill_Tree_Progression_Unlock_System` | Advanced tutorial for skill requirements, prerequisites, points, and progression unlocks. |
| `11_Expert_Gameplay_Event_Bus_Achievement_System` | Expert tutorial for event-driven gameplay communication and achievement unlocks. |
| `12_Advanced_Object_Pool_Projectile_Enemy_Spawn_System` | Advanced tutorial for object pooling, projectile reuse, enemy spawning, and active object tracking. |
| `13_Expert_Behavior_Tree_Boss_AI_Phase_System` | Expert tutorial for behavior tree nodes, boss decisions, cooldowns, and phase changes. |
| `14_Master_Deterministic_Replay_Command_Buffer_System` | Master tutorial for command buffers, deterministic replay, and checksum validation. |
| `15_Master_Rollback_Networking_Input_Prediction_System` | Master tutorial for client prediction, server correction, rollback, and replaying saved inputs. |
| `16_Grandmaster_ECS_Data_Oriented_Gameplay_System` | Grandmaster tutorial for entities, components, systems, damage events, and data-oriented cleanup. |
| `99_Project_Self_Checks_For_Game_Systems` | Lightweight self checks for core shared gameplay systems. |

## Learning Path

| Level | Study These Folders | Goal |
| --- | --- | --- |
| Beginner | `01`, `08` | Understand loops, input, movement, camera follow, and simple collision. |
| Core Gameplay | `02`, `03`, `04` | Learn shooter, soulslike, and action RPG gameplay rules. |
| Intermediate | `05`, `06`, `09` | Learn AI states, lock-on targeting, combo logic, and melee frame data. |
| Advanced | `07`, `10`, `12` | Learn persistence, player profiles, progression, skill tree unlock rules, object pools, and spawn systems. |
| Expert | `11`, `13`, `99` | Learn event-driven communication, boss behavior trees, phase logic, and self-checking core rules. |
| Master | `14`, `15` | Learn deterministic command replay, reproducible simulation, checksum validation, client prediction, rollback, and input replay. |
| Grandmaster | `16` | Learn ECS-style data separation, system passes, event queues, and data-oriented entity cleanup. |

## Build With CMake

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\Game_ProgrammingReference.exe
```

## Build With Visual Studio

Open `Game_ProgrammingReference.sln`, choose `x64`, then run either `Debug` or `Release`.

## Learning Notes

- All code comments are written in English for an international GitHub audience.
- Each `.cpp` and `.h` file now includes teaching comments that explain the purpose of the lesson, the data structures, and the main gameplay decisions.
- The demos are deterministic, so the output is stable and easy to compare while learning.
- The code avoids engine-specific APIs so the gameplay ideas stay portable.
- The examples favor clarity over cleverness. Each system is small enough to read in one sitting.
- GitHub Actions runs the CMake build and demo on Windows.
- `CONTRIBUTING.md` explains how to add new lesson folders cleanly.

## Current Gameplay Systems

- Core game loop, input, movement, and collision.
- Shooter weapon, ammo, reload, bullet, and hit detection logic.
- Soulslike stamina, dodge, parry, stagger, and damage rules.
- Action RPG abilities, inventory, quest progress, rewards, and leveling.
- Enemy AI patrol, chase, attack, and return behavior.
- Action game combo chains and lock-on targeting.
- Save/load profile and settings serialization.
- Beginner player controller and camera follow tutorial.
- Intermediate hitbox, hurtbox, and frame data tutorial.
- Advanced skill tree and progression unlock tutorial.
- Expert gameplay event bus and achievement tutorial.
- Advanced object pool, projectile reuse, and enemy spawn tutorial.
- Expert boss behavior tree and phase system tutorial.
- Master deterministic replay and command buffer tutorial.
- Master rollback networking and input prediction tutorial.
- Grandmaster ECS and data-oriented gameplay tutorial.
- Lightweight runtime self checks for shared systems.

## Suggested Next Steps

1. Add unit tests with a small C++ test framework.
2. Add Raylib or SDL visual demos after the console logic is stable.
3. Add more folders for animation events, loot tables, dialogue, rollback networking, and save-file versioning.
