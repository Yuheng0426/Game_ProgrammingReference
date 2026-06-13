# Unity Troubleshooting Guide

This guide lists common Unity problems that appear during game development, why they happen, and how to fix them.

## 1. Missing Script On GameObject

Symptoms:

- Inspector shows `Missing (Mono Script)`.
- Prefabs lose script references.
- Scene objects no longer run expected behavior.

Common causes:

- Script file was renamed without updating class name.
- `.meta` file was deleted or regenerated.
- Namespace or assembly definition changed.
- Compile errors prevent Unity from loading scripts.

Fix:

1. Fix all compile errors first.
2. Confirm the C# class name matches the file name for `MonoBehaviour` scripts.
3. Restore the original `.meta` file from Git if the GUID changed.
4. Reassign the script only if the old GUID cannot be restored.

Prevention:

- Commit `.meta` files.
- Move and rename assets inside Unity when possible.
- Avoid deleting `.meta` files manually.

## 2. Asset References Break After Moving Files

Why it happens:

- Unity identifies assets by GUID stored in `.meta` files. If the `.meta` file changes, references can break even if the visible filename is the same.

Fix:

1. Restore the original asset and `.meta` file from version control.
2. Let Unity reimport.
3. If needed, reassign references in prefabs and scenes.

Prevention:

- Use visible meta files.
- Keep `.meta` files in Git.
- Move assets through Unity's Project window.

## 3. Scripts Cannot See Other Scripts After Adding Assembly Definitions

Symptoms:

- `The type or namespace name could not be found`.
- Code worked before adding `.asmdef`.

Why it happens:

- Assembly Definition files split scripts into separate compiled assemblies. One assembly must explicitly reference another before it can use its types.

Fix:

1. Select the `.asmdef` file.
2. Add required references in the Inspector.
3. Keep runtime assemblies separate from editor-only assemblies.
4. Put editor scripts under an Editor assembly.

Prevention:

- Add asmdefs gradually.
- Start with `Game.Runtime`, `Game.Editor`, and `Game.Tests`.
- Avoid circular assembly dependencies.

## 4. Play Mode Is Slow To Enter

Common causes:

- Large script assemblies recompile often.
- Too many editor scripts run on domain reload.
- Asset database is constantly reimporting files.

Fix:

- Use Assembly Definitions to reduce recompilation scope.
- Avoid changing scripts while profiling play mode startup.
- Check Console for import errors.
- Profile editor scripts if custom tools are slow.

## 5. Build Works In Editor But Fails On Device

Checklist:

- Check platform-specific code inside `#if UNITY_ANDROID`, `#if UNITY_IOS`, etc.
- Verify scenes are added to Build Settings.
- Confirm required packages support the target platform.
- Check case-sensitive file paths.
- Test with a Development Build first.

## 6. Physics Or Gameplay Feels Different Between Machines

Prevention:

- Put physics gameplay in `FixedUpdate`.
- Avoid frame-rate-dependent movement.
- Use `Time.deltaTime` for frame updates.
- Use deterministic command buffers for tests.
- Do not depend on random values without a controlled seed when testing.

## Official References

- Unity Asset Metadata: https://docs.unity3d.com/Manual/AssetMetadata.html
- Unity Assembly Definitions: https://docs.unity3d.com/Manual/assembly-definition-files.html
- Unity Manual: https://docs.unity3d.com/Manual/
