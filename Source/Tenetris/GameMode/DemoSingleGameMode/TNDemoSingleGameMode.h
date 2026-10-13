// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/TNTenetrisGameModeBase.h"
#include "TNDemoSingleGameMode.generated.h"

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

public:
  int32 GetScore() const { return Score; }
  int32 GetLevel() const { return Level; }
  int32 GetTotalLines() const { return TotalLines; }
  bool IsGameOver() const { return bGameOver; }

private:
  ATNFieldBase* spawnField(const FVector& location, const FVector& scale);

  // Bound to the player board's game-rule delegates.
  void handleLinesCleared(const int32 lineCount);
  void handleGameOver();

  // Lines cleared at once -> points (before level scaling).
  int32 scoreForLines(const int32 lineCount) const;

public:
  TSubclassOf<ATNFieldBase> FieldClass;

protected:
  UPROPERTY()
  AActor* PlayerFieldPositionActor;

private:
  // Match state (outside the model).
  int32 Score = 0;
  int32 TotalLines = 0;
  int32 Level = 1;
  bool bGameOver = false;

  // TODO: Expose these as tunable settings.
  static constexpr int32 LinesPerLevel = 10;

  ATNFieldBase* PlayerField = nullptr;
};