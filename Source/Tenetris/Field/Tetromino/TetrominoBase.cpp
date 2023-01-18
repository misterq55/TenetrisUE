#include "TetrominoBase.h"

void FTetrominoBase::SetTetrominoPosition(int32 X, int32 Y)
{
	TetrominoInfo.SetPosition(X, Y);

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			OnTetrominoCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
		}
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

void FTetrominoBase::Spawn()
{
	TetrominoInfo.SetPosition(StartingLocation.X, StartingLocation.Y);

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			OnTetrominoCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
		}
	}
}

bool FTetrominoBase::CheckTetrominoCube(FVector2D InSimulationPosition)
{
	if (!OnCheckTetrominoCube.IsBound())
		return false;

	bool TetrominoCheck = false;

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			if (OnCheckTetrominoCube.Execute(Coord.X + InSimulationPosition.X, Coord.Y + InSimulationPosition.Y))
			{
				TetrominoCheck = true;
				break;
			}
		}
	}

	return TetrominoCheck;
}

void FTetrominoBase::HideTetromino()
{
	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			OnVisibilityTetrominoCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, false);
		}
	}
}

void FTetrominoBase::SetTetromino()
{
	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			OnTetrominoCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
		}
	}
}

void FTetrominoBase::SetTetrominoBackground()
{
	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			OnBackgroundCubeType.ExecuteIfBound(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
		}
	}
}
