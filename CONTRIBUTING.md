# Contributing

Thanks for improving this gameplay programming reference.

## Code Style

- Use C++17.
- Keep examples small and deterministic.
- Write code comments in English.
- Prefer clear gameplay logic over clever abstractions.
- Name folders after the exact game system they teach.

## Folder Naming

Use this pattern for new lesson folders:

```text
13_Level_Gameplay_System_Tutorial
```

Good examples:

- `13_Advanced_Loot_Table_Drop_Rarity_System`
- `14_Expert_Behavior_Tree_Boss_AI_Phases_Tutorial`
- `15_Intermediate_Dialogue_Choice_Branching_System`

Avoid vague names like:

- `NewCode`
- `GameplayStuff`
- `SystemDemo`

## Pull Request Checklist

- The new folder name explains the exact gameplay topic.
- The folder includes a short `README.md`.
- The demo is called from `Game_ProgrammingReference.cpp`.
- The source file is added to `CMakeLists.txt`.
- Visual Studio project files are updated if needed.
- The code builds without warnings in normal settings.
