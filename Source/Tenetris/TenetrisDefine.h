// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

const int32 RowMax = 20;
const int32 ColumnMax = 10;

const float MinoRatio = 0.25f;

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
	Enemy
};

struct FTNFieldContext
{
public:
	FTNFieldContext(E_TNFieldType fieldType)
		: FieldType(fieldType)
		, BufferHeight(ColumnMax)
		, BufferWidth(RowMax)
	{
		createBuffer();
	}

	FTNFieldContext(E_TNFieldType fieldType, TArray<TArray<E_TNTetrominoType>> initialBuffer, int32 height = ColumnMax, int32 width = RowMax)
		: FieldType(fieldType)
		, BufferHeight(height)
		, BufferWidth(width)
		, CheckBuffer(MoveTemp(initialBuffer))
	{}

	E_TNFieldType FieldType;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	TArray<TArray<E_TNTetrominoType>> CheckBuffer;

	void createBuffer()
	{
		for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
		{
			TArray<E_TNTetrominoType> buffer;
			for (int32 j = 0; j < BufferWidth + 2; j++)
			{
				if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				{
					buffer.Add(E_TNTetrominoType::Obstacle);
				}
				else
				{
					buffer.Add(E_TNTetrominoType::None);
				}
			}

			CheckBuffer.Add(buffer);
		}
	}
};

struct FTNMoveDirectionState
{
public:
	FTNMoveDirectionState()
		: Pressed(false)
		, PressedTime(0.f)
		, AutoRepeatKickIn(false)
		, AutoRepeatTime(0.f)
	{}

	bool Pressed;
	float PressedTime;
	bool AutoRepeatKickIn;
	float AutoRepeatTime;
};

class FTNLockDown
{
public:
	FTNLockDown()
	{
		LockDownRemainCount = MaxLockDownRemainCount;
	}

	~FTNLockDown() {}
	void StartLockDown()
	{
		LockDownStart = true;
	}

	bool UpdateLockDown(float deltaTime)
	{
		if (LockDownStart)
		{
			LockDownTime += deltaTime;

			if (LockDownTime >= LockDownDelay || LockDownRemainCount <= 0)
			{
				LockDownTime = 0.f;
				LockDownStart = false;
				LockDownRemainCount = MaxLockDownRemainCount;

				return true;
			}
		}

		return false;
	}

	void CheckRemainCount(E_TNTetrominoDirection tetrominoDirection)
	{
		if (!LockDownStart)
		{
			return;
		}

		if (tetrominoDirection == E_TNTetrominoDirection::Left
			|| tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			LockDownRemainCount--;
		}
		else if (tetrominoDirection == E_TNTetrominoDirection::Down)
		{
			LockDownRemainCount = MaxLockDownRemainCount;
			LockDownStart = false;
		}

		LockDownTime = 0.f;
	}

	void CheckRemainCount()
	{
		if (!LockDownStart)
		{
			return;
		}

		LockDownRemainCount--;
		LockDownTime = 0.f;
	}

private:
	float LockDownDelay = 0.5f;
	float LockDownTime = 0.f;
	bool LockDownStart = false;
	const int32 MaxLockDownRemainCount = 15;
	int32 LockDownRemainCount = 0;
};