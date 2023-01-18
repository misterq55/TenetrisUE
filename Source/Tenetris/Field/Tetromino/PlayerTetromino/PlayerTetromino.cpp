#include "PlayerTetromino.h"

// �� �Լ��� �ʵ�� �ø��� ���
// Lock Down�� �ʵ��� �����ΰ�, ��Ʈ�ι̳��� �����ΰ�?
void FPlayerTetromino::Move(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = SimulatePosition(InTetrominoDirection);

	if (!CheckTetrominoCube(SimulationPosition))
	{
		HideTetromino();
		TetrominoInfo.TetrominoCurrentPosition = SimulationPosition;
		SetTetromino();
	}
	else
	{
		if (InTetrominoDirection == ETetrominoDirection::Down)
			LockDown();
	}
}

void FPlayerTetromino::Rotate(ETetrominoRotation InTetrominoRotation)
{

}

void FPlayerTetromino::LockDown()
{
	HideTetromino();
	SetTetrominoBackground();
	Spawn();
}

FVector2D FPlayerTetromino::SimulatePosition(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = TetrominoInfo.TetrominoCurrentPosition;
	switch (InTetrominoDirection)
	{
	case ETetrominoDirection::Down:
		SimulationPosition.Y -= 1;
		break;

	case ETetrominoDirection::Left:
		SimulationPosition.X -= 1;
		break;

	case ETetrominoDirection::Right:
		SimulationPosition.X += 1;
		break;
	};

	return SimulationPosition;
}
