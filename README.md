# C++ Game Programming Reference

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/build-CMake-informational)
![Visual Studio](https://img.shields.io/badge/IDE-Visual%20Studio-purple)
![Learning Project](https://img.shields.io/badge/purpose-gameplay%20learning-green)

A practical C++ reference project for learning common gameplay systems used in shooter games, soulslike games, action RPGs, and general real-time games.

The project uses standard C++17 console demos first. This keeps the gameplay logic easy to read, easy to debug, and easy to move later into Unreal Engine, Unity native plugins, Raylib, SDL, or a custom engine.

## Repository Tags

`cpp` `game-development` `gameplay-programming` `shooter` `soulslike` `action-rpg` `ai-state-machine` `combat-system` `save-system` `learning-project`

## Folder Guide

Every major folder name describes the game system inside it. This is intentional so beginners can browse the repository without guessing what each folder means.

| Folder | Content |
| --- | --- |
| `Common_Game_Code` | Shared gameplay helpers such as vectors, health, cooldown timers, and resource pools. |
| `01_Game_Loop_Input_Collision` | Fixed update style loop, scripted input, movement, and simple collision checks. |
| `02_Shooter_Weapon_Bullet_System` | Weapon fire rules, magazine ammo, reload timing, bullet travel, and target hits. |
| `03_Soulslike_Stamina_Dodge_Parry_Combat` | Stamina spending, dodge invincibility frames, parry windows, damage, and stagger state. |
| `04_Action_RPG_Ability_Inventory_Quest` | Abilities, mana costs, cooldowns, potions, inventory items, quests, rewards, and leveling. |
| `05_Enemy_AI_State_Machine_Patrol_Chase_Attack` | Enemy AI states for patrolling, seeing the player, chasing, attacking, and returning to post. |
| `06_Action_Game_Combo_LockOn_Targeting_System` | Combo attack chains, input windows, target scoring, and lock-on target selection. |
| `07_Game_Save_Load_Player_Profile_Settings` | Save/load logic for player profile data, checkpoint name, level, experience, and settings. |
| `99_Project_Self_Checks_For_Game_Systems` | Lightweight self checks for core shared gameplay systems. |

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
- The demos are deterministic, so the output is stable and easy to compare while learning.
- The code avoids engine-specific APIs so the gameplay ideas stay portable.
- The examples favor clarity over cleverness. Each system is small enough to read in one sitting.

## Current Gameplay Systems

- Core game loop, input, movement, and collision.
- Shooter weapon, ammo, reload, bullet, and hit detection logic.
- Soulslike stamina, dodge, parry, stagger, and damage rules.
- Action RPG abilities, inventory, quest progress, rewards, and leveling.
- Enemy AI patrol, chase, attack, and return behavior.
- Action game combo chains and lock-on targeting.
- Save/load profile and settings serialization.
- Lightweight runtime self checks for shared systems.

## Suggested Next Steps

1. Add unit tests with a small C++ test framework.
2. Add a Raylib or SDL visual version after the console logic is stable.
3. Add more folders for camera, animation events, hit boxes, skill trees, loot tables, and dialogue.
