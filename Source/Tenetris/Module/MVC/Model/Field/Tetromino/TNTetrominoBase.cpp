#include "TNTetrominoBase.h"

TArray<int32> FTNTetrominoBase::GetMinoHeights()
{
	if (!TetrominoInfo.IsValid())
	{
		return TArray<int32>();
	}

	TArray<int32> result;

	for (const FVector2D& coord : TetrominoInfo->Coordinate)
	{
		result.AddUnique(coord.Y + TetrominoInfo->CurrentPosition.Y);
	}

	return result;
}

void FTNTetrominoBase::SetTetrominoPosition(const int32 x, const int32 y)
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->SetPosition(x, y);
}

void FTNTetrominoBase::SetTetrominoType(const E_TNTetrominoType currentTetrominoType)
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->CurrentType = currentTetrominoType;

	switch (TetrominoInfo->CurrentType)
	{
	case E_TNTetrominoType::I:
		TetrominoInfo->Coordinate = IMinoCoordinate;
		break;

	case E_TNTetrominoType::J:
		TetrominoInfo->Coordinate = JMinoCoordinate;
		break;

	case E_TNTetrominoType::L:
		TetrominoInfo->Coordinate = LMinoCoordinate;
		break;

	case E_TNTetrominoType::O:
		TetrominoInfo->Coordinate = OMinoCoordinate;
		break;

	case E_TNTetrominoType::S:
		TetrominoInfo->Coordinate = SMinoCoordinate;
		break;

	case E_TNTetrominoType::T:
		TetrominoInfo->Coordinate = TMinoCoordinate;
		break;

	case E_TNTetrominoType::Z:
		TetrominoInfo->Coordinate = ZMinoCoordinate;
		break;

	default:
		break;
	}
}

E_TNTetrominoType FTNTetrominoBase::GetTetrominoType()
{
	if (TetrominoInfo.IsValid())
	{
		return TetrominoInfo->CurrentType;
	}

	return E_TNTetrominoType();
}

void FTNTetrominoBase::SetStartingLocation(const int32 x, const int32 y)
{
	StartingLocation = FVector2D(x, y);
}

void FTNTetrominoBase::SetStartingLocation(const FVector2D& startingLocation)
{
	StartingLocation = startingLocation;
}

FVector2D FTNTetrominoBase::GetStaringLocation()
{
	return StartingLocation;
}

void FTNTetrominoBase::Spawn()
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->SetPosition(StartingLocation.X, StartingLocation.Y);
	TetrominoInfo->RotationState = 0;
	setTetromino();
}

bool FTNTetrominoBase::checkMino(const FVector2D& simulationPosition)
{
	if (!OnCheckMino.IsBound())
		return false;

	if (!TetrominoInfo.IsValid())
	{
		return false;
	}

	bool tetrominoCheck = false;

	for (const FVector2D& coord : TetrominoInfo->Coordinate)
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
	OnHideTetromino.ExecuteIfBound();
}

TSharedPtr<FTNTetrominoInfo> FTNTetrominoBase::GetTetrominoInfo()
{
	return TetrominoInfo;
}

void FTNTetrominoBase::setTetromino()
{
	OnSetTetromino.ExecuteIfBound();
}

void FTNTetrominoBase::setTetrominoBackground()
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	for (const FVector2D& coord : TetrominoInfo->Coordinate)
	{
		OnBackgroundCubeType.ExecuteIfBound(coord.X + TetrominoInfo->CurrentPosition.X, coord.Y + TetrominoInfo->CurrentPosition.Y, TetrominoInfo->CurrentType);
	}
}
