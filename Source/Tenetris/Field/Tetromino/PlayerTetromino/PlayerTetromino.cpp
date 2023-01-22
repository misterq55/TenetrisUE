#include "PlayerTetromino.h"

bool FPlayerTetromino::Move(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = SimulatePosition(InTetrominoDirection);

	if (!CheckMino(SimulationPosition))
	{
		HideTetromino();
		
		TetrominoInfo.CurrentPosition = SimulationPosition;

		if (InTetrominoDirection == ETetrominoDirection::Left ||
			InTetrominoDirection == ETetrominoDirection::Right)
		{
			HideGuideTetromino();
			SetGuideTetromino();
		}

		SetTetromino();

		return false;
	}

	return true;
}

bool FPlayerTetromino::Rotate(ETetrominoRotation InTetrominoRotation)
{
	HideTetromino();
	HideGuideTetromino();

	TArray<FVector2D> SimulationCoordinates = TetrominoInfo.Coordinate;
	FVector2D RotateMatrix[2];

	if (InTetrominoRotation == ETetrominoRotation::ClockWise)
	{
		RotateMatrix[0] = FVector2D(0, -1);
		RotateMatrix[1] = FVector2D(1, 0);
	}
	else if (InTetrominoRotation == ETetrominoRotation::CounterClockWise)
	{
		RotateMatrix[0] = FVector2D(0, 1);
		RotateMatrix[1] = FVector2D(-1, 0);
	}

	for (FVector2D &Coord : SimulationCoordinates)
	{
		Coord.X -= 1;
		int32 NewX = (RotateMatrix[0].X * Coord.X) + (RotateMatrix[1].X * Coord.Y);
		int32 NewY = (RotateMatrix[0].Y * Coord.X) + (RotateMatrix[1].Y * Coord.Y);

		Coord = FVector2D(NewX, NewY);
		Coord.X += 1;
	}

	TetrominoInfo.Coordinate = SimulationCoordinates;

	SetGuideTetromino();
	SetTetromino();

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
	HideGuideTetromino();
	SetGuideTetromino();
}

void FPlayerTetromino::SetGuideTetromino()
{
	if (!OnCalulateGuideMino.IsBound())
		return;

	TArray<int32> CheckHeightArray;

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		int32 Height = OnCalulateGuideMino.Execute(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y);
		CheckHeightArray.Add(Height);
	}

	int32 MinHeight = MAX_int32;
	
	for (int32 Height : CheckHeightArray)
	{
		if (MinHeight > Height)
			MinHeight = Height;
	}

	GuideTetrominoPosition = FVector2D(TetrominoInfo.CurrentPosition.X, TetrominoInfo.CurrentPosition.Y - MinHeight);

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + GuideTetrominoPosition.X, Coord.Y + GuideTetrominoPosition.Y, ETetrominoType::Guide);
	}
}

void FPlayerTetromino::HardDrop()
{
	HideTetromino();
	TetrominoInfo.CurrentPosition = GuideTetrominoPosition;
	HideGuideTetromino();
	SetGuideTetromino();
	GuideTetrominoPosition = FVector2D(0, 0);
	SetTetromino();
}

FVector2D FPlayerTetromino::SimulatePosition(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = TetrominoInfo.CurrentPosition;
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

void FPlayerTetromino::HideGuideTetromino()
{
	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(Coord.X + GuideTetrominoPosition.X, Coord.Y + GuideTetrominoPosition.Y, false);
	}
}
