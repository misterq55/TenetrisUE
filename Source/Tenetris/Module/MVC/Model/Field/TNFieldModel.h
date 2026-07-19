#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTNTetromino;
class FTNTetrominoGenerator;
class FTNLockDown;
class FTNRecorder;

DECLARE_DELEGATE_TwoParams(FTNUpdateModel, const int32, const E_TNFieldModelStateType);

class FTNFieldModel
{
public:
	FTNFieldModel(FTNFieldContext fieldContext, int32 height, int32 width);
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

	void HandleControlInput(const E_TNControlType controlType);
	
private:
	void startMoveLeft();
	void stopMoveLeft();

	void startMoveRight();
	void stopMoveRight();

	void startSoftDrop();
	void stopSoftDrop();

	void rotateClockWise() const;
	void rotateCounterClockWise() const;

	void hold();
	void rotateField();
	void hardDrop();

private:
	FTNCellInfo getValueFromCheckBuffer(const int32 x, const int32 y) const;
	void setValueToCheckBuffer(const int32 x, const int32 y, const FTNCellInfo& cellInfo);
	bool checkMino(const int32 x, const int32 y) const;
	int32 calculateGuideMinoHeight(const int32 x, const int32 y) const;
	
	void updateTetromino() const;
	
	void checkLineDelete(const TArray<int32>& heights);
	bool isLineDeleted(int32 height) const;

	void spawn();

	bool moveTetromino(E_TNTetrominoDirection tetrominoDirection) const;
	void rotateTetromino(E_TNTetrominoRotation tetrominoRotation) const;
	void setMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed);

	void tetrominoFall(float deltaTime);
	void setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirection) const;
	void updateLockDown(float deltaTime);
	void updateLineDelete(float deltaTime);

	void doLockDown();
	void waitForSpawn();
	void initializeBuffers();
	void createBuffer(TArray<TArray<FTNCellInfo>>& buffer) const;

	void spawnNextTetromino() const;
	void updateHoldTetromino() const;
	void updatePreviewTetrominoes();
	float getFallingSpeed() const;
	bool isSpaceInverting() const
	{
		return RotationPauseRemainingTime > 0.f;
	}

private:
	int32 Id = 0;
	FTNFieldContext FieldContext;
	int32 Height = 0;
	int32 Width = 0;
	FTNUpdateModel OnUpdateModel;

	TSharedPtr<FTNTetromino> CurrentTetromino;
	TSharedPtr<FTNTetrominoGenerator> TetrominoGenerator;

	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;
	float RotationPauseRemainingTime = 0.f;
	TArray<TArray<FTNCellInfo>> CheckBuffer;
	TArray<TArray<FTNCellInfo>> ReversedBuffer;
	
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
	TSharedPtr<FTNRecorder> Recorder;
	bool bWaitForSpawn = false;
	bool bCanHold = true;
};