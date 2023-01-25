// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Field/FieldBase.h"
#include "PlayerField.generated.h"

class FTetrominoBase;
class FTetrominoGenerator;

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

	void MoveTetromino(ETetrominoDirection InTetrominoDirection);
	void RotateTetromino(ETetrominoRotation InTetrominoRotation);
	void HardDrop();
	void SetSoftDrop(bool InSoftDrop) { SoftDrop = InSoftDrop; }
	bool GetSoftDrop() { return SoftDrop; }
	void SetMoveDirection(ETetrominoDirection InTetrominoDirection, bool InPressed);

private:
	void RegisterActions();
	void UnRegisterActions();
	void InitializePreviewTetrominos();
	void SpawnNextTetromino();
	void RenewPreviewTetromino();
	float GetFallingSpeed();
	void TetrominoFall(float DeltaTime);
	void SetMoveState(float DeltaTime, FMoveDirectionState& InMoveState, ETetrominoDirection InTetrominoDirction);

protected:
	void BindTetrominoToBuffer(FTetrominoBase* InTetromino, UTenetrisBufferComponent* InBuffer);
	void Spawn();

protected:
	FTetrominoBase* CurrentTetromino;
	FTetrominoGenerator* TetrominoGenerator;
	TArray<FTetrominoBase*> PreviewTetrominos;
	UTenetrisBufferComponent* PreviewBufferComponent;
	int32 PreviewTetrominoNum;
	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;

private:
	bool SoftDrop = false;
	FMoveDirectionState LeftDirectionState;
	FMoveDirectionState RightDirectionState;
	ETetrominoDirection TetrominoMoveDirection = ETetrominoDirection::None;
	float KickInDelay = 0.3f;
	float MoveSpeed = 0.05f;
};
