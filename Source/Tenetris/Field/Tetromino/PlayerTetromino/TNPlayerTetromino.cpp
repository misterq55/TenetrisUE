#include "TNPlayerTetromino.h"

bool FTNPlayerTetromino::Move(const E_TNTetrominoDirection tetrominoDirection)
{
	const FVector2D& simulationPosition = simulatePosition(tetrominoDirection);

	if (!checkMino(simulationPosition))
	{
		HideTetromino();
		
		TetrominoInfo.CurrentPosition = simulationPosition;

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

bool FTNPlayerTetromino::Rotate(const E_TNTetrominoRotation tetrominoRotation)
{
	// Super Rotation System
	// 추후 따로 클래스로 분리할 계획
	TArray<FVector2D> simulationCoordinates;
	TArray<FVector2D> rotateMatrix;

	const int32 oldRotationState = TetrominoInfo.RotationState;

	if (tetrominoRotation == E_TNTetrominoRotation::ClockWise)
	{
		rotateMatrix.Emplace(0, -1);
		rotateMatrix.Emplace(1, 0);
		TetrominoInfo.RotationState += 1;
	}
	else if (tetrominoRotation == E_TNTetrominoRotation::CounterClockWise)
	{
		rotateMatrix.Emplace(0, 1);
		rotateMatrix.Emplace(-1, 0);
		TetrominoInfo.RotationState -= 1;
	}

	TetrominoInfo.RotationState = mod(TetrominoInfo.RotationState, 4);

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		const int32 newX = (rotateMatrix[0].X * coord.X) + (rotateMatrix[1].X * coord.Y);
		const int32 newY = (rotateMatrix[0].Y * coord.X) + (rotateMatrix[1].Y * coord.Y);

		simulationCoordinates.Emplace(newX, newY);
	}

	TArray<TArray<FVector2D>> offset;

	if (TetrominoInfo.CurrentType == E_TNTetrominoType::I)
	{
		offset = IOffset;
	}
	else if (TetrominoInfo.CurrentType == E_TNTetrominoType::O)
	{
		offset = OOffset;
	}
	else
	{
		offset = JLSTZOffset;
	}

	const TArray<FVector2D>& oldOffset = offset[oldRotationState];
	const TArray<FVector2D>& newOffset = offset[TetrominoInfo.RotationState];

	// 킥 오프셋 정하기
	FVector2D kickOffset(0, 0);
	bool tetrominoCheck = true;
	for (int32 i = 0; i < newOffset.Num(); i++)
	{
		const FVector2D& difference = oldOffset[i] - newOffset[i];

		// CheckMino 변형
		bool simulationCheck = false;
		const FVector2D& simulationPosition = TetrominoInfo.CurrentPosition + difference;
		for (const FVector2D& coord : simulationCoordinates)
		{
			if (OnCheckMino.Execute(coord.X + simulationPosition.X, coord.Y + simulationPosition.Y))
			{
				simulationCheck = true;
				break;
			}
		}

		if (!simulationCheck)
		{
			tetrominoCheck = false;
			kickOffset = difference;
		}

		if (!tetrominoCheck)
		{
			break;
		}
	}

	if (!tetrominoCheck)
	{
		HideTetromino();
		HideGuideTetromino();

		TetrominoInfo.CurrentPosition += kickOffset;
		TetrominoInfo.Coordinate = simulationCoordinates;

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
	{
		return;
	}

	TArray<int32> checkHeightArray;

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		const int32 height = OnCalulateGuideMino.Execute(coord.X + TetrominoInfo.CurrentPosition.X, coord.Y + TetrominoInfo.CurrentPosition.Y);
		checkHeightArray.Add(height);
	}

	int32 minHeight = MAX_int32;
	
	for (const int32& height : checkHeightArray)
	{
		if (minHeight > height)
		{
			minHeight = height;
		}
	}

	GuideTetrominoPosition = FVector2D(TetrominoInfo.CurrentPosition.X, TetrominoInfo.CurrentPosition.Y - minHeight);

	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnMinoType.ExecuteIfBound(coord.X + GuideTetrominoPosition.X, coord.Y + GuideTetrominoPosition.Y, E_TNTetrominoType::Guide);
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

FVector2D FTNPlayerTetromino::simulatePosition(const E_TNTetrominoDirection tetrominoDirection)
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

	return MoveTemp(simulationPosition);
}

void FTNPlayerTetromino::HideGuideTetromino()
{
	for (const FVector2D& coord : TetrominoInfo.Coordinate)
	{
		OnVisibilityMinoType.ExecuteIfBound(coord.X + GuideTetrominoPosition.X, coord.Y + GuideTetrominoPosition.Y, false);
	}
}
