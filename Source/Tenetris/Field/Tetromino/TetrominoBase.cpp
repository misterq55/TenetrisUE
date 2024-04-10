#include "TetrominoBase.h"

TArray<int32> FTetrominoBase::GetMinoHeights()
{
	TArray<int32> Result;

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		Result.AddUnique(Coord.Y + TetrominoInfo.CurrentPosition.Y);
	}

	return Result;
}

void FTetrominoBase::SetTetrominoPosition(int32 x, int32 y)
{
	TetrominoInfo.SetPosition(x, y);

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

void FTetrominoBase::SetTetrominoType(ETetrominoType currentTetrominoType)
{
	TetrominoInfo.CurrentType = currentTetrominoType;

	switch (TetrominoInfo.CurrentType)
	{
	case ETetrominoType::I:
		TetrominoInfo.Coordinate = IMinoCoordinate;
		break;

	case ETetrominoType::J:
		TetrominoInfo.Coordinate = JMinoCoordinate;
		break;

	case ETetrominoType::L:
		TetrominoInfo.Coordinate = LMinoCoordinate;
		break;

	case ETetrominoType::O:
		TetrominoInfo.Coordinate = OMinoCoordinate;
		break;

	case ETetrominoType::S:
		TetrominoInfo.Coordinate = SMinoCoordinate;
		break;

	case ETetrominoType::T:
		TetrominoInfo.Coordinate = TMinoCoordinate;
		break;

	case ETetrominoType::Z:
		TetrominoInfo.Coordinate = ZMinoCoordinate;
		break;

	default:
		break;
	}
}

void FTetrominoBase::SetStartingLocation(int32 x, int32 y)
{
	StartingLocation = FVector2D(x, y);
}

FVector2D FTetrominoBase::GetStaringLocation()
{
	return StartingLocation;
}

void FTetrominoBase::Spawn()
{
	TetrominoInfo.SetPosition(StartingLocation.X, StartingLocation.Y);
	TetrominoInfo.RotationState = 0;

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

bool FTetrominoBase::checkMino(const FVector2D& simulationPosition)
{
	if (!OnCheckMino.IsBound())
		return false;

	bool TetrominoCheck = false;

	for (const FVector2D& Coord : TetrominoInfo.Coordinate)
	{
		if (OnCheckMino.Execute(Coord.X + simulationPosition.X, Coord.Y + simulationPosition.Y))
		{
			TetrominoCheck = true;
			break;
		}
	}

	return TetrominoCheck;
}

void FTetrominoBase::HideTetromino()
{
	for (const FVector2D& Coord : TetrominoInfo.Coordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, false);
	}
}

void FTetrominoBase::setTetromino()
{
	for (const FVector2D& Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

void FTetrominoBase::setTetrominoBackground()
{
	for (const FVector2D& Coord : TetrominoInfo.Coordinate)
	{
		OnBackgroundCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}
