#include "TetrominoBase.h"

void FTetrominoBase::SetTetrominoPosition(int32 X, int32 Y)
{
	TetrominoInfo.SetPosition(X, Y);

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

void FTetrominoBase::SetTetrominoType(ETetrominoType InCurrentTetrominoType)
{
	TetrominoInfo.CurrentType = InCurrentTetrominoType;

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
	TetrominoInfo.RotationState = 0;

	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

bool FTetrominoBase::CheckMino(FVector2D InSimulationPosition)
{
	if (!OnCheckMino.IsBound())
		return false;

	bool TetrominoCheck = false;

	for (FVector2D Coord : TetrominoInfo.Coordinate)
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
	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, false);
	}
}

void FTetrominoBase::SetTetromino()
{
	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}

void FTetrominoBase::SetTetrominoBackground()
{
	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnBackgroundCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.CurrentPosition.X, Coord.Y + TetrominoInfo.CurrentPosition.Y, TetrominoInfo.CurrentType);
	}
}
