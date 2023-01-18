#include "PlayerTetromino.h"

void FPlayerTetromino::Move(ETetrominoDirection InTetrominoDirection)
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
