# Tenetris — Development Guide / Handoff

A Tetris-variant built in Unreal Engine 5.7 (C++ module `Tenetris`).
Signature mechanics: **Space Inversion** (flip the board 180°) and **Time Inversion**
(rewind recent play). This document captures the architecture and current direction
so work can continue in any session.

---

## 1. High-level architecture

### MVC core (plain C++, not UObject)
The board simulation lives in a hand-rolled MVC under `Source/Tenetris/Module/MVC/`,
accessed through a singleton `FTNMVCHolder`:
- **Model** (`FTNFieldModel`, `FTNTestModel`, ...) — pure board simulation. No engine
  types, no audio, no rendering. Emits events via delegates.
- **View** (`FTNTestView`, ...) — pushes model state to the on-screen actors.
- **Controller** (`FTNTestController`, ...) — receives input, drives the model.

The model is deliberately engine-agnostic. **Keep gameplay rules and audio/visual
reactions OUT of the model.** The model only *notifies*; UObject layers react.

### Rendering (3D, not 2D UMG)
The board is drawn with **3D actors** (`ATNElectronicDisplayField`, mino actors via
`UTNTenetrisBufferComponent`), not a 2D UMG grid. This is intentional: the
Space-Inversion 180° flip animation needs real 3D rotation, which UMG can't do.
- `ATNElectronicDisplayField::Tick` drives the flip animation (RotationRemainingTime).
  If an actor is spawned without `FinishSpawning`, its Tick never registers and the
  flip animation silently breaks — see pitfalls below.

### UI (UMG with C++ base + WBP design)
Pattern used throughout: a **C++ `UUserWidget` base** holds logic and binds named
widgets via `meta=(BindWidget)`; a **WBP subclass** does the visual layout. Examples:
`UTNScoreHUDWidget`, `UTNTitleMenuWidget`.

### Two-layer delegate pattern (important)
Events cross layers via two distinct delegate tiers:
1. **Model → GameMode** (`DECLARE_DELEGATE`, in `TNFieldModel.h`): `FTNLinesCleared`,
   `FTNGameOver`, `FTNRewindStarted`, `FTNRewindEnded`. Pure gameplay events.
2. **GameMode → UI** (`DECLARE_DYNAMIC_MULTICAST_DELEGATE`, BlueprintAssignable):
   `FTNScoreChanged`, `FTNGameOverUI`. For HUD binding.

New cross-cutting features follow this: add a model delegate (shared, inert for modes
that don't subscribe), then have the relevant GameMode subscribe and react.

---

## 2. Game modes & flow

- `ATNDemoSingleGameMode` — the demo's single-player mode. Holds match rules that live
  OUTSIDE the model: score (100/300/500/800 × level), level = totalLines/10 + 1,
  game over on block-out. Creates the HUD, spawns the field, starts BGM.
  - Field start position is found by **actor tag** `PlayerFieldStart` (not injected),
    because the GameMode's StartPlay runs before level-BP injection would arrive.
  - `spawnField` uses `SpawnActorDeferred` + **`FinishSpawningActor`** (required!).
- `ATNTitleGameMode` — shows `UTNTitleMenuWidget`, sets UI-only input + cursor.
- `ATNTenetrisTestLocalMultipleGameMode` — test/multi mode (level-placed actors).

Flow: Title level → Play button `OpenLevel(DemoSingle)` → demo. Game-over panel has
Replay (`RestartMatch`) and Main Menu (`ReturnToTitle`, opens `TitleLevelName`).

Input: `ATNTenetrisPlayerController` (Enhanced Input, shared by all modes). On
`BeginPlay` it resets input mode to **GameAndUI** so a UI-only mode left over from the
title screen doesn't swallow gameplay input after level travel.

---

## 3. Audio system (data-driven, name-keyed)

Added so sounds can be assigned in the editor without code changes.

```
BP_TNGameInstance (UTNGameInstance)  -- holds -->  DA_TNAudio (UTNAudioData)
                                                      |  TMap<FName, USoundBase*>
UTNAudioSubsystem (GameInstanceSubsystem)  -- reads DA on Initialize
```

- **`UTNAudioData`** (`Source/Tenetris/Audio/`) — `TMap<FName, USoundBase*> Sounds`,
  `GetSound(key)`. Keys are **`TNSoundKeys`** constants (compile-time safety; avoids
  silent typo failures): `LineClear, HardDrop, SpaceInversion, TimeInversion, Bgm,
  GameOver, MenuClick`.
- **`UTNAudioSubsystem`** — `PlaySFX`, `PlayBGM` (via persistent `UAudioComponent`
  handle), `PauseBGM`/`ResumeBGM`/`StopBGM`, `PlayLoop`/`StopLoop` (separate channel
  for the rewind track). Missing key → logs a warning, no crash. No-op if no AudioData.
- **`UTNGameInstance`** (`Source/Tenetris/Core/`) — injection point (`AudioData` prop).

### Rewind audio behavior
Rewind is **variable length** (ends when the recorder empties, not a fixed 1s), so the
model exposes START/END delegates. Demo GameMode reacts:
- Start → `PauseBGM()` (preserves position) + `PlayLoop(TimeInversion)`.
- End → `StopLoop()` + `ResumeBGM()` (continues from the paused position).

Call sites: line-clear/game-over SFX + BGM in `ATNDemoSingleGameMode`; hard-drop /
space-inversion SFX in `ATNTenetrisPlayerController`.

### Audio = still needs editor setup (NOT done yet)
1. Create **`DA_TNAudio`** (UTNAudioData). Add map entries keyed EXACTLY:
   `LineClear, HardDrop, SpaceInversion, TimeInversion, Bgm` (+ optional
   `GameOver, MenuClick`).
2. Create **`BP_TNGameInstance`** (UTNGameInstance), assign `DA_TNAudio`.
3. Project Settings → Maps & Modes → **GameInstance Class = BP_TNGameInstance**.
4. Import 8-bit sounds. Mark `Bgm` and `TimeInversion` as **Looping**.

---

## 4. Current status

Done (committed):
- Flip-animation fix (`FinishSpawningActor` in demo + multi).
- Demo score HUD (C++ base + WBP), labels.
- Title menu (Play / How to Play / Quit) + How-To-Play panel toggle (MainMenuPanel
  hides while the panel shows).
- Return-to-title flow (game-over panel Main Menu button).
- Title→game input fix (GameAndUI reset).
- Dark background Plane (`M_BG`, unlit) replacing the sky sphere; ghost (Guide)
  material tuned for the dark backdrop.
- Data-driven audio system code (subsystem/data/game-instance + rewind delegates +
  wiring). **Latest commit `b36519a`.**

Editor-side assets the user maintains: `DemoSingle.umap`, `DemoTitle.umap`,
`Content/BluePrint/*` game-mode BPs, `WB_TNDemoScore`, `WB_TNDemoTitle`, `M_BG`.
Note: some BPs were moved to `Content/BluePrint/` leaving UE redirectors — run
"Fix Up Redirectors" when convenient.

---

## 5. Next steps / backlog

- **Audio editor setup** (section 3) — wire DA/GameInstance, import 8-bit sounds.
  Minimal set agreed: BGM + LineClear (+ SpaceInversion as the signature). Hard-drop
  and rewind are nice-to-have.
- Feed `Level` into the board fall speed (TODO in `handleLinesCleared`).
- Optional: menu-click SFX, game-over SFX, multi-mode audio (subscribe same delegates).
- Optional: reverse the ghost/background polish, unlit mino materials.

---

## 6. Pitfalls / gotchas (learned the hard way)

- **`SpawnActorDeferred` MUST be paired with `FinishSpawningActor`.** Otherwise the
  actor's Tick never registers and `BeginPlay` never fires — this silently broke the
  flip animation (the only thing driven purely by Tick).
- **`GetLevel` can't be a UFUNCTION override** — clashes with `AActor::GetLevel()`.
  The match-level getter is named `GetMatchLevel`.
- **UMG UserWidget root allows only ONE child.** Put a Canvas Panel as that single
  child, then add siblings inside it.
- **Input mode leaks across level travel.** Each screen should set its own input mode
  on entry (title = UI-only; game = GameAndUI).
- **UI (UMG) always draws over the 3D scene** — can't be used as a background behind
  the 3D board. Use a 3D Plane instead.
- **Auto Exposure** can wash out a dark scene — fix with a Post Process Volume
  (Exposure = Manual) if the screen looks blown out.
- Model is pure C++ (not UObject): reach audio/engine only from UObject layers
  (GameMode/PlayerController), never from the model.

---

## 7. Build & conventions

- Build: `& "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat"
  TenetrisEditor Win64 Development -project="<repo>\Tenetris.uproject" -waitmutex`
  (~60-70s; "License not activated" warning is harmless).
- Commit code and content assets; keep local junk (`.idea/`, `*.user`, `*.zip`) out.
- Commits use a `Co-authored-by: Copilot` trailer.
