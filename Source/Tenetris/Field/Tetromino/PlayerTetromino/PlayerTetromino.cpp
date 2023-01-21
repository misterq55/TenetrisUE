#include "PlayerTetromino.h"

bool FPlayerTetromino::Move(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = SimulatePosition(InTetrominoDirection);

	if (!CheckMino(SimulationPosition))
	{
		HideTetromino();
		TetrominoInfo.TetrominoCurrentPosition = SimulationPosition;

		if (InTetrominoDirection == ETetrominoDirection::Left ||
			InTetrominoDirection == ETetrominoDirection::Right)
			SetGuideTetromino();

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

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(Coord.X + GuideTetrominoPosition.X, Coord.Y + GuideTetrominoPosition.Y, false);
	}

	TArray<int32> CheckHeightArray;

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		int32 Height = OnCalulateGuideMino.Execute(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y);
		CheckHeightArray.Add(Height);
	}

	int32 MinHeight = MAX_int32;
	
	for (int32 Height : CheckHeightArray)
	{
		if (MinHeight > Height)
			MinHeight = Height;
	}

	GuideTetrominoPosition = FVector2D(TetrominoInfo.TetrominoCurrentPosition.X, TetrominoInfo.TetrominoCurrentPosition.Y - MinHeight);

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + GuideTetrominoPosition.X, Coord.Y + GuideTetrominoPosition.Y, ETetrominoType::Guide);
	}
}

void FPlayerTetromino::HardDrop()
{
	HideTetromino();
	TetrominoInfo.TetrominoCurrentPosition = GuideTetrominoPosition;
	SetGuideTetromino();
	SetTetromino();
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

	if (SimulationPosition.X < 0)
		int32 Temp = 0;

	return SimulationPosition;
}
