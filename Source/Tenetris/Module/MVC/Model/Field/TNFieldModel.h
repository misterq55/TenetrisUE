#pragma once

#include "Tenetris/TenetrisDefine.h"
class FTNTetrominoBase;
class FTNPlayerTetromino;
class FTNTetrominoGenerator;

DECLARE_DELEGATE_TwoParams(FTNUpdateModel, const int32, const E_TNFieldModelStateType);

class FTNFieldModel
{
public:
	FTNFieldModel(FTNFieldContext fieldContext);
	virtual ~FTNFieldModel() {}

public:
	void Initialize();
	void SetId(const int32 id);
	int32 GetId() const;
	void Tick(float deltaTime);
	E_TNTetrominoType GetValueFromCheckBuffer(const int32 x, const int32 y) const;
	void SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	bool CheckMino(const int32 x, const int32 y) const;
	int32 CalculateGuideMinoHeight(const int32 x, const int32 y) const;
	void HidePreviewTetromino() const;
	void SetPreviewTetromino() const;
	void HideHoldTetromino() const;
	void SetHoldTetromino() const;
	void HideTetromino() const;
	void SetTetromino() const;
	void HideGuideTetromino() const;
	void SetGuideTetromino() const;
	void CheckLineDelete(const TArray<int32>& heights);
	bool IsLineDeleted(int32 height) const;
	void HandleLineDeletion(const TArray<int32>& linesToDelete);

	FTNFieldContext& GetFieldContext() {
		return FieldContext;
	}

	virtual FTNUpdateModel& GetOnUpdateModelDelegate() {
		return OnUpdateModel;
	}

	TSharedPtr<FTNPlayerTetromino> GetCurrentTetromino() const
	{
		return CurrentTetromino;
	}
	
	TSharedPtr<FTNTetrominoBase> GetHoldTetromino() const
	{
		return HoldTetromino;
	}

	TArray<TSharedPtr<FTNTetrominoBase>>& GetPreviewTetrominos()
	{
		return PreviewTetrominos;
	}

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
	void HardDrop();

private:
	void initializePreviewTetrominos();
	void initializeHoldTetromino();

	void spawn();

	bool moveTetromino(E_TNTetrominoDirection tetrominoDirection);
	void rotateTetromino(E_TNTetrominoRotation tetrominoRotation);
	void setMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed);

	void tetrominoFall(float deltaTime);
	void setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirction);
	void updateLockDown(float deltaTime);
	void updateLineDelete(float deltaTime);

	void lineDelete();
	void doLockDown();
	void waitForSpawn();

	void spawnNextTetromino();
	void renewPreviewTetromino();
	float getFallingSpeed() const;

private:
	FTNFieldContext FieldContext;

	TSharedPtr<FTNPlayerTetromino> CurrentTetromino;
	TSharedPtr<FTNTetrominoGenerator> TetrominoGenerator;
	TArray<TSharedPtr<FTNTetrominoBase>> PreviewTetrominos;

	TArray<int32> DeletedLines;
	float DeleteLineCheckTime = 0.f;
	bool bLineDeleting = false;

	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;

	TSharedPtr<FTNTetrominoBase> HoldTetromino;

	FTNUpdateModel OnUpdateModel;

private:
	int32 Id = 0;
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