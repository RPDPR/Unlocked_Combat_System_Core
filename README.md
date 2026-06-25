
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
* Complete memory cleanup and safety checks implemented.
* Seamless state preservation across save game updates and load triggers.
* No access violations or crashes when passing expired or invalid data.

## Requirements
* `Gothic I Classic` or `Gothic II NoTR` game platform
* `Union` version 1.0m or newer

---

## Quick Setup (v2.4.0)

### 1. Download
Download the latest version from [latest release](https://github.com/RPDPR/Unlocked-Combat-System/releases/latest) page
that matches your game version (**Gothic 1** or **Gothic 2 NoTR**).

Extract the archive. Each one contains:
* `UCS_gXX.dll` — Core subsystem library.
* `UCS\` — UCS scripts.
* `UCS\Consts_gXX.d` — UCS constants and useful switchers.
* `UCS\FXLib\` — FX library.
* `UCS\FXLib\FXLib_Proc.d` — FX library processor.
* `UCS\OnDamage_gXX.d` — UCS hooks on each 'OnDamage' function breakpoint.
* `Gothic.src` — Script loading order.
* `Externals.d` — Parser definitions.
* `README.txt` — Full SDK documentation reference.

> <img width="91" height="118" alt="image" src="https://github.com/user-attachments/assets/b44ca3f3-803c-4f2a-85f7-580b90db9202" />
> <img width="91" height="117" alt="image" src="https://github.com/user-attachments/assets/d56678da-bd7f-4a9b-b7ac-4df1435e553b" />




### 2. Integration Methods

#### Method A: Rapid Injection
*Best for quickly integrating the UCS to a pre-compiled game or mod.*

1. Copy `UCS_gXX.dll`, `Gothic.src` and `UCS\` folder into `\System\Autorun` directory of your Gothic.
2. Start the game and check it out!

#### Method B: Project Integration (Recommended)
*Best for modders building their own project from scratch or re-compiling gothic or mods.*

##### Manually:

1. Copy the core dynamic library `UCS_gXX.dll` into your game's `\System\Autorun\` folder (preferably packed within a `.mod` or `.vdf` volume for a final release).
2. Extract `UCS\` folder from the downloaded archive into your project's root directory (`_Work\Data\Scripts\Content\`).
3. Open your main `Gothic.src` file. Append it with the `UCS\Gothic.src` contents from the downloaded archive in the order already specified there.

> <img width="460" height="388" alt="image" src="https://github.com/user-attachments/assets/486c3d32-757f-47a1-bee0-9cace13d54cc" />

4. Save file changes and go check it out to the game that all is made correctly!

##### Via Gothic Sourcer:
1. Copy `UCS_gXX.dll` into the `\System\Autorun` folder (preferably within a `.mod` or `.vdf` volume).
2. Open your project in **GothicSourcer**.
3. Copy `UCS\` folder structure and its contents to your project. It's best to create a similar `UCS\` folder in the root of your project for this purpose:
   * Right-click your folder and select **"New script file"**.
   * Select the script position in the `.src` file according to its position in the existing `Gothic.src` file from the downloaded archive.
4. **Register External Functions:**
   * In GothicSourcer, go to **Help** -> **Show external functions**.
   * Open the provided `Externals.d` from the archive, copy its contents, and append them to the other signatures.
5. **Compile** your project as usual.

---

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.


