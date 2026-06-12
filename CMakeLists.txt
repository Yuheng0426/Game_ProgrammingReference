cmake_minimum_required(VERSION 3.16)

project(Game_ProgrammingReference LANGUAGES CXX)

add_executable(Game_ProgrammingReference
    Game_ProgrammingReference/Game_ProgrammingReference.cpp
    01_Game_Loop_Input_Collision/GameLoopDemo.cpp
    02_Shooter_Weapon_Bullet_System/ShooterDemo.cpp
    03_Soulslike_Stamina_Dodge_Parry_Combat/SoulslikeDemo.cpp
    04_Action_RPG_Ability_Inventory_Quest/ActionRPGDemo.cpp
    05_Enemy_AI_State_Machine_Patrol_Chase_Attack/EnemyAIDemo.cpp
    06_Action_Game_Combo_LockOn_Targeting_System/ComboLockOnDemo.cpp
    07_Game_Save_Load_Player_Profile_Settings/SaveLoadDemo.cpp
    99_Project_Self_Checks_For_Game_Systems/SelfCheckDemo.cpp
)

target_compile_features(Game_ProgrammingReference PRIVATE cxx_std_17)

target_include_directories(Game_ProgrammingReference PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(Game_ProgrammingReference PRIVATE /W4 /permissive-)
else()
    target_compile_options(Game_ProgrammingReference PRIVATE -Wall -Wextra -pedantic)
endif()
