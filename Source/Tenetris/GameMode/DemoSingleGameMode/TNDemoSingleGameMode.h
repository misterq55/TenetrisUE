// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/TNTenetrisGameModeBase.h"
#include "TNDemoSingleGameMode.generated.h"

// Broadcast whenever the match score/level/total-lines change (for HUD binding).
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTNScoreChanged, int32, Score, int32, Level, int32, TotalLines);

// Broadcast once when the board tops out (for HUD game-over panel).
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTNGameOverUI);

// Demo single-player game mode.
//
// Minimal match rules living OUTSIDE the board model (the model stays a pure board simulation):
//   - score (Single/Double/Triple/Tetris, scaled by level)
//   - level progression by total cleared lines, feeding fall speed
//   - game over on spawn block-out
//
// Rules are driven by the model's game-rule delegates (FTNLinesCleared / FTNGameOver),
// kept separate from the view-update delegate.
UCLASS()
class TENETRIS_API ATNDemoSingleGameMode : public ATNTenetrisGameModeBase
{
  GENERATED_BODY()

public:
  ATNDemoSingleGameMode();
  virtual void StartPlay() override;
  virtual void Tick(float deltaSeconds) override;
  virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:
  UFUNCTION(BlueprintPure, Category = "TenetrisGameMode")
  int32 GetScore() const { return Score; }

  UFUNCTION(BlueprintPure, Category = "TenetrisGameMode")
  int32 GetMatchLevel() const { return Level; }

  UFUNCTION(BlueprintPure, Category = "TenetrisGameMode")
  int32 GetTotalLines() const { return TotalLines; }

  UFUNCTION(BlueprintPure, Category = "TenetrisGameMode")
  bool IsGameOver() const { return bGameOver; }

  UFUNCTION(BlueprintPure, Category = "TenetrisGameMode")
  bool IsGamePaused() const { return bPaused; }

  // Toggles the pause menu (ignored after game over).
  UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
  void TogglePause();

  // Pauses/resumes the simulation, music and gameplay input and shows/hides the pause menu.
  UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
  void SetGamePaused(const bool bShouldPause);

  // Restart the match from scratch (reloads the current level). Bound to the HUD replay button.
  UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
  void RestartMatch();

  // Return to the title/main-menu level. Bound to the HUD main-menu button.
  UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
  void ReturnToTitle();

public:
  UPROPERTY(BlueprintAssignable, Category = "TenetrisGameMode")
  FTNScoreChanged OnScoreChanged;

  UPROPERTY(BlueprintAssignable, Category = "TenetrisGameMode")
  FTNGameOverUI OnGameOverUI;

private:
  ATNFieldBase* spawnField(const FVector& location, const FVector& scale);

  // Bound to the player board's game-rule delegates.
  void handleLinesCleared(const int32 lineCount);
  void handleGameOver();
  void handleLockDown();

  // Bound to the player board's rewind lifecycle: pause/resume the main BGM and
  // play a looping rewind track for the (variable-length) rewind duration.
  void handleRewindStarted();
  void handleRewindEnded();

  // Convenience accessor for the game-instance audio subsystem (may be null).
  class UTNAudioSubsystem* getAudioSubsystem() const;

  // Lines cleared at once -> points (before level scaling).
  int32 scoreForLines(const int32 lineCount) const;

public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TenetrisGameMode")
  TSubclassOf<ATNFieldBase> FieldClass;

  // HUD widget (e.g. WBP_ScoreHUD) created and shown on StartPlay. Demo-only; assigned in the BP.
  UPROPERTY(EditDefaultsOnly, Category = "TenetrisGameMode")
  TSubclassOf<UUserWidget> HUDWidgetClass;

  // Pause menu widget (e.g. WBP_PauseMenu, based on UTNPauseMenuWidget) shown while paused.
  UPROPERTY(EditDefaultsOnly, Category = "TenetrisGameMode")
  TSubclassOf<UUserWidget> PauseMenuWidgetClass;

  // Level opened when returning to the title (set in the BP).
  UPROPERTY(EditDefaultsOnly, Category = "TenetrisGameMode")
  FName TitleLevelName = FName("TitleMap");

private:
  // Match state (outside the model).
  int32 Score = 0;
  int32 TotalLines = 0;
  int32 Level = 1;
  bool bGameOver = false;
  bool bPaused = false;
  bool bCursorWasVisible = false;

  UPROPERTY(Transient)
  TObjectPtr<UUserWidget> PauseMenuWidget;

  // TODO: Expose these as tunable settings.
  static constexpr int32 LinesPerLevel = 10;
};