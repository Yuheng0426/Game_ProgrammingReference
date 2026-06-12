# C++ Game Programming Reference

这个项目用于学习 C++ 游戏开发中的常见基础系统。每个大文件夹都是一个独立主题，文件夹名直接说明代码内容，方便以后放到 GitHub 给其他人阅读、运行和参考。

代码目前使用标准 C++17 和控制台模拟，不依赖 Unreal、Unity、SDL 或其他大型库。这样做的目的是先把玩法系统的核心逻辑讲清楚，例如游戏循环、碰撞、武器、子弹、耐力、翻滚、招架、技能、背包和任务。

## Folder Guide

- `Common_Game_Code`：可复用的基础代码，例如向量、生命值、冷却计时器、资源池。
- `01_Game_Loop_Input_Collision`：游戏循环、输入命令、移动、简单碰撞检测。
- `02_Shooter_Weapon_Bullet_System`：射击游戏的武器、弹药、换弹、子弹飞行和命中。
- `03_Soulslike_Stamina_Dodge_Parry_Combat`：魂类游戏常见的耐力、翻滚无敌帧、招架窗口、硬直。
- `04_Action_RPG_Ability_Inventory_Quest`：动作 RPG 常见的技能、蓝量、背包、药水、任务和升级。

## Build With CMake

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\Game_ProgrammingReference.exe
```

如果你使用 Visual Studio，也可以直接打开 `Game_ProgrammingReference.sln`，选择 `x64` 和 `Release` 或 `Debug` 后运行。

## Learning Notes

- All code comments are written in English so the repository is friendly for an international GitHub audience.
- The examples are deterministic. They run the same way every time, which makes debugging and learning easier.
- The code favors clear gameplay logic over engine-specific APIs. You can later move the same ideas into Unreal, Unity C++ plugins, SDL, Raylib, or a custom engine.

## Suggested Next Steps

1. Add simple unit tests for each gameplay system.
2. Add a Raylib or SDL visual version after the logic is stable.
3. Split each demo into smaller lessons if you want a tutorial-style GitHub repository.
