// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tenetris/TenetrisDefine.h"
#include "TenetrisPlayerController.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FTetrominoMoveDelegate, ETetrominoDirection);
DECLARE_DELEGATE_OneParam(FTetrominoRotateDelegate, ETetrominoRotation);
DECLARE_DELEGATE_OneParam(FTetrominoToggleSoftDrop, bool);
DECLARE_DELEGATE(FTetrominoHardDropDelegate);

UCLASS()
class TENETRIS_API ATenetrisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

//public:
//	virtual void PlayerTick(float DeltaTime) override;

public:
	void MoveLeft();
	void MoveRight();
	// void MoveDown();
	void StartSoftDrop();
	void StopSoftDrop();
	void RotateClockWise();
	void RotateCounterClockWise();
	void HardDrop();

public:
	FTetrominoMoveDelegate OnTetrominoMove;
	FTetrominoRotateDelegate OnTetrominoRotate;
	FTetrominoHardDropDelegate OnTetrominoHardDrop;
	FTetrominoToggleSoftDrop OnToggleSoftDrop;
};
