<img width="1195" height="672" alt="UCS_splash" src="https://github.com/user-attachments/assets/48814236-7a5a-4f3e-ac9a-205fa0194874" />

# UnlockedCombatSystem

## About the Plugin

**UnlockedCombatSystem** is a lightweight C++ plugin built on the [Union Framework](https://github.com). It fully unlocks the combat system and its operations, providing developers with complete access to every stage of the damage calculation logic.
The plugin is primarily designed for **Daedalus** scripters, allowing them to modify engine-level combat functions directly from your scripts.

**Key Features:**
- Fully unlocked combat mechanics
- Access to all breakpoints in the damage calculations
- Minimal impact on original game's engine functions
- Compatible with Gothic I Classic and Gothic II NoTR

## Requirements
* **Gothic I Classic** or **Gothic II NoTR**
* **Union** version 1.0m or newer

---

# Quick Setup

### 1. Download
Download the latest version from [latest release](https://github.com/RPDPR/Unlocked-Combat-System/releases/latest) page.

Extract the archive. You will need specific files depending on your game version:
* **For Gothic 1:** Keep `UCS_g1.dll` and `UCS_logic.d`.
* **For Gothic 2 NoTR:** Keep `UCS_g2a.dll` and `UCS_logic.d`.
* The `Externals.d` file is required for valdation in compilators like Gothic Sourcer.

You can delete the rest of the files if you are sure that you will not need to modify another version of Gothic in the future.

> <img width="167" height="155" alt="!1" src="https://github.com/user-attachments/assets/3154a062-1000-4ff6-9893-8f24714a3aeb" />

### 2. Integration Methods

#### Method A: Rapid Injection
*Best for quickly adding the plugin to a pre-compiled game or mod.*

1. Copy `UCS.dll` and `UCS_logic.dll` into `\System\Autorun` folder of your Gothic directory.

#### Method B: Project Integration (Recommended)
*Best for modders building their own project from scratch or re-compiling gothic or mods.*

1. Copy `UCS.dll` into the `\System\Autorun` folder (preferably within a `.mod` or `.vdf` volume).
2. Open your project in **GothicSourcer**.
3. Add `UCS_logic.d` to your project. It's best to create a `\Utils` or `\Utilities` folder in the root of your project for this purpose, although this isn't required. Experienced scripters can choose the best location for this script based on their project's architecture:
   * Right-click your folder and select **"New script file"**.
   * Name it `UCS_logic.d` or whatever you like.

> <img width="139" height="148" alt="!2" src="https://github.com/user-attachments/assets/a39d9717-3180-44d3-90d7-6e5b5cd83c0d" />

4. Select the script position in the `.src` file. **Script Order Matters,** incorrect positioning may cause compilation errors. It's recommended to choose a position **after** constants, classes, and AI functions, which already should be loaded by this time, but **before** any `\Story` scripts.

> <img width="220" height="269" alt="!3" src="https://github.com/user-attachments/assets/96f0cab8-17c9-4923-9915-1ead1e299b60" />

5. Open the file you just created, in the GothicSourcer editor, copy the contents of the `UCS_logic.d` file that is in the UCS plugin archive you downloaded.

6. **Register External Functions:**
   * In GothicSourcer, go to **Help** -> **Show external functions**.
   * Open the provided `Externals.d` from the UCS plugin archive you downloaded.
   * Copy the function signatures and paste them into the editor's `Externals.d` file.

> <img width="563" height="206" alt="!4" src="https://github.com/user-attachments/assets/44b57332-0a1c-4be0-a36c-362b109ea84c" />

7. **Compile:** Build your project as usual.

---

### 3. Done!
You are ready to go. Write your custom code, implement the best ideas inside `UCS_logic.d` and enjoy the results!

