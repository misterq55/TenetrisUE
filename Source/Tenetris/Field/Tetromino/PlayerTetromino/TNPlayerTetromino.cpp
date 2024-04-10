#include "TNPlayerTetromino.h"

bool FTNPlayerTetromino::Move(E_TNTetrominoDirection tetrominoDirection)
{
	FVector2D SimulationPosition = simulatePosition(tetrominoDirection);

	if (!checkMino(SimulationPosition))
	{
		HideTetromino();
		
		TetrominoInfo.CurrentPosition = SimulationPosition;

		if (tetrominoDirection == E_TNTetrominoDirection::Left ||
			tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			HideGuideTetromino();
			SetGuideTetromino();
		}

		setTetromino();

		return false;
	}

	return true;
}

bool FTNPlayerTetromino::Rotate(E_TNTetrominoRotation tetrominoRotation)
{
	// Super Rotation System
	// 추후 따로 클래스로 분리할 계획
	TArray<FVector2D> SimulationCoordinates = TetrominoInfo.Coordinate;
	FVector2D RotateMatrix[2];

	int32 OldRotationState = TetrominoInfo.RotationState;

	if (tetrominoRotation == E_TNTetrominoRotation::ClockWise)
	{
		RotateMatrix[0] = FVector2D(0, -1);
		RotateMatrix[1] = FVector2D(1, 0);
		TetrominoInfo.RotationState += 1;
	}
	else if (tetrominoRotation == E_TNTetrominoRotation::CounterClockWise)
	{
		RotateMatrix[0] = FVector2D(0, 1);
		RotateMatrix[1] = FVector2D(-1, 0);
		TetrominoInfo.RotationState -= 1;
	}

	TetrominoInfo.RotationState = Mod(TetrominoInfo.RotationState, 4);

	for (FVector2D &Coord : SimulationCoordinates)
	{
		int32 NewX = (RotateMatrix[0].X * Coord.X) + (RotateMatrix[1].X * Coord.Y);
		int32 NewY = (RotateMatrix[0].Y * Coord.X) + (RotateMatrix[1].Y * Coord.Y);

		Coord = FVector2D(NewX, NewY);
	}

	TArray<TArray<FVector2D>> Offset;

	if (TetrominoInfo.CurrentType == E_TNTetrominoType::I)
	{
		Offset = IOffset;
	}
	else if (TetrominoInfo.CurrentType == E_TNTetrominoType::O)
	{
		Offset = OOffset;
	}
	else
	{
		Offset = JLSTZOffset;
	}

	TArray<FVector2D> OldOffset = Offset[OldRotationState];
	TArray<FVector2D> NewOffset = Offset[TetrominoInfo.RotationState];

	// 킥 오프셋 정하기
	FVector2D KickOffset(0, 0);
	bool TetrominoCheck = true;
	for (int32 i = 0; i < NewOffset.Num(); i++)
	{
		FVector2D Difference = OldOffset[i] - NewOffset[i];

		// CheckMino 변형
		bool SimulationCheck = false;
		FVector2D SimulationPosition = TetrominoInfo.CurrentPosition + Difference;
		for (FVector2D Coord : SimulationCoordinates)
		{
			if (OnCheckMino.Execute(Coord.X + SimulationPosition.X, Coord.Y + SimulationPosition.Y))
			{
				SimulationCheck = true;
				break;
			}
		}

		if (!SimulationCheck)
		{
			TetrominoCheck = false;
			KickOffset = Difference;
		}

		if (!TetrominoCheck)
			break;
	}

	if (!TetrominoCheck)
	{
		HideTetromino();
		HideGuideTetromino();

		TetrominoInfo.CurrentPosition += KickOffset;
		TetrominoInfo.Coordinate = SimulationCoordinates;

		SetGuideTetromino();
		setTetromino();

		return false;
	}

	return true;
}

void FTNPlayerTetromino::LockDown()
{
	HideGuideTetromino();
	HideTetromino();
	setTetrominoBackground();
}

void FTNPlayerTetromino::Spawn()
{
	FTNTetrominoBase::Spawn();
	SetGuideTetromino();
}

void FTNPlayerTetromino::SetGuideTetromino()
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
		OnMinoType.ExecuteIfBound(Coord.X + GuideTetrominoPosition.X, Coord.Y + GuideTetrominoPosition.Y, E_TNTetrominoType::Guide);
	}
}

void FTNPlayerTetromino::HardDrop()
{
	HideTetromino();
	TetrominoInfo.CurrentPosition = GuideTetrominoPosition;
	HideGuideTetromino();
	SetGuideTetromino();
	setTetromino();
}

FVector2D FTNPlayerTetromino::simulatePosition(E_TNTetrominoDirection tetrominoDirection)
{
	FVector2D simulationPosition = TetrominoInfo.CurrentPosition;
	switch (tetrominoDirection)
	{
	case E_TNTetrominoDirection::Down:
		simulationPosition.Y -= 1;
		break;

	case E_TNTetrominoDirection::Left:
		simulationPosition.X -= 1;
		break;

	case E_TNTetrominoDirection::Right:
		simulationPosition.X += 1;
		break;
	};

	return simulationPosition;
}

void FTNPlayerTetromino::HideGuideTetromino()
{
	for (FVector2D Coord : TetrominoInfo.Coordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(Coord.X + GuideTetrominoPosition.X, Coord.Y + GuideTetrominoPosition.Y, false);
	}
}
