
<img width="1672" height="940" alt="UCS_2 0_Banner1" src="https://github.com/user-attachments/assets/e02a5c27-fb0e-4426-ba12-361912167ea8" />

# UnlockedCombatSystem (UCS)

## About the plugin

**UnlockedCombatSystem** is a powerful C++ subsystem built on the [Union Framework](https://gitlab.com/union-framework). It integrates directly into the **ZenGin** engine to fully unlock the combat logic for `Gothic I Classic`, `Gothic II: Night of the Raven`, and massive mods like Legend of Ahssun and so on. 

The subsystem is designed specifically for `Daedalus scripters`, allowing you to bypass native engine limitations and control combat mechanics directly from your `.d` files.

## Key Features

### ⚔️ Damage Pipeline (Legacy v1.0 Core)
* Full access to all internal engine damage calculation stages.
* Custom interception breakpoints inside core combat routines.
* Minimally invasive impact on original game code structure.

### 🧪 Advanced FX Engine (New in v2.0)
* Dedicated built-in engine to safely process periodic (Loop) and instant effects.
* Perfect for custom mechanics like realistic poisons, burning, or magic debuffs.
* Automated state machine that fully handles effect lifecycles under the hood.

### 🕹️ Runtime FX Control (New in v2.0)
* Over 20+ new external functions (getters and setters) for your scripts.
* Change running effects on the fly based on specific script events.
* Hot-swap active PFX visuals, adjust tick intervals, or rewrite the whole FX chain.

### 🎯 Flawless NPC Targeting (New in v2.0)
* Instant access to exact, real-time memory pointers for attackers and victims.
* Uses direct `UCS_GetDamageSender` and `UCS_GetDamageReceiver` calls.
* Completely fixes v1.0 bugs where identical monsters (e.g., multiple Wolves) break contexts.

### 💾 Save-Safe Architecture
* Complete memory cleanup and safety checks implemented in C++.
* Seamless state preservation across save game updates and load triggers.
* No access violations or crashes when passing expired or invalid data.

## Requirements
* **Gothic I Classic** or **Gothic II NoTR**
* **Union** version 1.0m or newer

---

## Quick Setup (v2.0)

### 1. Download
Download the latest version from [latest release](https://github.com/RPDPR/Unlocked-Combat-System/releases/latest) page
that matches your game version (**Gothic 1** or **Gothic 2 NoTR**).

Extract the archive. Each one contains:
* `UCS_gXX.dll` — Core subsystem library.
* `UCS_Consts_gXX.d` & `UCS_OnDamage_gXX.d` — Baseline setup scripts.
* `README.txt` & `Externals.d` — Full SDK documentation reference and compiler definitions.

You can delete the rest of the files if you are sure that you will not need to modify another version of Gothic in the future.

> <img width="149" height="172" alt="image" src="https://github.com/user-attachments/assets/73a6e0f3-970b-47c3-96ab-d181dbafa36b" />
> <img width="150" height="170" alt="image" src="https://github.com/user-attachments/assets/80e37af8-c090-4aee-9fd8-923e2532f3d7" />




### 2. Integration Methods

#### Method A: Rapid Injection
*Best for quickly integrating the UCS to a pre-compiled game or mod.*

1. Copy `UCS_Consts_gXX.d`, `UCS_OnDamage_gXX.d` and `UCS_gXX.dll` into `\System\Autorun` folder of your Gothic directory.
2. Start the game and check it out!

#### Method B: Project Integration (Recommended)
*Best for modders building their own project from scratch or re-compiling gothic or mods.*

##### Manually:

1. Copy the core dynamic library `UCS_gXX.dll` into your game's `\System\Autorun\` folder (preferably packed within a `.mod` or `.vdf` volume for a final release).
2. In your project's directory (`_Work\Data\Scripts\Content\`), create a new folder named `UCS`.
3. Extract `UCS_Consts_gXX.d` and `UCS_OnDamage_gXX.d` from the downloaded archive into this newly created `UCS` folder.
4. Open your main `Gothic.src` file and register the scripts:
   Insert the constants path `UCS\UCS_Consts_gXX.d` and pipeline path `UCS\UCS_OnDamage_gXX.d`
   right before the `STORY\Startup.d` in exact order. **Constants path must be placed before the pipeline path!**

> <img width="217" height="77" alt="image" src="https://github.com/user-attachments/assets/34bcae76-67bd-48da-aaa7-09484be3cf7e" />

6. Save file changes and go check it out to the game that all is made correctly!

##### Via Gothic Sourcer:
1. Copy `UCS_gXX.dll` into the `\System\Autorun` folder (preferably within a `.mod` or `.vdf` volume).
2. Open your project in **GothicSourcer**.
3. Add `UCS_Consts_gXX.d` and `UCS_OnDamage_gXX.d` files to your project. It's best to create a `\UCS` folder in the root of your project for this purpose:
   * Right-click your folder and select **"New script file"**.
   * Select the script position in the `.src` file. **Script Order Matters,** incorrect positioning may cause compilation errors.
   * It's required to choose a position **after** Constants.d and Classes.d files. `UCS_Consts_gXX.d` must go earlier than `UCS_OnDamage_gXX.d`.
     The best place for both paths is right before the `STORY\Startup.d`.
5. **Register External Functions:**
   * In GothicSourcer, go to **Help** -> **Show external functions**.
   * Open the provided `Externals.d` from the archive, copy its contents, and append them to the compiler definitions.
6. **Compile** your project as usual.

---

## FX Prototype Initialization

UCS v2.0 introduces a native engine startup callback. Inside your script files, utilize `UCS_Init()` to define your blueprints. This is the best place to call prototype registrations:

```c
func void UCS_Init()
{
    // Automatically triggered by C++ core on game startup
    UCS_CreateFXProto(PoisonFXP, 10, DT_POISON, -1, "PFX_POISON", 0, 1000.0, 5, 500.0, vf);
}
```

---

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.


