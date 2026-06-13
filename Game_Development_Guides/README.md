# Game Development Guides

These guides are not engine-specific code samples. They explain practical production habits that help small game projects become easier to finish, debug, and publish.

## 1. Build A Vertical Slice First

A vertical slice is a tiny but complete version of the game experience.

For an action RPG, a good slice might include:

- One small level.
- One controllable player.
- One enemy type.
- One weapon or ability.
- One win condition.
- One save checkpoint.
- One basic UI screen.

Do not start with ten weapons, five maps, and a giant skill tree. First prove that the smallest version is fun and stable.

## 2. Separate Prototype Code From Production Code

Prototype code answers "is this fun?"

Production code answers "can we keep building on this?"

A useful rule:

- Prototype fast when testing a new mechanic.
- Rewrite cleanly when the mechanic becomes part of the real game.
- Add comments when a system becomes teaching material or long-term project code.

## 3. Use Debug Output Early

Before adding UI and effects, print important state:

- Player health.
- Current weapon.
- Current AI state.
- Ability cooldown.
- Quest progress.
- Save checkpoint.
- Network prediction frame.

If a system cannot explain itself in debug output, it will be hard to tune later.

## 4. Keep Data Editable

Good gameplay systems separate rules from tuning data.

Examples:

- Weapon code should not hard-code every damage value.
- Loot code should read item weights from data.
- AI code should expose sight range, attack range, and cooldowns.
- Ability code should store mana cost and cooldown as data.

This project demonstrates that pattern in weapon configs, skill definitions, AI tuning values, and loot tables.

## 5. Test With Small Deterministic Scenarios

Randomness and real-time input make bugs difficult to reproduce. For learning and debugging, use deterministic inputs first.

Good examples:

- Scripted input frames.
- Fixed random seed.
- Command buffer replay.
- Self-check functions.
- Small test maps.

After the deterministic version works, add real input, UI, animation, and engine integration.

## 6. Common Beginner Production Mistakes

- Building a huge open world before one room is fun.
- Adding inventory UI before item rules are stable.
- Starting multiplayer before single-player simulation is deterministic.
- Creating many enemy types before one enemy has good feedback.
- Ignoring save/load until the end.
- Moving engine assets outside the editor and breaking references.

## Suggested Learning Order

1. Build gameplay logic in console C++.
2. Move one system into Unreal Engine 5 or Unity.
3. Add debug UI.
4. Add art and animation.
5. Add save/load.
6. Add profiling.
7. Add packaging and release checks.
