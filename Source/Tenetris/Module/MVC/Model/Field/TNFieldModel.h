#pragma once

#include "Tenetris/TenetrisDefine.h"
class FTNTetromino;
class FTNTetrominoGenerator;
class FTNLockDown;

DECLARE_DELEGATE_TwoParams(FTNUpdateModel, const int32, const E_TNFieldModelStateType);

class FTNFieldModel
{
public:
	FTNFieldModel(FTNFieldContext fieldContext);
	virtual ~FTNFieldModel() {}

public:
	void Initialize();
	void SetId(const int32 id);
	void Tick(float deltaTime);

	FTNFieldContext& GetFieldContext() {
		return FieldContext;
	}

	virtual FTNUpdateModel& GetOnUpdateModelDelegate() {
		return OnUpdateModel;
	}

	TSharedPtr<FTNTetromino> GetCurrentTetromino() const
	{
		return CurrentTetromino;
	}

	int32 GetId() const;

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
	E_TNTetrominoType getValueFromCheckBuffer(const int32 x, const int32 y) const;
	void setValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	bool checkMino(const int32 x, const int32 y) const;
	int32 calculateGuideMinoHeight(const int32 x, const int32 y) const;
	
	void updateTetromino() const;
	
	void checkLineDelete(const TArray<int32>& heights);
	bool isLineDeleted(int32 height) const;

	void spawn();

	bool moveTetromino(E_TNTetrominoDirection tetrominoDirection);
	void rotateTetromino(E_TNTetrominoRotation tetrominoRotation);
	void setMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed);

	void tetrominoFall(float deltaTime);
	void setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirection);
	void updateLockDown(float deltaTime);
	void updateLineDelete(float deltaTime);

	void lineDelete();
	void doLockDown();
	void waitForSpawn();
	void handleLineDeletion(const TArray<int32>& linesToDelete);
	void initializeBuffers();
	void createBuffer(TArray<TArray<E_TNTetrominoType>>& buffer) const;

	void spawnNextTetromino() const;
	void updateHoldTetromino();
	void updatePreviewTetrominoes();
	float getFallingSpeed() const;
	bool isSpaceInverting() const
	{
		return RotationPauseRemainingTime > 0.f;
	}

private:
	int32 Id = 0;
	FTNFieldContext FieldContext;
	FTNUpdateModel OnUpdateModel;

	TSharedPtr<FTNTetromino> CurrentTetromino;
	TSharedPtr<FTNTetrominoGenerator> TetrominoGenerator;

	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;
	float RotationPauseRemainingTime = 0.f;
	TArray<TArray<E_TNTetrominoType>> CheckBuffer;
	TArray<TArray<E_TNTetrominoType>> ReversedBuffer;

	TArray<int32> DeletedLines;
	float DeleteLineCheckTime = 0.f;
	bool bLineDeleting = false;

	bool bSoftDrop = false;
	FTNMoveDirectionState LeftDirectionState;
	FTNMoveDirectionState RightDirectionState;
	E_TNTetrominoDirection TetrominoMoveDirection = E_TNTetrominoDirection::None;
	float KickInDelay = 0.3f;
	float MoveSpeed = 0.05f;

	TSharedPtr<FTNLockDown> LockDown;
	bool bWaitForSpawn = false;
	bool bCanHold = true;
};