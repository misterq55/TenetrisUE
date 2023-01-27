// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tenetris/TenetrisDefine.h"
#include "TenetrisPlayerController.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_TwoParams(FTetrominoMoveDelegate, ETetrominoDirection, bool);
DECLARE_DELEGATE_OneParam(FTetrominoRotateDelegate, ETetrominoRotation);

DECLARE_DELEGATE_OneParam(FTetrominoSoftDropDelegate, bool);
DECLARE_DELEGATE(FTetrominoHardDropDelegate);

UCLASS()
class TENETRIS_API ATenetrisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

public:
	void StartMoveLeft();
	void StopMoveLeft();
	
	void StartMoveRight();
	void StopMoveRight();
	
	void StartSoftDrop();
	void StopSoftDrop();
	
	void RotateClockWise();
	void RotateCounterClockWise();
	
	void HardDrop();
	void Hold();

public:
	FTetrominoMoveDelegate OnTetrominoMove;
	FTetrominoRotateDelegate OnTetrominoRotate;
	FTetrominoHardDropDelegate OnTetrominoHardDrop;
	FTetrominoSoftDropDelegate OnTetrominoSoftDrop;
};
