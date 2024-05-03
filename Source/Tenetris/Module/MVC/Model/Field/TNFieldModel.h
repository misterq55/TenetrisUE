#include "Tenetris/TenetrisDefine.h"

class ATNFieldBase;
class FTNTetrominoBase;
class FTNTetrominoGenerator;

class FTNFieldModel
{
public:
	FTNFieldModel() {}
	FTNFieldModel(FTNFieldInfo fieldInfo);
		

public:
	void Tick(float deltaSeconds);
	void SetBufferSize(const int32 bufferHeight, const int32 bufferWidth);
	E_TNTetrominoType GetValueFromCheckBuffer(const int32 x, const int32 y) const;
	void SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	bool CheckMino(const int32 x, const int32 y);
	int32 CalculateGuideMinoHeight(const int32 x, const int32 y);
	void CheckLineDelete(const TArray<int32>& heights);
	bool IsLineDeleted(int32 height) const;
	void HandleLineDeletion(const TArray<int32>& linesToDelete);
	TArray<TArray<E_TNTetrominoType>>& GetCheckBuffer();

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

public:
	void AddFieldActor(ATNFieldBase* fieldActor);

private:
	bool moveTetromino(E_TNTetrominoDirection tetrominoDirection);
	void rotateTetromino(E_TNTetrominoRotation tetrominoRotation);
	void setMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed);
	void setSoftDrop(bool softDrop) { bSoftDrop = softDrop; }
	bool getSoftDrop() { return bSoftDrop; }
	void lineDelete();
	void doLockDown();

private:

	FTNFieldInfo FieldInfo;
	ATNFieldBase* FieldActor;

	TSharedPtr<FTNTetrominoBase> CurrentTetromino;
	TSharedPtr<FTNTetrominoGenerator> TetrominoGenerator;
	TArray<TSharedPtr<FTNTetrominoBase>> PreviewTetrominos;

	TArray<TArray<E_TNTetrominoType>> CheckBuffer;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	TArray<int32> DeletedLines;
	float DeleteLineCheckTime = 0.f;
	bool bLineDeleting = false;
	bool bSpaceInverted = false;

	int32 PreviewTetrominoNum;
	float TetrominoFallingSpeed = 1.f;
	float CurrentTime = 0.f;

	TSharedPtr<FTNTetrominoBase> HoldTetromino;

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