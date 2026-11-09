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
`UTNScoreHUDWidget`, `UTNTitleMenuWidget`, `UTNOptionsWidget`, `UTNPauseMenuWidget`.

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

### Volume control
`UTNAudioSubsystem` owns `Master/BGM/SFX` volumes (0..1; final = Master × channel).
- Setters `SetMasterVolume/SetBGMVolume/SetSFXVolume` apply live to the BGM/Loop
  `UAudioComponent`s (`SetVolumeMultiplier`); SFX volume is passed to `PlaySound2D`.
- Persisted via `SaveVolumes()` / loaded in `Initialize` from `GGameUserSettingsIni`,
  section `Tenetris.Audio`.
- UI never holds volume logic: `UTNAudioSettingsWidget` only calls the setters/getters.
- `SetLoopSuspended(bool)` pauses only the rewind loop track while the game is paused;
  the main BGM keeps playing so volume changes stay audible in the pause menu.
  BGM pause state is tracked separately (`bBgmPaused`), so a rewind-paused BGM is never
  resumed by a pause/resume cycle.

### Audio = still needs editor setup (NOT done yet)
1. Create **`DA_TNAudio`** (UTNAudioData). Add map entries keyed EXACTLY:
   `LineClear, HardDrop, SpaceInversion, TimeInversion, Bgm` (+ optional
   `GameOver, MenuClick`). `MenuClick` is also the sample played when an options
   slider is released.
2. Create **`BP_TNGameInstance`** (UTNGameInstance), assign `DA_TNAudio`.
3. Project Settings → Maps & Modes → **GameInstance Class = BP_TNGameInstance**.
4. Import 8-bit sounds. Mark `Bgm` and `TimeInversion` as **Looping**.

---

## 3b. Pause & Options

Code (done):
- `ATNDemoSingleGameMode::TogglePause/SetGamePaused`: `bPaused` makes `Tick` skip the
  controller tick (same pattern as `bGameOver`). All model timers (fall, rewind,
  rotation) advance only via `deltaTime`, so skipping Tick freezes them safely — this
  includes pausing mid-rewind. `SetGamePaused` is NOT `UGameplayStatics::SetGamePaused`
  (that would also stall audio components). Blocked after game over.
- On pause: loop track suspended, cursor shown, `PauseMenuWidgetClass` created
  (new instance each time so it always opens on the main panel). `EndPlay` clears the
  loop suspension because the audio subsystem outlives the level.
- `ATNTenetrisPlayerController`: `PauseAction` (Enhanced Input) → `togglePause`.
  `SetGameplayInputBlocked(true)` ignores all gameplay input and first releases held
  left/right/soft-drop so the piece does not keep moving after resume.
- `UTNOptionsWidget` is the options container (reused by the title menu
  (`OptionsButton`/`OptionsPanel`, both optional) and the pause menu). It hosts two
  sections, `UTNAudioSettingsWidget` (3 volume sliders) and `UTNControlsSettingsWidget`
  (key bindings; skeleton only, rebinding not implemented yet), switched by optional
  tab buttons, plus an optional close button (`OnCloseRequested`).

Editor setup and play-test verification are tracked as TODO checklists in section 5
(Next steps / backlog → "Pause/Options TODO").

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
`Content/BluePrint/*` game-mode BPs, `WB_DemoScore`, `WB_DemoTitle` (renamed from `WB_TNDemoScore`/`WB_TNDemoTitle`), `WB_AudioSettings`, `WB_ControlSettings`, `WB_Options`, `WB_PauseMenu`, `M_BG`.
Note: some BPs were moved to `Content/BluePrint/` leaving UE redirectors — run
"Fix Up Redirectors" when convenient.

---

## 5. Next steps / backlog

- **Audio editor setup** (section 3) — wire DA/GameInstance, import 8-bit sounds.
  Minimal set agreed: BGM + LineClear (+ SpaceInversion as the signature). Hard-drop
  and rewind are nice-to-have.
- **Pause/Options TODO** (code done & compiled; design in section 3b). Editor setup:
  - [ ] `IA_Pause` (Input Action, Digital/bool); add to the mapping context: `Escape`,
    `P`, gamepad `Special Right` (Esc ends PIE in the editor → test with `P`).
  - [ ] Player controller BP: assign `PauseAction` = `IA_Pause` (unassigned = no pause
    + error log).
  - [x] `WB_AudioSettings` (parent `TNAudioSettingsWidget`): required `MasterSlider`,
    `BGMSlider`, `SFXSlider` (range 0..1). Names must match exactly.
    - Root `CanvasPanel`, anchor Stretch (0,0)-(1,1), offsets 0.
    - `VerticalBox "ContentBox"` as a Canvas child: anchor Center (0.5,0.5), alignment
      (0.5,0.5), fixed width (e.g. 400) or Auto size.
    - One row per slider (`HorizontalBox` of a label `TextBlock` + the named `Slider`).
  - [x] `WB_ControlSettings` (parent `TNControlsSettingsWidget`): placeholder for now
    (empty `CanvasPanel` + a "Coming soon" `TextBlock` is enough). Required because
    `WB_Options` needs an instance to satisfy its `ControlsSettings` BindWidget.
  - [x] `WB_Options` (parent `TNOptionsWidget`): required `AudioSettings`
    (`WB_AudioSettings` instance) and `ControlsSettings` (`WB_ControlSettings`
    instance); optional `AudioTabButton`, `ControlsTabButton`, `CloseButton`.
    - Root `CanvasPanel`, anchor Stretch; optional dim/background `Border` behind
      everything.
    - `HorizontalBox "TabBar"` (Audio/Controls buttons): anchor Top-Center (0.5,0),
      alignment (0.5,0), position e.g. (0,20).
    - `Overlay "SectionArea"` holding BOTH the `AudioSettings` and `ControlsSettings`
      instances stacked on top of each other: anchor Center, alignment (0.5,0.5),
      fixed size (e.g. 500x400). No `WidgetSwitcher` needed — C++ toggles
      `SetVisibility(Collapsed/SelfHitTestInvisible)` on each.
    - `CloseButton`: anchor Bottom-Center, position e.g. (0,-20).
  - [x] `WB_PauseMenu` (parent `TNPauseMenuWidget`): required `ResumeButton`,
    `QuitToTitleButton`; optional `OptionsButton`, `MainMenuPanel` (button container),
    `OptionsPanel` (a `WB_Options` instance placed OUTSIDE `MainMenuPanel`).
    - Root `CanvasPanel`, anchor Stretch; a dim background `Border`/`Image` behind.
    - `VerticalBox "MainMenuPanel"` (Resume/Options/Quit buttons): anchor Center,
      alignment (0.5,0.5).
    - `OptionsPanel` (`WB_Options` instance) as a sibling of `MainMenuPanel`, same
      Center anchor (overlap is fine; only one is `Visible` at a time).
  - [x] Title WBP: add a `Button "OptionsButton"` inside the existing `MainMenuPanel`
    (same level as Play/How To Play/Quit), and a `WB_Options` instance named
    `OptionsPanel` as a sibling OUTSIDE `MainMenuPanel`, same anchor as it.
  - [ ] Demo game-mode BP: set `PauseMenuWidgetClass` = `WB_PauseMenu`.
  - [ ] `DA_TNAudio`: register `MenuClick`.

  Play-test verification (not yet tested in editor):
  - [ ] Options sliders work on the title screen; values persist after restart.
  - [ ] Pause freezes the game, BGM keeps playing, resume continues seamlessly.
  - [ ] Pause during rewind: only the reverse loop stops; resume continues the rewind.
  - [ ] Holding a move key while pausing does not move the piece by itself on resume.
  - [ ] Quit to Title from the pause menu leaves no suspended audio behind.
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
