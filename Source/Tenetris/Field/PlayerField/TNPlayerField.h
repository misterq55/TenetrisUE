// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Field/TNFieldBase.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNPlayerField.generated.h"

class FTNTetrominoBase;
class FTNTetrominoGenerator;

UCLASS()
class TENETRIS_API ATNPlayerField : public ATNFieldBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATNPlayerField();
	virtual ~ATNPlayerField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual	void Initialize();

	bool MoveTetromino(E_TNTetrominoDirection tetrominoDirection);
	void RotateTetromino(E_TNTetrominoRotation tetrominoRotation);
	void HardDrop();
	void SetSoftDrop(bool softDrop) { bSoftDrop = softDrop; }
	bool GetSoftDrop() { return bSoftDrop; }
	void SetMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed);

private:
	void registerActions();
	void unRegisterActions();
	void initializePreviewBuffer();
	void initializePreviewTetrominos();
	void initializeHoldBuffer();
	void initializeHoldTetromino();
	void spawnNextTetromino();
	void renewPreviewTetromino();
	float getFallingSpeed();
	void tetrominoFall(float deltaTime);
	void setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirction);
	void updateLockDown(float deltaTime);
	void doLockDown();
	void lineDelete();
	void waitForSpawn();

protected:
	void bindTetrominoToBuffer(FTNTetrominoBase* tetromino, UTNTenetrisBufferComponent* buffer);
	void spawn();

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

protected:
	FTNTetrominoBase* CurrentTetromino;
	FTNTetrominoGenerator* TetrominoGenerator;
	TArray<FTNTetrominoBase*> PreviewTetrominos;
	UTNTenetrisBufferComponent* PreviewBufferComponent;
	int32 PreviewTetrominoNum;
	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;

	UTNTenetrisBufferComponent* HoldBufferComponent;
	FTNTetrominoBase* HoldTetromino;

private:
	bool bSoftDrop = false;
	FTNMoveDirectionState LeftDirectionState;
	FTNMoveDirectionState RightDirectionState;
	E_TNTetrominoDirection TetrominoMoveDirection = E_TNTetrominoDirection::None;
	float KickInDelay = 0.3f;
	float MoveSpeed = 0.05f;
	FTNLockDown LockDown;
	bool bWaitForSpawn = false;
	bool bCanHold = true;
};
