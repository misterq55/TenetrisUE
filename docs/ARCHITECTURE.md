# Tenetris — Architecture

An overview of how the game code is organized. For the download and how to run it, see the
[README](../README.md).

## MVC core (plain C++, not UObject)
The board simulation uses a custom MVC design under `Source/Tenetris/Module/MVC/`. All
parts are reached through the singleton `FTNMVCHolder`.

- **Model** (`FTNFieldModel`, ...): the board simulation. It uses no engine types and does
  no audio or rendering. It reports events through delegates.
- **View**: sends the model's state to the actors on screen.
- **Controller**: receives input and drives the model.

The model does not depend on the engine on purpose. It only reports what happened, and
the Unreal (UObject) layers react with audio, visuals and UI. This keeps the game rules
separate from presentation.

## Rendering (3D, not 2D UMG)
The board is drawn with **3D actors** (`ATNElectronicDisplayField`, with mino actors managed
by `UTNTenetrisBufferComponent`), not with a 2D UMG grid. Space Inversion's 180° flip
animation needs real 3D rotation, which UMG cannot do.

## UI (C++ base + Widget Blueprint layout)
Each widget has a **C++ `UUserWidget` base class** that holds the logic and binds named
widgets with `meta=(BindWidget)`. A **Widget Blueprint subclass** handles only the visual
layout (e.g. `UTNScoreHUDWidget`, `UTNTitleMenuWidget`, `UTNOptionsWidget`,
`UTNPauseMenuWidget`). Menus derive from `UTNMenuWidgetBase` for keyboard and gamepad
navigation.

## Two-layer delegates
Events move between layers through two kinds of delegates:

1. **Model → GameMode** (`DECLARE_DELEGATE`): gameplay events such as `FTNLinesCleared`,
   `FTNGameOver`, `FTNRewindStarted` and `FTNRewindEnded`.
2. **GameMode → UI** (`DECLARE_DYNAMIC_MULTICAST_DELEGATE`, BlueprintAssignable):
   events for the HUD, such as `FTNScoreChanged` and `FTNGameOverUI`.

Match rules such as score, level and game over live in the GameMode, not in the model.
