# Unreal Engine 5 Troubleshooting Guide

This guide lists common Unreal Engine 5 problems that appear during game development, why they happen, and how to fix them.

## 1. Moved Assets Become Missing References

Symptoms:

- Blueprints lose references after moving assets.
- Materials or meshes show missing links.
- Old folders seem empty but references still point there.

Why it happens:

- When assets are moved or renamed in Unreal Engine, UE can leave redirectors so unloaded packages can still find moved assets.

Fix:

1. Move and rename assets inside the UE Content Browser.
2. Right-click the affected folder.
3. Choose `Fix Up Redirectors in Folder`.
4. Save all modified assets.
5. Commit both moved assets and changed `.uasset` files.

Prevention:

- Do not move `.uasset` files in Windows Explorer.
- Create a project folder structure early.
- Fix redirectors before large commits.

## 2. Blueprint Works In Editor But Fails After Restart

Symptoms:

- Blueprint references reset.
- Variables point to `None`.
- Nodes compile before restart but fail later.

Common causes:

- Unsaved dependent assets.
- Broken redirectors.
- Renamed C++ classes without redirects.
- Circular Blueprint dependencies.

Fix:

1. Compile the Blueprint.
2. Save all assets.
3. Fix redirectors.
4. Restart the editor.
5. If C++ names changed, add Core Redirects or avoid renaming reflected types mid-project.

## 3. C++ Class Does Not Appear In Editor

Symptoms:

- New C++ class is missing from the editor.
- Blueprint cannot parent to the class.

Checklist:

- Class uses `UCLASS()` if it must be visible to UE reflection.
- Parent type is valid, such as `AActor`, `UActorComponent`, or `UObject`.
- Header includes generated header last, for example `MyClass.generated.h`.
- Project compiled successfully.
- Editor was restarted after major module changes.

## 4. Hot Reload Creates Strange Blueprint Bugs

Symptoms:

- Duplicate classes appear.
- Blueprint defaults reset.
- Editor behavior differs from full rebuild.

Fix:

1. Close the editor for major C++ changes.
2. Build from Visual Studio or Rider.
3. Reopen the project.

Prevention:

- Use Live Coding for small function body changes.
- Avoid relying on Hot Reload for reflected property, class, or inheritance changes.

## 5. Packaging Fails

Checklist:

- Check the first real error in the Output Log, not the final generic failure.
- Fix missing assets and redirectors.
- Make sure maps are included in packaging settings.
- Check plugin platform support.
- Build in `Development` before `Shipping`.

## 6. Performance Drops In Gameplay

First checks:

- Use `stat unit`.
- Use `stat game`.
- Use `stat fps`.
- Check Blueprint tick usage.
- Check too many dynamic lights or expensive materials.

Common fixes:

- Disable unnecessary `Tick`.
- Use timers or events instead of per-frame Blueprint logic.
- Use object pooling for frequent spawn/despawn objects.
- Profile before guessing.

## Official References

- Unreal Engine documentation: https://dev.epicgames.com/documentation/unreal-engine
- Asset Redirectors: https://dev.epicgames.com/documentation/unreal-engine/asset-redirectors-in-unreal-engine
- Core Redirects: https://dev.epicgames.com/documentation/unreal-engine/core-redirects-in-unreal-engine
