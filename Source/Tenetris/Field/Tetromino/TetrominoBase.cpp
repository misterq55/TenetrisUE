#include "TetrominoBase.h"

void FTetrominoBase::SetTetrominoPosition(int32 X, int32 Y)
{
	TetrominoInfo.SetPosition(X, Y);

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
	}
}

void FTetrominoBase::SetTetrominoType(ETetrominoType InCurrentTetrominoType)
{
	TetrominoInfo.CurrentTetrominoType = InCurrentTetrominoType;

	switch (TetrominoInfo.CurrentTetrominoType)
	{
	case ETetrominoType::I:
		TetrominoInfo.TetrominoCoordinate = IMinoCoordinate;
		break;

	case ETetrominoType::J:
		TetrominoInfo.TetrominoCoordinate = JMinoCoordinate;
		break;

	case ETetrominoType::L:
		TetrominoInfo.TetrominoCoordinate = LMinoCoordinate;
		break;

	case ETetrominoType::O:
		TetrominoInfo.TetrominoCoordinate = OMinoCoordinate;
		break;

	case ETetrominoType::S:
		TetrominoInfo.TetrominoCoordinate = SMinoCoordinate;
		break;

	case ETetrominoType::T:
		TetrominoInfo.TetrominoCoordinate = TMinoCoordinate;
		break;

	case ETetrominoType::Z:
		TetrominoInfo.TetrominoCoordinate = ZMinoCoordinate;
		break;

	default:
		break;
	}
}

void FTetrominoBase::SetStartingLocation(int32 X, int32 Y)
{
	StartingLocation = FVector2D(X, Y);
}

FVector2D FTetrominoBase::GetStaringLocation()
{
	return StartingLocation;
}

void FTetrominoBase::Spawn()
{
	TetrominoInfo.SetPosition(StartingLocation.X, StartingLocation.Y);

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
	}
}

bool FTetrominoBase::CheckMino(FVector2D InSimulationPosition)
{
	if (!OnCheckMino.IsBound())
		return false;

	bool TetrominoCheck = false;

	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		if (OnCheckMino.Execute(Coord.X + InSimulationPosition.X, Coord.Y + InSimulationPosition.Y))
		{
			TetrominoCheck = true;
			break;
		}
	}

	return TetrominoCheck;
}

void FTetrominoBase::HideTetromino()
{
	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, false);
	}
}

void FTetrominoBase::SetTetromino()
{
	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
	}
}

void FTetrominoBase::SetTetrominoBackground()
{
	for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
	{
		OnBackgroundCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
	}
}
