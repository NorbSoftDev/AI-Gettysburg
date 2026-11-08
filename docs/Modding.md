# Scourge of War AI Modding Guide

This guide is for modders who want to change how the computer opponent fights in **Scourge of War: Waterloo** and **Scourge of War: Gettysburg**. It covers the AI DLLs, whose source code is in this repository, and the data files the AI reads.

You can change the AI in two ways:

- **Edit data files (no programming).** Many AI decisions come from CSV files in the game's `Logistics` folder: when brigades fall back, how close they get before charging, which formation they fight in, and how a division deploys. See [Tuning the AI without code](#3-tuning-the-ai-without-code).
- **Change the code (C++).** For anything else, change the source, build your own AI DLL and ship it in a mod. The [quick start](#2-quick-start-your-first-ai-mod) takes you through it step by step.

Both games share one engine and almost the same AI code, so this guide covers both. Where they differ, the text says **Waterloo:** or **Gettysburg:**. The [last section](#9-waterloo-and-gettysburg-differences) lists every difference.

## Contents

1. [How the AI works](#1-how-the-ai-works)
2. [Quick start: your first AI mod](#2-quick-start-your-first-ai-mod)
3. [Tuning the AI without code](#3-tuning-the-ai-without-code)
4. [What controls what in battle](#4-what-controls-what-in-battle)
5. [Programming reference](#5-programming-reference)
6. [Debugging](#6-debugging)
7. [Sharing your mod](#7-sharing-your-mod)
8. [Rules and pitfalls](#8-rules-and-pitfalls)
9. [Waterloo and Gettysburg differences](#9-waterloo-and-gettysburg-differences)

---

## 1. How the AI works

The game engine runs the battle: movement, firing, casualties and morale. The AI only decides what each computer-controlled unit and officer should do next. It gives those orders as commands, such as *fire*, *charge*, *fall back*, *form square* or *move here*. These are the same commands a player gives.

There are two AI DLLs, both in the game's `Modules` folder:

| DLL | What it does |
|---|---|
| `SowAiInf.dll` | **Battle AI.** Every regiment, battery, ammunition wagon, courier and officer on the battlefield. |
| `SowCampAI.dll` | **Campaign AI.** Moves the computer's armies on the campaign map and decides when to fight. |

### When the battle AI runs

The engine calls a function in the battle DLL for each unit, again and again, on a timer. Which function a unit gets is set by its type in `Logistics\unittype.csv`:

| Unit | Function | Called about every |
|---|---|---|
| Infantry regiment | `SowInfAIFunc` | 10 seconds |
| Cavalry regiment, mounted infantry | `SowCavAIFunc` | 10 seconds |
| Artillery battery | `SowArtAIFunc` | 10 seconds |
| Ammunition wagon | `SowSoldAmmoThink` | 10 seconds |
| Courier | `SowSoldCourThink` | 3 seconds |
| Infantry brigade commander | `SowUnitBrigThink` | 15 seconds |
| Artillery battalion commander | `SowArtyOffThink` | 15 seconds |
| Cavalry brigade commander | `SowCavOffThink` | 15 seconds |
| Division commander | `SowUnitDivThink` | 20 seconds |
| Corps commander | `SowUnitCorpThink` | 20 seconds |
| Army commander | `SowUnitArmyThink` | 25 seconds |
| Side (overall) commander | `SowUnitSideThink` | 30 seconds |

These are seconds of game time. Each unit's first call comes at a random moment in its first few seconds, so units don't all think at once. A unit whose morale changes is called straight away, without waiting for its timer.

Units the player controls also go through these functions. They get a much smaller set of decisions (`MyThink`), such as returning fire and auto-charging, so the AI doesn't take over the player's troops.

### What the AI cannot change

Some decisions are made inside the engine, and the DLL can only ask for them:

- **The game's simulation:** movement, pathfinding, firing, melee and casualties.
- **Officer routines:** many officer routines the DLL calls but cannot change, such as `OffThink`, `Orders`, `RunPlay`, `SupportUnits` and `CanICharge`.
- **Army-level grand tactics:** the attack, defend and meeting plays an army commander picks from.

---

## 2. Quick start: your first AI mod

This section takes you from nothing to a working AI mod. You will make the AI write a message to the game's log file, so you can see that your DLL is being used. You will also make AI infantry more willing to charge. It takes about 15 minutes, plus the time to install Visual Studio.

### 2.1 What you need

- **The game**, installed through Steam.
- **Visual Studio 2022 Community** (free) from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/). In the installer, tick the **Desktop development with C++** workload.
- **This repository.** Either clone it with git, or click the green **Code** button on GitHub and choose **Download ZIP**, then unzip it. A short path such as `C:\Mods\AI-Waterloo` is easiest to work with.

### 2.2 Find your game folder

Steam installs the game deep inside its own folder. To find it, right-click the game in your Steam **Library** and choose **Manage > Browse local files**. A File Explorer window opens in the game folder.

The default locations are:

| Game | Default game folder |
|---|---|
| Waterloo | `C:\Program Files (x86)\Steam\steamapps\common\Scourge Of War - Remastered` |
| Gettysburg | `C:\Program Files (x86)\Steam\steamapps\common\SOWGBx64` |

Steam lets you put your game library anywhere, so your path may be different. The part from `steamapps` onward stays the same.

The game data is in a folder called `Base` (Waterloo) or `BaseGB` (Gettysburg) inside the game folder. This guide writes `Base` for both. On Gettysburg, read it as `BaseGB`.

### 2.3 Create your mod folder

Mods live in the `Mods` folder inside `Base`. Each mod is a folder of its own. An AI DLL goes in a `Modules` folder inside the mod.

1. In the game folder, open `Base\Mods` (Gettysburg: `BaseGB\Mods`).
2. Create a new folder for your mod, for example `My AI Mod`.
   - Use **more than 4 characters**. The game ignores mod folders with shorter names.
   - **Don't use a dot (`.`)** in the name. The game ignores folder names with dots.
3. Inside your mod folder, create a folder called `Modules`.
4. Optional: create a text file called `EnglishModIntro.txt` in your mod folder. Write a sentence or two about your mod in it. The game shows this text when the mod is selected on the **Modifications** screen.

You now have:

```
Scourge Of War - Remastered\
  Base\
    Mods\
      My AI Mod\
        EnglishModIntro.txt
        Modules\
```

Never change or replace the game's own files in `Base\Modules` or `Base\Logistics`. Steam overwrites them when it updates or verifies the game, and other mods expect them to be the originals. Everything you change goes in your mod folder.

### 2.4 Make a change

Open the solution in Visual Studio: double-click `AI-Waterloo.sln` or `AI-Gettysburg.sln` in the repository folder.

**Change 1: log a message when your DLL loads.** Open the battle AI's main file: `SowAiInf.cpp` (Waterloo) or `SowGBAiInf.cpp` (Gettysburg). Add an include for `xunit.h` under the other includes near the top:

```cpp
#include "../sowmod/xunitdef.h"
#include "../sowmod/xtables.h"
#include "../sowmod/xunit.h"        // add this line
```

Then add one line to the `SowInit` function, after `theApp.m_sow = *sow;`:

```cpp
int SowInit( void *funcs )
{
	SSowModFuncs *sow = (SSowModFuncs *)funcs;

	theApp.m_sow = *sow;

	CXUtil::AddLog( "My AI mod is loaded" );   // add this line

	return 1;
}
```

The engine calls `SowInit` every time it loads a battle, sandbox or campaign map. `CXUtil::AddLog` writes a line to the game's log file. Call it only after the `theApp.m_sow = *sow;` line, because that line is what connects your DLL to the engine.

**Change 2: make infantry charge at lower odds.** Open `offcmds.cpp` and find this line near the top:

```cpp
#define INFLINEATTACK 3
```

Change the `3` to `2`:

```cpp
#define INFLINEATTACK 2
```

A firing AI infantry line charges when the [odds check](#42-the-odds-check-fearcheck) says it is at least `INFLINEATTACK` times stronger than the enemy infantry in front of it. The odds are rounded to a whole number, so `3` means about 2.5 to 1 or better, and `2` means about 1.5 to 1 or better. AI lines therefore charge sooner and more often.

### 2.5 Build

1. In the Visual Studio toolbar, set the configuration to **Release** and the platform to **x64**.
2. Choose **Build > Build Solution** (or press **Ctrl+Shift+B**).
3. The Output window should end with `Build: 2 succeeded, 0 failed`.

The DLLs are in the repository's `bin\Release` folder:

| Game | Battle AI | Campaign AI |
|---|---|---|
| Waterloo | `bin\Release\SowAiInf.dll` | `bin\Release\SowCampAI.dll` |
| Gettysburg | `bin\Release\SowAIInf.dll` | `bin\Release\SowCampAI.dll` |

The build never writes into the game folder, so it can't damage your installation.

### 2.6 Put the DLL in your mod

Copy the battle AI DLL from `bin\Release` into your mod's `Modules` folder:

```
Base\Mods\My AI Mod\Modules\SowAiInf.dll
```

Copy only the DLLs you changed. You didn't change the campaign AI, so leave `SowCampAI.dll` out, and the game keeps using its own.

**Optional: copy automatically after every build.** Set a Windows environment variable called `SOW_MOD_DIR` to your mod folder, and every build copies both DLLs into its `Modules` folder for you.

1. In the Windows search box, type **environment variables** and open **Edit environment variables for your account**.
2. Under **User variables**, click **New**.
3. Enter the name `SOW_MOD_DIR`. Enter your mod folder as the value, for example `...\steamapps\common\Scourge Of War - Remastered\Base\Mods\My AI Mod`.
4. Click **OK**, then restart Visual Studio so it picks up the variable.

### 2.7 Turn the mod on

1. Start the game.
2. On the main menu, click **Modifications**.
3. Click the check button next to your mod's name.
4. **Quit and restart the game.** The game loads the AI DLL when a battle loads and keeps it in memory until you quit, so a new or changed DLL is only picked up after a restart.

Only keep **one AI mod** turned on at a time. If two active mods both contain an AI DLL, only one of them is used.

### 2.8 Check that it worked

Start a **new** battle; the DLL is only loaded when a battle loads. Use a new battle rather than a saved game. A saved game of one of the shipped historical scenarios never uses mods that change gameplay, such as AI DLLs; only mods that don't affect gameplay are loaded. Then quit and open the game's log file. It is in your **Documents** folder:

| Game | Log file |
|---|---|
| Waterloo | `Documents\SOWx64\SOWx64.log` |
| Gettysburg | `Documents\SowGBx64\SowGBx64.log` |

Search it for `My AI mod is loaded`. If the line is there, the game is running your DLL. It appears once for every battle you load. In battle, AI infantry lines should now charge sooner than before.

If the line isn't there:

- Search the log for `Could not find ai dll` or `Could not load ai dll`. These mean the game couldn't find or open the DLL. Check the folder names in [2.3](#23-create-your-mod-folder) and that the mod is checked on the **Modifications** screen.
- Make sure you built **Release**, not Debug, and copied the DLL from `bin\Release`.
- Make sure you restarted the game after turning the mod on and after copying a new DLL.

### 2.9 Turning the mod off

Click its check button again on the **Modifications** screen and restart the game, or delete the mod folder. The game goes back to its own AI. Nothing outside your mod folder was changed.

---

## 3. Tuning the AI without code

You can change a lot of AI behaviour by editing data files in the game's `Base\Logistics` folder. The engine loads them, and the AI reads the values. No programming or rebuilding is needed, and data changes work with the game's own AI DLL or with yours.

### 3.1 How to change a data file in a mod

Don't edit the game's own files. Instead:

1. Create a `Logistics` folder inside your mod folder: `Base\Mods\My AI Mod\Logistics`.
2. Copy the file you want to change from `Base\Logistics` into it.
3. Edit your copy in a spreadsheet program or a text editor, and save it as CSV.

For the files in this section you can delete every row you didn't change. The game reads its own file first, and then your mod's file. A row in your file replaces the game's row with the same key, and a row with a new key is added. The key is:

| File | Key column |
|---|---|
| `orders.csv` | `Order` (the order name) |
| `stance.csv` | `MaxVal` |
| `playbook.csv` | `Value` (replaces all 5 rows of that strategy) |
| `unittype.csv` | `TypeName` |
| `unitattributes.csv` | the ability name on each `NEW` row (replaces the whole block) |

Some rules hold in all of these files:

- **Keep the header row.**
- **Comments:** a row whose first cell is empty is a comment and is skipped. The game's files use such rows at the top to explain every column.
- **Complete rows:** a row must have every column, even the ones you didn't change.

**Restart the game** after changing data files.

### 3.2 orders.csv: the orders an officer can be given

Every officer has an order: **Stay, Hold, Defend, Probe, Attack** or **Blitz**. The order decides three things:

- **Stances:** which stance the brigade uses while moving, after arriving, and while fighting (`MoveStance`, `ArriveStance`, `FightStance`).
- **Division plays:** whether the division follows a playbook strategy (`DivPlay`).
- **Freedom to act:** whether the brigade may wheel or change formation, and whether it keeps its order no matter what (`RegWheel`, `BrigWheel`, `KeepForm`, `KeepOrders`).

AI corps commanders pick the order whose `LeaderStyle` is closest to the leader's style points. `DivMod` pushes the division toward defensive (negative) or aggressive (positive) playbook strategies. A negative `DivMod` also stops artillery moving to high ground.

The **first row** is the default order for an officer with none, so keep `Stay` first. `KeepStance` set to 0 lets the game shift the stance value by up to 100 either way at random, plus the commander's *Stance Modifiers* from `unitattributes.csv` (see [3.6](#36-unitattributescsv-ai-settings-per-unit-ability)). Set it to 1 to use the value exactly.

**Example:** to make brigades under Attack orders fight more cautiously, lower Attack's `FightStance` from 1150 to 1000. When they fight, they then mostly use Probe stance rows instead of Attack rows. The random shift can still land anywhere from about 900 to 1100.

### 3.3 stance.csv: how a brigade behaves

A stance is a full set of behaviour settings: how close to stay to the enemy, at what morale to fall back or charge, when to form square, when to send out skirmishers, which formation to fight in, and which moves to make toward the enemy.

**How a row is picked:** the order gives a stance value, for example 400 for Defend. The game uses the first row whose `MaxVal` is at least that value, after the random shift and the commander's stance modifier (unless the order has `KeepStance`=1). The rows come in five groups:

| MaxVal | Group |
|---|---|
| 50–250 | Hold |
| 300–500 | Defend |
| 550–750 | Skirmish |
| 800–1000 | Probe |
| 1050–1250 | Attack |

The settings that matter most in play:

| Column | Effect in battle |
|---|---|
| `MoveMult`, `StayMult` | How close the brigade stays to the enemy, as tenths of its weapons' maximum range. Lower means it closes in. |
| `Fallback`, `Retreat` | A unit falls back or retreats when its morale drops to or below this level. Higher means it gives ground sooner. |
| `ChargeMelee`, `ChargeNoAmmo` | Morale needed to charge, and to charge when out of ammunition. |
| `InfantryCharge` | Distance in yards at which an infantry charge may start. |
| `Volley`, `MaintainLine`, `MoveForward`, `Flanking` | Morale needed to fire volleys, hold the line, advance, and move around a flank. |
| `SquareMen`, `SquareToEnemy`, `CavDanger` | Squares: minimum men, distance from cavalry at which to form and break square, and morale check (**Waterloo only**). |
| `CavCharge`, `CavChargDist`, `CavRunDist`, `CavPowerFactor`, `SquareChargeFactor` | When AI cavalry charges, and how much it fears squares. |
| `SkirmAttitude`, `SkirmNumber`, `SkirmDimension`, `SkirmSplit` | Whether companies split off as skirmishers, how many and how big, and the morale needed. |
| `ArtyFallback` | How close the enemy can get before unsupported artillery pulls back. |
| `BrigForm` | The formation the brigade fights in, for example `Fight` or `Assault` (attack column). |
| `C1`–`C5` command columns | The moves the brigade makes toward the enemy at each range band, such as `Move` to maximum range, then `Advance` to typical range. A blank command ends the list. |

The notes at the top of `stance.csv` explain every column.

Morale is a small number. 0 is routed and 1 is broken; higher is steadier. The thresholds in this file run from 0 to about 11.

### 3.4 playbook.csv: how a division deploys

When a division runs a strategy, the playbook says where each brigade, cavalry brigade and battery goes and which order it gets. The places are named spots around the division commander, for example `Mid`, `FarMid`, `Back`, or `High` (high ground) for artillery. The division picks a strategy from its commander's style points, with some randomness, and the order's `DivMod` added. Low values are defensive strategies and high values are attacks.

Each strategy is 5 rows with the same `Value`. To replace a strategy, repeat its `Value` in your mod file with all 5 rows. To add one, use a new `Value`.

The notes at the top of the file include a map of all the spots. Two rules:

- Every `Orders` cell must be an order name from `orders.csv`.
- Cavalry and artillery must not use the infantry spots.

### 3.5 unittype.csv: AI settings per unit type

Each row is a unit type, such as `TN_INFANTRY`, `TN_CAVALRY` or `TN_INFBRIG`. The columns the AI uses:

| Column | Effect |
|---|---|
| `AiDll`, `AiFunc` | Which DLL and function run this unit type. See [Replacing the AI for one unit type](#57-replacing-the-ai-for-one-unit-type). |
| `InfWeight`, `CavWeight`, `ArtyWeight` | How much this unit type fears each kind of enemy when choosing which way to face. These are weights in the row of the unit doing the looking. For example, the infantry row's `CavWeight` sets how much infantry fears cavalry. |
| `TicsReaquire` | Seconds a firing unit keeps its target before looking for a better one. Artillery and infantry use it, and so does Gettysburg cavalry. |
| `MarchTargYds` | Distance in yards at which a marching unit reacts to an enemy. |
| `CanCharge` | **Both games:** with 1, mounted cavalry that is attacked in melee fights back; with 0 it retreats. **Waterloo:** 1 also lets raiding cavalry charge its target. **Gettysburg:** also a switch between two versions of the cavalry charge code. 1 (shipped) keeps the original Gettysburg behaviour, and 0 turns on the newer charge code in `cavai.cpp` (`ChargeAI`). |
| `BaseVolleyYards` | Distance in yards at which a held volley is released. The experience modifier from `unitattributes.csv` (`VolleyYds`) is added to it. |

### 3.6 unitattributes.csv: AI settings per unit ability

This file is a set of blocks, one per ability, such as Experience, Morale, Fatigue, Style, Leadership and the proficiencies. Each block starts with a `NEW` row and lists levels, and every level has a value for each modifier column. A unit's modifier is the **sum** of the values for its level in every block. For example, its fallback chance is its Experience value, plus its Fatigue value, plus its Morale value.

The modifiers the AI uses, and which blocks set them in the shipped files:

| Modifier | Set by (shipped files) | Effect |
|---|---|---|
| Fallback Chance, Retreat Chance | Experience, Fatigue, Morale | Percent chance that a unit falls back, or retreats, each time its morale drops while it is under threat. |
| Stance Modifiers | Style (of the commander) | Shifts the brigade's stance value, so a bold commander uses a more aggressive stance. The engine applies it, not the DLL. |
| VolleyYds | Experience, Fatigue, Morale, Firearm Proficiency | Added to `BaseVolleyYards`. Negative values make a unit hold its volley until the enemy is closer. |
| MarchUnderFire | | How far a unit keeps marching in formation while under fire. |
| Wheeling Locked | | The distance within which units stop wheeling. |
| Max ammo | Style | How much ammunition a wagon hands out. |

To change a block in a mod, repeat its whole `NEW` block, with all its levels. The notes at the top of the file explain each column.

---

## 4. What controls what in battle

This section explains the main AI decisions in plain terms and says where each one is in the code or data. The constants named here are `#define` lines near the top of the file listed.

**Watch for duplicate constants.** Several constants are defined in more than one file, sometimes with different values. For example, `INFLINEATTACK` is in both `unitai.cpp` and `offcmds.cpp`, and the two copies are used for different things. Search the whole solution before you change one (**Edit > Find and Replace > Find in Files**).

### 4.1 Morale: routing, retreating and falling back

Every time an infantry regiment thinks, it checks morale first (`MoraleAI` in `unitai.cpp`):

1. **Morale 0:** the regiment routs.
2. **The engine's rout check fails:** the regiment routs.
3. **Morale 1 (broken), with an enemy in sight or while under fire:** it retreats.
4. **Morale has dropped since the last check, with an enemy in sight or while under fire:** it rolls a random number from 0 to 100.
   - If the roll is within *Fallback Chance* (`unitattributes.csv`), it **falls back**.
   - Otherwise, if the roll is within *Retreat Chance*, it **retreats**.
   - A square or column retreats instead of falling back.
   - Skirmisher companies run back to their parent battalion instead.

Separately, the brigade's stance (`Fallback` and `Retreat` in `stance.csv`) makes a unit fall back or retreat when its morale drops to those levels or below.

To make the AI hold longer, lower *Fallback Chance* and *Retreat Chance*, or lower the stance `Fallback` and `Retreat` values.

### 4.2 The odds check (FearCheck)

Before a charge, and when a charge comes in, the AI works out the odds with `FearCheck` in `offcmds.cpp`. It compares the attacker's strength with the defender's and returns a ratio, where 3 means the attacker is about three times stronger.

Each side's strength is built from:

- **Men:** number of men ÷ 100. Cavalry counts its men **10 times** (`CAVWEIGHT`).
- **Movement:** each side adds 1, or 4 if it is cavalry that is galloping or charging. The attacker adds 1 more if it is fresh enough to run.
- **Condition:** morale, plus unit and officer morale bonuses.
- **Fire:** minus a penalty for being under fire.
- **Terrain:** high ground adds 4 for the defender and 2 for the attacker, and defensive terrain adds 2 for the defender.
- **Flanks:** minus a penalty for being flanked.
- **Leader:** the brigade commander's personality.
- **Volley:** a defender holding a loaded volley gets a bonus, and the attacker loses the same amount.

The result is then adjusted:

- **Weak units:** skirmishers and artillery count only one fifth.
- **Defender already busy:** a defender already in melee or falling back counts only one fifth.
- **Angle of attack:** an attack on the flank is doubled, and an attack from behind is tripled. Squares have no flank.

The ratio is rounded to the nearest whole number, then compared with these constants:

| Constant | File | Default | Used for |
|---|---|---|---|
| `INFLINEATTACK` | `offcmds.cpp` | 3 | An infantry line charges enemy infantry from its firing position at these odds or better. It never charges cavalry this way. |
| `INFCOLUMNATTACK` | `offcmds.cpp` | 2 | An attack column charges at these odds or better. |
| `INFLINEADVANCE` | `offcmds.cpp` | 1 | Above these odds, the line advances while firing. |
| `BAYONETBREAK` | `unitai.cpp` | 3 | A unit charged at these odds or worse runs instead of standing. |
| `SABERBREAK` | `cavai.cpp` | 3 | The same for cavalry being charged by cavalry (**Waterloo only**). |

An infantry charge also needs the stance's `ChargeMelee` morale, must start within the stance's `InfantryCharge` distance, and is never made from square or against a fort.

### 4.3 Infantry

All in `unitai.cpp` and `offcmds.cpp` (`GetInfCommand`).

- **Choosing a target and facing:** `CheckAllTargets` adds up the threat in each direction around the regiment. Each enemy counts its men multiplied by the `unittype.csv` weights, and artillery counts much more. Enemies in front, within 45°, get priority. The regiment only wheels toward another direction if the threat there is bigger by more than `AI_GREATERTHREAT` (2000).
- **Volleys:** a line holds a loaded volley until the enemy is within `BaseVolleyYards` + `VolleyYds` (see [3.5](#35-unittypecsv-ai-settings-per-unit-type) and [3.6](#36-unitattributescsv-ai-settings-per-unit-ability)). The stance's `Volley` morale is needed to fire volleys at all.
- **Charging:** see [4.2](#42-the-odds-check-fearcheck).
- **Advancing and holding the line:** these are set by the stance's command list (`C1`–`C5`), `MoveForward`, `MaintainLine` and `MoveMult`/`StayMult`.
- **Deploying from march column:** a marching column deploys when an enemy comes within 1.1 × its maximum range (`COL_CHECK` in both games). Gettysburg also defines `COLUMN_CHECK`, but nothing uses it.
- **Skirmishers:** only battalions of **more than 450 men** can split off skirmisher companies (`SplitCompany` in `offcmds.cpp`). How many companies, and how big, comes from the stance's `Skirm*` columns.
- **Squares (Waterloo only):** `CavalryDangerCheck` forms square when enemy cavalry is within `CAVALRY_DANGER_DISTANCE` (250 yards) in front, or half that to the flanks and rear. The stance's `SquareMen`, `SquareToEnemy` and `CavDanger` decide whether and when. `SQUARE_MEN_MINIMAL` only applies to regiments under a player-controlled officer.
- **Forts (Waterloo only):** the stance's `FortRange`, `MoraleBesieged` and `LeaveFort` columns control occupying and leaving forts.

### 4.4 Cavalry

In `cavai.cpp` and `offcmds.cpp` (`GetCavCommand`).

- **Charging:** cavalry trots, gallops and charges based on the stance's `CavChargDist`, `CavRunDist`, `CavCharge` and `CavPowerFactor`.
- **Squares:** cavalry never charges a formed square that is standing still. It only charges a square that is marching or running, that is within `DISTCAV1AUTOMELEE`/2 + `CavPowerFactor` yards, and only if its men × morale beat the square's men × morale × `SquareChargeFactor`.
- **Close enemies:**
  - **Waterloo:** enemy cavalry and artillery within `DISTCAV1AUTOMELEE` (100 yards) are added to the cavalry's targets.
  - **Gettysburg:** with `CanCharge` = 0, cavalry charges non-artillery enemies within that distance.
- **Screens and guards:** cavalry on a flank screen or guarding a battery follows its assignment from the playbook.
- **Gettysburg:** dismounted cavalry remounts after a while. See [section 9](#9-waterloo-and-gettysburg-differences).

### 4.5 Artillery

In `artyai.cpp` and `offcmds.cpp` (`GetArtCommand`).

- **Pulling back:**
  - Guns that are limbered, or still unlimbering, retreat when an enemy comes within `AI_ARTYRAD` (125 yards) and the battery has no morale bonus from nearby units.
  - Deployed guns limber and pull back when an enemy is inside the stance's `ArtyFallback` distance and either they have no morale bonus, or no friendly infantry or cavalry is nearer to them than the enemy.
- **Targeting:**
  - It takes the closest target in front of it, unless an enemy in any direction is within `CLOSESUPPORT` (250 yards).
  - It wheels toward another direction only if the threat there is bigger by more than `ARTGREATERTHREAT` (4000).
  - It looks for a better target every `TicsReaquire` seconds (`unittype.csv`).
- **Ammunition:** with no target it switches from canister back to solid shot, shell or shrapnel.

### 4.6 Officers

In `offai.cpp`.

- **Brigade commanders (`CUnitBrigThink`):**
  - The brigade switches to fighting orders when an enemy comes within `STANCE_DANGER_DISTANCE` (150 yards).
  - It leaves the road within twice that distance.
  - It can wheel once every 2.5 minutes (`TICKS_WHEEL`).
  - The commander sends supports to regiments in trouble.
- **Artillery commanders (`CArtyOffThink`):** move batteries to the best ground and halt within `DISTARTYHOLD` (200 yards) of the enemy.
- **Ammunition wagons (`CSoldAmmoThink`):** drive to units that need ammunition, and flee from enemies within `DISTOFFSCARED` (80 yards).
- **Division commanders (`CUnitDivThink`):**
  - They send orders by courier at most every 2.5 minutes.
  - They ask for help at most every 5 minutes.
  - They run the playbook strategies from `playbook.csv`.
- **Campaign battles:** the top AI commander gives up the battle when losses pass `RETREATPERCENTAGE` (21%) plus his personality (`CampaignRetreatCalculation`).

### 4.7 Campaign AI

In `campai.cpp`, in the campaign DLL. The engine calls it when:

- the campaign starts, or a force splits
- every 30 minutes of campaign time, for each computer force
- a force arrives at its destination
- a force sights the enemy, loses sight of it, or finishes a battle

The campaign AI decides:

- **Attacker or defender:** it compares the commander's average personality with 4, then applies the objective type. Single-town and destroy objectives mean attack, major-town objectives mean defend, and an army protects its base town.
- **Where to go next:** target towns, picket positions, and chasing weaker enemies within 15,000 yards.
- **Fight or retreat when the enemy is sighted (`AttackEnemy`):** it compares divisions, cavalry and artillery on each side. After a retreat it waits 4 hours (`RETREATDELAY`) before moving again.

---

## 5. Programming reference

### 5.1 The files

| File | Contents |
|---|---|
| `SowAiInf.cpp` (Gettysburg: `SowGBAiInf.cpp`) | `SowInit`, called once when the DLL loads. |
| `unitai.cpp` | Infantry regiments: morale, targets, facing, squares, forts, the player-controlled `MyThink`. |
| `cavai.cpp` | Cavalry regiments. |
| `artyai.cpp` | Artillery batteries. |
| `offcmds.cpp` | `FearCheck` and the stance-driven commands for infantry, cavalry and artillery (`GetInfCommand`, `GetCavCommand`, `GetArtCommand`). |
| `offai.cpp` | Officers, ammunition wagons and couriers. |
| `campai.cpp` | Campaign AI (campaign DLL). |
| `SOWWLMOD.cpp` | Old logging helpers. They are switched off, so use `CXUtil::AddLog` instead. |
| `SowMod\` | The interface to the engine. **Don't edit**; see [section 8](#8-rules-and-pitfalls). |

### 5.2 Entry points

Each battle AI function has the form:

```cpp
int SowInfAIFunc( int ticks, void *punit )
{
	CXUnit unit( (CUnit *)punit );
	...
}
```

Wrap the pointer in the matching class and work through it: `CXUnit` for regiments, batteries and wagons, `CXOff` for officers, and `CXCour` for couriers. The campaign DLL exports `SowAIFunc( int state, void *force, int camptype )`. `state` is the event (`eSBCInit`, `eSBCThink`, `eSBCArrive`, `eSBCEnemySeen` and so on). For `eSBCEnemySeen` the function must return `eComSBCBattle` or `eComSBCRetreat`.

`SowInit` receives a table of engine functions and **must** copy it (`theApp.m_sow = *sow;`) before anything else calls into the engine. It runs every time a battle, sandbox or campaign map loads, so it may run many times in one session. Anything you set up there must be safe to do again.

### 5.3 The engine interface (SowMod)

`SowMod\xunit.h` wraps everything the AI can see and do. The main classes:

- **`CXUnit`** (a regiment or battery):
  - state: `NumMen()`, `Morale()`, `State()`, `Ammo()`, `UnderFire()`, `IsFlanked()`, `IsHighGrnd()`, `OnDefTerr()`
  - nearby units: `TargBeg()`/`TargNext()`, `EnemyBeg()`/`EnemyNext()`, `FriendBeg()`/`FriendNext()`, and the `TargClose`/`EnemyClose` shortcuts
  - data values: `GetMod(eGame...)` from `unitattributes.csv`, `GetUnitTypeVal(UVAL_...)` from `unittype.csv`
  - actions: `ICommand()`, `Command()`, `SetDest()`, `Limber()`, `ChooseAmmo()`
- **`CXOff`** (an officer; it also has all of `CXUnit`): stance and orders, subordinates, destinations (`GetLineDest`, `GetFlankDest`, …), `Personality()`, playbook control, `SendOrdersByCour()` and `TicksForOffSDK()`.
- **`CXUtil`** (general helpers): `Ran()` random numbers, `GetTime()`, `AddLog()`, `GetSold()`, `AddComm()`, `IsMulti()`.
- **`CXCamp`, `SXTown`:** campaign forces and towns.

Looping over enemies looks like this:

```cpp
CXUnit sold;
int    dist;

base.EnemyBeg();

while ( base.EnemyNext( sold, dist ) )
{
	if ( sold.IsOfficer() )
		continue;
	// look at sold
}
```

### 5.4 Giving commands

Commands are the `eCom...` values in `SowMod\xunitdef.h`, for example `eComFire`, `eComCharge`, `eComFallback`, `eComRetreat`, `eComHalt` or `eComFormType`. The simplest way to give one is `ICommand`:

```cpp
unit.ICommand( eComRetreat, 1 );
unit.ICommand( eComFormType, eFTAssault );   // form attack column
```

For a command with a destination or facing, fill an `SXComm` and call `Command( &comm )`.

`eComMax` means "no command". Its value changes when new commands are added to the engine, which is one reason to rebuild after every engine update.

### 5.5 Time and timers

- `CXUtil::GetTime()` returns the game clock in ticks. `TICSPERSEC` (360 ticks per second) and `TICSPERMIN` convert them, for example `5 * TICSPERMIN` is 5 minutes.
- Some counters in the code are named "ticks" but count AI passes, for example `COUR_RECALCTICKS`. They are commented in the code.
- Before engine 1.022, `TICSPERSEC` was wrongly 60. If you are updating an older mod, see the 1.022 release notes in the [README](../README.md).

### 5.6 Saving data with the game

Each officer has 30 integer slots that are saved in the saved game, for your AI's own use:

```cpp
int value = off.TicksForOffSDK( 5 );      // read slot 5
off.TicksForOffSDK( 5, value + 1 );       // write slot 5
```

Rules:

- Use it **only on officers** (`CXOff`).
- Passing `-1` as the value means "read", so you can't store -1.
- Slots 1 and 2 are already used by the shipped AI (`CampaignRetreatCalculation`).

### 5.7 Replacing the AI for one unit type

The DLL name and function name for each unit type come from `unittype.csv` (`AiDll` and `AiFunc`). A mod can ship a DLL with a different name, for example `MyCavalryAI.dll`, plus a `Logistics\unittype.csv` containing only the rows for the unit types it takes over. Every other unit type keeps using the game's AI.

A DLL used this way still needs to export `SowInit` and copy the function table, as in [5.2](#52-entry-points).

---

## 6. Debugging

**Log messages.** `CXUtil::AddLog( "text" )` writes a line to the log file listed in [2.8](#28-check-that-it-worked). It is the quickest way to see what your AI is deciding. Build the text with `sprintf_s` if you need numbers in it.

- **Watch for `%`.** The engine treats the text as a `printf` format, so a `%` in it produces garbage or a crash. Write `%%` for a percent sign, and never pass text you didn't write yourself, such as unit names, straight in.
- **Keep lines short:** under 2,000 characters.
- **Don't log every pass of every unit.** There are thousands of calls per minute, and the log gets huge.

**Stepping through code in Visual Studio:**

1. Build the **Debug** configuration. It produces the same DLL names in `bin\Debug`.
2. Copy that DLL into your mod's `Modules` folder.
3. Start the game from Steam.
4. In Visual Studio, choose **Debug > Attach to Process** and pick `sow64.exe` (Waterloo) or `sowgbx64.exe` (Gettysburg).
5. Set breakpoints in your AI code. They are hit when the game calls it.

The Debug DLL is slower and much larger. Always share a **Release** build.

**Turning the AI off.** In the game's INI file (`Documents\SOWx64\SOWx64.ini` or `Documents\SowGBx64\SowGBx64.ini`), setting `NoAI=1` in the `[Debug]` section stops the battle AI for every unit. The campaign AI still runs. This is useful to check whether a problem comes from the battle AI at all.

---

## 7. Sharing your mod

Share your whole mod folder:

```
My AI Mod\
  EnglishModIntro.txt
  Modules\
    SowAiInf.dll        (Gettysburg: SowAIInf.dll)
    SowCampAI.dll       (only if you changed the campaign AI)
  Logistics\
    stance.csv          (only if you changed data files; only the rows you changed)
```

- **Build:** ship a **Release** build.
- **Loose files:** ship the DLL as a loose file in the folder. A DLL inside a packed `.sow` archive is not supported.
- **Engine version:** say which game and engine version you built against. The README shows the latest. A DLL built against older `SowMod` headers may misbehave on a newer engine.
- **Tell players:** tell players to turn on only one AI mod at a time, and to restart the game after turning it on.
- **Steam Workshop:** Workshop items work like mods. The [SOWSteamWorkshop](https://github.com/NorbSoftDev/SOWSteamWorkshop) tool uploads a mod folder to the Workshop.
- **Source code:** this code is MIT licensed, so you may share your changed source too.

---

## 8. Rules and pitfalls

- **Don't edit the `SowMod` folder.** Its headers describe the engine's memory layout and function table. Change them, and your DLL no longer matches the engine, which usually means a crash. If you need something the interface doesn't offer, ask for it.
- **Rebuild after every engine update.** When the game updates, get the latest code from this repository and rebuild. The README's *Latest release* section lists any changes that affect AI mods.
- **Restart the game** after turning a mod on or off, and after copying in a new DLL or data file.
- **One AI mod at a time.**
- **Saved games of the shipped historical scenarios don't use AI mods.** Only mods that don't affect gameplay are loaded for them.
- **Don't put your build in the game's own `Base\Modules` folder.** Steam replaces it on updates, and players can't easily undo it. Use a mod folder.
- **Search for every copy of a constant** before changing it (see [section 4](#4-what-controls-what-in-battle)).
- **Keep the DLL names.** The game loads `SowAiInf.dll` and `SowCampAI.dll` by name, unless your mod's `unittype.csv` points at another name ([5.7](#57-replacing-the-ai-for-one-unit-type)). The campaign DLL is always `SowCampAI.dll`.
- **Use Visual Studio 2022 and the provided solution.** The projects use the static C runtime, so players don't need to install anything extra.

---

## 9. Waterloo and Gettysburg differences

| Area | Waterloo | Gettysburg |
|---|---|---|
| Folders | `SowAiInf`, `SowCampAI` | `SowGBAiInf`, `SowGBCampAI` |
| Battle DLL name | `SowAiInf.dll` | `SowAIInf.dll` (the same name; Windows ignores upper and lower case) |
| Data folder | `Base` | `BaseGB` |
| Squares and cavalry danger | Yes (`CavalryDangerCheck`) | No |
| Forts | Yes | No |
| Cavalry | Full charge, screen and raid logic | `unittype.csv` `CanCharge` picks the original (1, shipped) or newer (0) charge code; dismounted cavalry remounts after a while |
| Top two Attack stances (`stance.csv` 1200, 1250) | `BrigForm` = `Assault` (attack column) | `BrigForm` = `Fight` |
| Campaign retreat | Picks a retreat town by direction | Adds a fallback when no town fits: it avoids the enemy's direction and accepts unoccupied towns |
| Unused files | — | `tables.cpp`, `tables.h` and `tactics.h` are old and not part of the build |
