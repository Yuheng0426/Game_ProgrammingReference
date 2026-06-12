# Start Here - Beginner To Expert Learning Roadmap

Welcome to the guided learning path for this C++ gameplay programming reference.

This repository is built so you can learn one system at a time. Start with movement and collision, then move into combat, AI, progression, saving, and event-driven architecture.

## How To Study

1. Build and run the program once.
2. Open the next numbered folder.
3. Read the folder README.
4. Read the `.h` file to see the public entry point.
5. Read the `.cpp` file from top to bottom.
6. Change one value and run again.
7. Write down what changed in the output.

## Stage 1 - Beginner

Study these first:

- `01_Game_Loop_Input_Collision`
- `08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial`

Goal: understand how a game updates every frame, reads input, moves a player, clamps positions, checks simple collision, and follows the player with a camera.

## Stage 2 - Core Gameplay

Study these next:

- `02_Shooter_Weapon_Bullet_System`
- `03_Soulslike_Stamina_Dodge_Parry_Combat`
- `04_Action_RPG_Ability_Inventory_Quest`

Goal: understand weapon rules, stamina rules, ability rules, inventory use, quest progress, and reward logic.

## Stage 3 - Intermediate

Study these after the core gameplay examples:

- `05_Enemy_AI_State_Machine_Patrol_Chase_Attack`
- `06_Action_Game_Combo_LockOn_Targeting_System`
- `09_Intermediate_Combat_Hitbox_Hurtbox_Frame_Data_Tutorial`

Goal: understand AI state changes, lock-on scoring, combo input windows, active frames, hitboxes, and hurtboxes.

## Stage 4 - Advanced

Study these when you are comfortable reading multiple systems:

- `07_Game_Save_Load_Player_Profile_Settings`
- `10_Advanced_Skill_Tree_Progression_Unlock_System`

Goal: understand how player data is serialized, loaded, validated, and connected to progression rules.

## Stage 5 - Expert

Study these last:

- `11_Expert_Gameplay_Event_Bus_Achievement_System`
- `99_Project_Self_Checks_For_Game_Systems`

Goal: understand how larger game systems can communicate through events, and how lightweight checks can catch broken assumptions early.

## Suggested Practice

- Add a new weapon to the shooter folder.
- Add a stamina cost to the combo tutorial.
- Add a new skill prerequisite to the skill tree.
- Add a new event type for quest progress.
- Add one self check for any new shared rule.
