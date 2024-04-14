#include "TNTetrominoBase.h"

TArray<int32> FTNTetrominoBase::GetMinoHeights()
{
	TArray<int32> result;

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		result.AddUnique(coord.Y + TetrominoInfo.CurrentPosition.Y);
	}

	return result;
}

void FTNTetrominoBase::SetTetrominoPosition(int32 x, int32 y)
{
	TetrominoInfo.SetPosition(x, y);

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(coord.X + TetrominoInfo.CurrentPosition.X, coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

void FTNTetrominoBase::SetTetrominoType(E_TNTetrominoType currentTetrominoType)
{
	TetrominoInfo.CurrentType = currentTetrominoType;

	switch (TetrominoInfo.CurrentType)
	{
	case E_TNTetrominoType::I:
		TetrominoInfo.Coordinate = IMinoCoordinate;
		break;

	case E_TNTetrominoType::J:
		TetrominoInfo.Coordinate = JMinoCoordinate;
		break;

	case E_TNTetrominoType::L:
		TetrominoInfo.Coordinate = LMinoCoordinate;
		break;

	case E_TNTetrominoType::O:
		TetrominoInfo.Coordinate = OMinoCoordinate;
		break;

	case E_TNTetrominoType::S:
		TetrominoInfo.Coordinate = SMinoCoordinate;
		break;

	case E_TNTetrominoType::T:
		TetrominoInfo.Coordinate = TMinoCoordinate;
		break;

	case E_TNTetrominoType::Z:
		TetrominoInfo.Coordinate = ZMinoCoordinate;
		break;

	default:
		break;
	}
}

void FTNTetrominoBase::SetStartingLocation(int32 x, int32 y)
{
	StartingLocation = FVector2D(x, y);
}

FVector2D FTNTetrominoBase::GetStaringLocation()
{
	return StartingLocation;
}

void FTNTetrominoBase::Spawn()
{
	TetrominoInfo.SetPosition(StartingLocation.X, StartingLocation.Y);
	TetrominoInfo.RotationState = 0;

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(coord.X + TetrominoInfo.CurrentPosition.X, coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

bool FTNTetrominoBase::checkMino(const FVector2D& simulationPosition)
{
	if (!OnCheckMino.IsBound())
		return false;

	bool tetrominoCheck = false;

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		if (OnCheckMino.Execute(coord.X + simulationPosition.X, coord.Y + simulationPosition.Y))
		{
			tetrominoCheck = true;
			break;
		}
	}

	return tetrominoCheck;
}

void FTNTetrominoBase::HideTetromino()
{
	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(coord.X + TetrominoInfo.CurrentPosition.X, coord.Y + TetrominoInfo.CurrentPosition.Y, false);
	}
}

void FTNTetrominoBase::setTetromino()
{
	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(coord.X + TetrominoInfo.CurrentPosition.X, coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

void FTNTetrominoBase::setTetrominoBackground()
{
	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnBackgroundCubeType.ExecuteIfBound(coord.X + TetrominoInfo.CurrentPosition.X, coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}
