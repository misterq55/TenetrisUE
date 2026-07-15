#pragma once

#include "CoreMinimal.h"

enum class E_TNTetrominoType : uint32
{
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
	Guide,
	Obstacle,
	None,
};

enum class E_TNTetrominoDirection : uint32
{
	Down,
	Left,
	Right,
	None,
};

enum class E_TNTetrominoRotation : uint32
{
	ClockWise,
	CounterClockWise,
};

enum class E_TNTetrominoBufferDataType : uint32
{
	Blank,
	Tetromino,
	Item,
};

enum class E_TNFieldType : uint32
{
	Player,
	Enemy,
	None,
};

enum class E_TNFieldModelStateType : uint32
{
	UpdateTetromino,
	UpdateHoldTetromino,
	UpdatePreviewTetrominoes,
	LockDown,
	RotateField,
};

