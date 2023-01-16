#include "TetrominoBase.h"
#include "Tenetris/Field/TenetrisFieldBase.h"

void FTetrominoBase::Move(ETetrominoDirection InTetrominoDirection)
{
	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoCoordinate)
		{
			CurrentTenetrisField->SetVisibilityTetrominoCube(Coord.X + TetrominoCurrentPosition.X, Coord.Y + TetrominoCurrentPosition.Y, false);
		}
	}

	switch (InTetrominoDirection)
	{
	case ETetrominoDirection::Down:
		TetrominoCurrentPosition.Y -= 1;
		break;

	case ETetrominoDirection::Left:
		TetrominoCurrentPosition.X -= 1;
		break;

	case ETetrominoDirection::Right:
		TetrominoCurrentPosition.X += 1;
		break;
	};

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoCoordinate)
		{
			CurrentTenetrisField->SetTetrominoCubeType(Coord.X + TetrominoCurrentPosition.X, Coord.Y + TetrominoCurrentPosition.Y, CurrentTetrominoType);
		}
	}
}

void FTetrominoBase::Rotate(ETetrominoRotation InTetrominoRotation)
{
}

void FTetrominoBase::SetTetrominoPosition(int32 X, int32 Y)
{
	TetrominoCurrentPosition = FVector2D(X, Y);

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoCoordinate)
			CurrentTenetrisField->SetTetrominoCubeType(Coord.X + TetrominoCurrentPosition.X, Coord.Y + TetrominoCurrentPosition.Y, CurrentTetrominoType);
	}
}
