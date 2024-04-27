// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNTenetrisPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class TENETRIS_API ATNTenetrisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

protected:
	void startMoveLeft();
	void stopMoveLeft();

	void startMoveRight();
	void stopMoveRight();

	void startSoftDrop();
	void stopSoftDrop();

	void rotateClockWise();
	void rotateCounterClockWise();

	void hold();
	void toggleSpaceInversion();
	void HardDrop();
};
