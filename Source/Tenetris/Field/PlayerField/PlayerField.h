// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Field/FieldBase.h"
#include "PlayerField.generated.h"

class FTetrominoBase;
class FTetrominoGenerator;

class FLockDown
{
public:
	FLockDown() 
	{
		LockDownRemainCount = MaxLockDownRemainCount;
	}

	~FLockDown() {}
	void StartLockDown() 
	{
		LockDownStart = true;
	}

	bool UpdateLockDown(float DeltaTime) 
	{ 
		if (LockDownStart)
		{
			LockDownTime += DeltaTime;

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

	void CheckRemainCount(ETetrominoDirection InTetrominoDirection)
	{
		if (!LockDownStart)
			return;

		if (InTetrominoDirection == ETetrominoDirection::Left
			|| InTetrominoDirection == ETetrominoDirection::Right)
		{
			LockDownRemainCount--;
		}
		else if (InTetrominoDirection == ETetrominoDirection::Down)
		{
			LockDownRemainCount = MaxLockDownRemainCount;
			LockDownStart = false;
		}

		LockDownTime = 0.f;
	}

	void CheckRemainCount()
	{
		if (!LockDownStart)
			return;

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
class TENETRIS_API APlayerField : public AFieldBase
{
	GENERATED_BODY()

	typedef struct FMoveDirectionState
	{
	public:
		FMoveDirectionState()
			: Pressed(false)
			, PressedTime(0.f)
			, AutoRepeatKickIn(false)
			, AutoRepeatTime(0.f)
		{}
		
		bool Pressed;
		float PressedTime;
		bool AutoRepeatKickIn;
		float AutoRepeatTime;
	}FMoveDirectionState;

public:	
	// Sets default values for this actor's properties
	APlayerField();
	virtual ~APlayerField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual	void Initialize();

	bool MoveTetromino(ETetrominoDirection InTetrominoDirection);
	void RotateTetromino(ETetrominoRotation InTetrominoRotation);
	void HardDrop();
	void SetSoftDrop(bool InSoftDrop) { bSoftDrop = InSoftDrop; }
	bool GetSoftDrop() { return bSoftDrop; }
	void SetMoveDirection(ETetrominoDirection InTetrominoDirection, bool InPressed);

private:
	void RegisterActions();
	void UnRegisterActions();
	void InitializePreviewBuffer();
	void InitializePreviewTetrominos();
	void InitializeHoldBuffer();
	void InitializeHoldTetromino();
	void SpawnNextTetromino();
	void RenewPreviewTetromino();
	float GetFallingSpeed();
	void TetrominoFall(float DeltaTime);
	void SetMoveState(float DeltaTime, FMoveDirectionState& InMoveState, ETetrominoDirection InTetrominoDirction);
	void UpdateLockDown(float DeltaTime);
	void DoLockDown();
	void LineDelete();
	void WaitForSpawn();

protected:
	void BindTetrominoToBuffer(FTetrominoBase* InTetromino, UTenetrisBufferComponent* InBuffer);
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

protected:
	FTetrominoBase* CurrentTetromino;
	FTetrominoGenerator* TetrominoGenerator;
	TArray<FTetrominoBase*> PreviewTetrominos;
	UTenetrisBufferComponent* PreviewBufferComponent;
	int32 PreviewTetrominoNum;
	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;

	UTenetrisBufferComponent* HoldBufferComponent;
	FTetrominoBase* HoldTetromino;

private:
	bool bSoftDrop = false;
	FMoveDirectionState LeftDirectionState;
	FMoveDirectionState RightDirectionState;
	ETetrominoDirection TetrominoMoveDirection = ETetrominoDirection::None;
	float KickInDelay = 0.3f;
	float MoveSpeed = 0.05f;
FLockDown LockDown;
	bool bWaitForSpawn = false;
};
