#include "PlayerTetromino.h"

// 이 함수를 필드로 올릴지 고민
// Lock Down은 필드의 영역인가, 테트로미노의 영역인가?
bool FPlayerTetromino::Move(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = SimulatePosition(InTetrominoDirection);

	if (!CheckTetrominoCube(SimulationPosition))
	{
		HideTetromino();
		TetrominoInfo.TetrominoCurrentPosition = SimulationPosition;
		SetTetromino();

		return false;
	}
	
	/*if (InTetrominoDirection == ETetrominoDirection::Down)
		LockDown();*/

	return true;
}

bool FPlayerTetromino::Rotate(ETetrominoRotation InTetrominoRotation)
{
	return true;
}

void FPlayerTetromino::LockDown()
{
	HideTetromino();
	SetTetrominoBackground();
	// Spawn();
}

FVector2D FPlayerTetromino::SimulatePosition(ETetrominoDirection InTetrominoDirection)
{
	FVector2D SimulationPosition = TetrominoInfo.TetrominoCurrentPosition;
	switch (InTetrominoDirection)
	{
	case ETetrominoDirection::Down:
		SimulationPosition.Y -= 1;
		break;

	case ETetrominoDirection::Left:
		SimulationPosition.X -= 1;
		break;

	case ETetrominoDirection::Right:
		SimulationPosition.X += 1;
		break;
	};

	return SimulationPosition;
}
