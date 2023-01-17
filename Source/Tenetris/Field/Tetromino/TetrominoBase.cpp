#include "TetrominoBase.h"
#include "Tenetris/Field/TenetrisFieldBase.h"

void FTetrominoBase::Move(ETetrominoDirection InTetrominoDirection)
{
	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			CurrentTenetrisField->SetVisibilityTetrominoCube(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, false);
		}
	}

	switch (InTetrominoDirection)
	{
	case ETetrominoDirection::Down:
		TetrominoInfo.TetrominoCurrentPosition.Y -= 1;
		break;

	case ETetrominoDirection::Left:
		TetrominoInfo.TetrominoCurrentPosition.X -= 1;
		break;

	case ETetrominoDirection::Right:
		TetrominoInfo.TetrominoCurrentPosition.X += 1;
		break;
	};

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			CurrentTenetrisField->SetTetrominoCubeType(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
		}
	}
}

void FTetrominoBase::Rotate(ETetrominoRotation InTetrominoRotation)
{
}

void FTetrominoBase::SetTetrominoPosition(int32 X, int32 Y)
{
	TetrominoInfo.SetPosition(X, Y);

	if (CurrentTenetrisField)
	{
		for (FVector2D Coord : TetrominoInfo.TetrominoCoordinate)
		{
			CurrentTenetrisField->SetTetrominoCubeType(Coord.X + TetrominoInfo.TetrominoCurrentPosition.X, Coord.Y + TetrominoInfo.TetrominoCurrentPosition.Y, TetrominoInfo.CurrentTetrominoType);
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
