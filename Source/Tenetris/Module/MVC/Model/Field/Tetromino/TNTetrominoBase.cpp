#include "TNTetrominoBase.h"

TArray<int32> FTNTetrominoBase::GetMinoHeights() const
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

void FTNTetrominoBase::SetTetrominoPosition(const int32 x, const int32 y) const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->SetPosition(x, y);
}

void FTNTetrominoBase::SetTetrominoType(const E_TNTetrominoType currentTetrominoType) const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->ApplyTetrominoType(currentTetrominoType);
}

E_TNTetrominoType FTNTetrominoBase::GetTetrominoType() const
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

FVector2D FTNTetrominoBase::GetStaringLocation() const
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
	showTetromino();
}

bool FTNTetrominoBase::checkMino(const FVector2D& simulationPosition) const
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

void FTNTetrominoBase::HideTetromino() const
{
	OnHideTetromino.ExecuteIfBound();
}

TSharedPtr<FTNTetrominoInfo> FTNTetrominoBase::GetTetrominoInfo() const
{
	return TetrominoInfo;
}

void FTNTetrominoBase::showTetromino() const
{
	OnShowTetromino.ExecuteIfBound();
}

void FTNTetrominoBase::moveTetrominoToCheckBuffer() const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	for (const FVector2D& coord : TetrominoInfo->Coordinate)
	{
		OnMoveTetrominoToCheckBuffer.ExecuteIfBound(coord.X + TetrominoInfo->CurrentPosition.X, coord.Y + TetrominoInfo->CurrentPosition.Y, TetrominoInfo->CurrentType);
	}
}
