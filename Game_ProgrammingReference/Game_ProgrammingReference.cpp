#include <iostream>

#include "01_Game_Loop_Input_Collision/GameLoopDemo.h"
#include "02_Shooter_Weapon_Bullet_System/ShooterDemo.h"
#include "03_Soulslike_Stamina_Dodge_Parry_Combat/SoulslikeDemo.h"
#include "04_Action_RPG_Ability_Inventory_Quest/ActionRPGDemo.h"

int main()
{
    std::cout << "C++ Game Programming Reference\n";
    std::cout << "================================\n\n";

    game_ref::RunGameLoopDemo();
    game_ref::RunShooterDemo();
    game_ref::RunSoulslikeDemo();
    game_ref::RunActionRPGDemo();

    std::cout << "All reference demos completed.\n";
    return 0;
}
