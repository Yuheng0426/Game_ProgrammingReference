#include <iostream>

#include "01_Game_Loop_Input_Collision/GameLoopDemo.h"
#include "02_Shooter_Weapon_Bullet_System/ShooterDemo.h"
#include "03_Soulslike_Stamina_Dodge_Parry_Combat/SoulslikeDemo.h"
#include "04_Action_RPG_Ability_Inventory_Quest/ActionRPGDemo.h"
#include "05_Enemy_AI_State_Machine_Patrol_Chase_Attack/EnemyAIDemo.h"
#include "06_Action_Game_Combo_LockOn_Targeting_System/ComboLockOnDemo.h"
#include "07_Game_Save_Load_Player_Profile_Settings/SaveLoadDemo.h"
#include "08_Beginner_Player_Controller_Movement_Camera_Follow_Tutorial/BeginnerPlayerControllerDemo.h"
#include "09_Intermediate_Combat_Hitbox_Hurtbox_Frame_Data_Tutorial/IntermediateHitboxDemo.h"
#include "10_Advanced_Skill_Tree_Progression_Unlock_System/AdvancedSkillTreeDemo.h"
#include "11_Expert_Gameplay_Event_Bus_Achievement_System/ExpertEventBusDemo.h"
#include "12_Advanced_Object_Pool_Projectile_Enemy_Spawn_System/ObjectPoolSpawnDemo.h"
#include "99_Project_Self_Checks_For_Game_Systems/SelfCheckDemo.h"

int main()
{
    std::cout << "C++ Game Programming Reference\n";
    std::cout << "================================\n\n";

    game_ref::RunGameLoopDemo();
    game_ref::RunShooterDemo();
    game_ref::RunSoulslikeDemo();
    game_ref::RunActionRPGDemo();
    game_ref::RunEnemyAIDemo();
    game_ref::RunComboLockOnDemo();
    game_ref::RunSaveLoadDemo();
    game_ref::RunBeginnerPlayerControllerDemo();
    game_ref::RunIntermediateHitboxDemo();
    game_ref::RunAdvancedSkillTreeDemo();
    game_ref::RunExpertEventBusDemo();
    game_ref::RunObjectPoolSpawnDemo();
    game_ref::RunSelfCheckDemo();

    std::cout << "All reference demos completed.\n";
    return 0;
}
