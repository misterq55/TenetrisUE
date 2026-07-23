#include "TNTetromino.h"

bool FTNTetromino::Move(const E_TNTetrominoDirection tetrominoDirection)
{
	if (!TetrominoInfo.IsValid())
	{
		return false;
	}

	const FVector2D& simulationPosition = simulatePosition(tetrominoDirection);

	if (!checkMino(simulationPosition))
	{
		TetrominoInfo->Position = simulationPosition;
		
		if (tetrominoDirection == E_TNTetrominoDirection::Left || tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			calculateGuideTetromino();
		}
		
		updateTetromino();

		return false;
	}

	return true;
}

bool FTNTetromino::Rotate(const E_TNTetrominoRotation tetrominoRotation)
{
	if (!TetrominoInfo.IsValid())
	{
		return false;
	}

	// Super Rotation System
	// 별도 클래스로 분리할 계획
	TArray<FVector2D> simulationCoordinates;
	TArray<FVector2D> clockwiseRotateMatrix = { FVector2D(0, -1), FVector2D(1, 0) };
	TArray<FVector2D> counterClockwiseRotateMatrix = { FVector2D(0, 1), FVector2D(-1, 0) };
	TArray<FVector2D> rotateMatrix;

	const int32 oldRotationState = TetrominoInfo->RotationState;

	if (tetrominoRotation == E_TNTetrominoRotation::ClockWise)
	{
		rotateMatrix = clockwiseRotateMatrix;
		TetrominoInfo->RotationState += 1;
	}
	else if (tetrominoRotation == E_TNTetrominoRotation::CounterClockWise)
	{
		rotateMatrix = counterClockwiseRotateMatrix;
		TetrominoInfo->RotationState -= 1;
	}

	TetrominoInfo->RotationState = mod(TetrominoInfo->RotationState, 4);
	
	for (const FVector2D& coord : Coordinate)
	{
		const int32 newX = (rotateMatrix[0].X * coord.X) + (rotateMatrix[1].X * coord.Y);
		const int32 newY = (rotateMatrix[0].Y * coord.X) + (rotateMatrix[1].Y * coord.Y);
	
		simulationCoordinates.Emplace(newX, newY);
	}

	TArray<TArray<FVector2D>> offset;

	if (TetrominoInfo->TetrominoType == E_TNTetrominoType::I)
	{
		offset = IOffset;
	}
	else if (TetrominoInfo->TetrominoType == E_TNTetrominoType::O)
	{
		offset = OOffset;
	}
	else
	{
		offset = JLSTZOffset;
	}

	const TArray<FVector2D>& oldOffset = offset[oldRotationState];
	const TArray<FVector2D>& newOffset = offset[TetrominoInfo->RotationState];

	// 킥 오프셋 구하기
	FVector2D kickOffset(0, 0);
	bool tetrominoCheck = true;
	for (int32 i = 0; i < newOffset.Num(); i++)
	{
		const FVector2D& difference = oldOffset[i] - newOffset[i];

		// CheckMino 실행
		bool simulationCheck = false;
		const FVector2D& simulationPosition = TetrominoInfo->Position + difference;
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
		TetrominoInfo->Position += kickOffset;
		Coordinate = simulationCoordinates;
		
		calculateGuideTetromino();
		updateTetromino();

		return false;
	}

	return true;
}

void FTNTetromino::LockDown()
{
	moveTetrominoToCheckBuffer();
}

void FTNTetromino::Spawn()
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->Position = StartingLocation;
	TetrominoInfo->RotationState = 0;
	TetrominoInfo->Id++;
	
	calculateGuideTetromino();
	updateTetromino();
}

void FTNTetromino::calculateGuideTetromino() const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	if (!OnCalculateGuideMino.IsBound())
	{
		return;
	}

	TArray<int32> checkHeightArray;

	for (const FVector2D& coord : Coordinate)
	{
		const int32 height = OnCalculateGuideMino.Execute(coord.X + TetrominoInfo->Position.X, coord.Y + TetrominoInfo->Position.Y);
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

	TetrominoInfo->GuideTetrominoPosition = FVector2D(TetrominoInfo->Position.X, TetrominoInfo->Position.Y - minHeight);

}

void FTNTetromino::HardDrop()
{	
	if (TetrominoInfo.IsValid())
	{
		TetrominoInfo->Position = TetrominoInfo->GuideTetrominoPosition;
	}
	
	calculateGuideTetromino();
	updateTetromino();
}

TArray<int32> FTNTetromino::GetMinoHeights() const
{
	if (!TetrominoInfo.IsValid())
	{
		return TArray<int32>();
	}

	TArray<int32> result;

	for (const FVector2D& coord : Coordinate)
	{
		result.AddUnique(coord.Y + TetrominoInfo->Position.Y);
	}

	return result;
}

void FTNTetromino::SetTetrominoPosition(const int32 x, const int32 y) const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->Position = FVector2D(x, y);
}

void FTNTetromino::ApplyTetrominoType(const E_TNTetrominoType currentTetrominoType)
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}

	TetrominoInfo->TetrominoType = currentTetrominoType;
	
	ResetCoordinate(currentTetrominoType);
}

void FTNTetromino::ResetCoordinate(const E_TNTetrominoType tetrominoType)
{
	const uint32 typeIndex = static_cast<uint32>(tetrominoType);
	if (typeIndex < UE_ARRAY_COUNT(TetrominoCoordinatesByType))
	{
		Coordinate = TetrominoCoordinatesByType[typeIndex];
	}
}

void FTNTetromino::SetTetrominoType(const E_TNTetrominoType tetrominoType) const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}
	
	TetrominoInfo->TetrominoType = tetrominoType;
}

void FTNTetromino::SetPosition(const FVector2D position) const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}
	
	TetrominoInfo->Position = position;
}

void FTNTetromino::SetRotationState(const int32 rotationState) const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}
	
	TetrominoInfo->RotationState = rotationState;
}

int32 FTNTetromino::GetId() const
{
	if (TetrominoInfo.IsValid())
	{
		return TetrominoInfo->Id;
	}

	return -1;
}

E_TNTetrominoType FTNTetromino::GetTetrominoType() const
{
	if (TetrominoInfo.IsValid())
	{
		return TetrominoInfo->TetrominoType;
	}

	return E_TNTetrominoType();
}

void FTNTetromino::SetStartingLocation(const int32 x, const int32 y)
{
	StartingLocation = FVector2D(x, y);
}

void FTNTetromino::SetStartingLocation(const FVector2D& startingLocation)
{
	StartingLocation = startingLocation;
}

FVector2D FTNTetromino::GetStaringLocation() const
{
	return StartingLocation;
}

void FTNTetromino::updateTetromino() const
{
	OnUpdateTetromino.ExecuteIfBound();
}

TSharedPtr<FTNTetrominoInfo> FTNTetromino::GetTetrominoInfo() const
{
	return TetrominoInfo;
}

bool FTNTetromino::checkMino(const FVector2D& simulationPosition) const
{
	if (!OnCheckMino.IsBound())
	{
		return false;
	}

	if (!TetrominoInfo.IsValid())
	{
		return false;
	}

	bool tetrominoCheck = false;

	for (const FVector2D& coord : Coordinate)
	{
		if (OnCheckMino.Execute(coord.X + simulationPosition.X, coord.Y + simulationPosition.Y))
		{
			tetrominoCheck = true;
			break;
		}
	}

	return tetrominoCheck;
}

void FTNTetromino::moveTetrominoToCheckBuffer() const
{
	if (!TetrominoInfo.IsValid())
	{
		return;
	}
	
	const FTNCellInfo cellInfo(TetrominoInfo->Id, TetrominoInfo->TetrominoType);
	
	for (const FVector2D& coord : Coordinate)
	{
		OnMoveTetrominoToCheckBuffer.ExecuteIfBound(coord.X + TetrominoInfo->Position.X, coord.Y + TetrominoInfo->Position.Y, cellInfo);
	}
}

FVector2D FTNTetromino::simulatePosition(const E_TNTetrominoDirection tetrominoDirection) const
{
	if (!TetrominoInfo.IsValid())
	{
		return FVector2D::ZeroVector;
	}

	FVector2D simulationPosition = TetrominoInfo->Position;

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

	default:
		break;
	}

	return MoveTemp(simulationPosition);
}
