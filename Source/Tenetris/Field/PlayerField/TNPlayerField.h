// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Field/TNFieldBase.h"
#include "TNPlayerField.generated.h"

class FTNTetrominoBase;
class FTNTetrominoGenerator;

class FTNLockDown
{
public:
	FTNLockDown() 
	{
		LockDownRemainCount = MaxLockDownRemainCount;
	}

	~FTNLockDown() {}
	void StartLockDown() 
	{
		LockDownStart = true;
	}

	bool UpdateLockDown(float deltaTime) 
	{ 
		if (LockDownStart)
		{
			LockDownTime += deltaTime;

			if (LockDownTime >= LockDownDelay || LockDownRemainCount <= 0)
			{
				LockDownTime = 0.f;
				LockDownStart = false;
				LockDownRemainCount = MaxLockDownRemainCount;

				return true;
			}
		}

		return false; 
	}

	void CheckRemainCount(E_TNTetrominoDirection tetrominoDirection)
	{
		if (!LockDownStart)
		{
			return;
		}

		if (tetrominoDirection == E_TNTetrominoDirection::Left
			|| tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			LockDownRemainCount--;
		}
		else if (tetrominoDirection == E_TNTetrominoDirection::Down)
		{
			LockDownRemainCount = MaxLockDownRemainCount;
			LockDownStart = false;
		}

		LockDownTime = 0.f;
	}

	void CheckRemainCount()
	{
		if (!LockDownStart)
		{
			return;
		}

		LockDownRemainCount--;
		LockDownTime = 0.f;
	}

private:
	float LockDownDelay = 0.5f;
	float LockDownTime = 0.f;
	bool LockDownStart = false;
	const int32 MaxLockDownRemainCount = 15;
	int32 LockDownRemainCount = 0;
};

UCLASS()
class TENETRIS_API ATNPlayerField : public ATNFieldBase
{
	GENERATED_BODY()

	struct FTNMoveDirectionState
	{
	public:
		FTNMoveDirectionState()
			: Pressed(false)
			, PressedTime(0.f)
			, AutoRepeatKickIn(false)
			, AutoRepeatTime(0.f)
		{}
		
		bool Pressed;
		float PressedTime;
		bool AutoRepeatKickIn;
		float AutoRepeatTime;
	};

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
	void BindTetrominoToBuffer(FTNTetrominoBase* tetromino, UTNTenetrisBufferComponent* buffer);
	void Spawn();

	void StartMoveLeft();
	void StopMoveLeft();

	void StartMoveRight();
	void StopMoveRight();

	void StartSoftDrop();
	void StopSoftDrop();

	void RotateClockWise();
	void RotateCounterClockWise();

	void Hold();
	void ToggleSpaceInversion();

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
