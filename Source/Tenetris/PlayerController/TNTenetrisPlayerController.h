// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TNTenetrisPlayerController.generated.h"

/**
 * 
 */

class UInputAction;

UCLASS()
class TENETRIS_API ATNTenetrisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	// While blocked, gameplay input is ignored (e.g. pause menu open). Blocking also
	// releases any held movement/soft-drop so nothing stays stuck across the pause.
	void SetGameplayInputBlocked(const bool bBlocked);

protected:
	void togglePause();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SoftDropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateClockWiseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateCounterClockWiseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HardDropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HoldAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SpaceInversionAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TimeInversionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseAction;
	
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
	void toggleTimeInversion();
	void hardDrop();

private:
	bool bGameplayInputBlocked = false;
};
