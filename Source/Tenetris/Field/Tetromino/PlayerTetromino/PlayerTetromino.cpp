#include "PlayerTetromino.h"

bool FPlayerTetromino::Move(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = SimulatePosition(InTetrominoDirection);

	if (!CheckMino(SimulationPosition))
	{
		HideTetromino();
		TetrominoInfo.TetrominoCurrentPosition = SimulationPosition;
		SetTetromino();

		return false;
	}

	return true;
}

bool FPlayerTetromino::Rotate(ETetrominoRotation InTetrominoRotation)
{
	return true;
}

void FPlayerTetromino::LockDown()
{
	HideTetromino();
	SetTetrominoBackground();
}

void FPlayerTetromino::Spawn()
{
	FTetrominoBase::Spawn();
	SetGuideTetromino();
}

void FPlayerTetromino::SetGuideTetromino()
{
	if (!OnCalulateGuideMino.IsBound())
		return;

	TArray<int32> TempCheckHeight;

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		int32 Height = OnCalulateGuideMino.Execute(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y);
		TempCheckHeight.Add(Height);
	}

	int32 temp = 0;
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
