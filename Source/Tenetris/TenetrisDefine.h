// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

constexpr int32 RowMax = 20;
constexpr int32 ColumnMax = 10;
constexpr int32 PreviewTetrominoMax = 5;

constexpr float MinoRatio = 0.25f;
constexpr float RotationDuration = 1.f;

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

enum class E_TNFieldModelStateType : uint32
{
	UpdateTetromino,
	UpdateHoldTetromino,
	UpdatePreviewTetrominoes,
	LockDown,
	RotateField,
};

typedef TArray<FVector2D> TTetrominoCoordinate;

const TTetrominoCoordinate IMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(0.f, 0.f), FVector2D(1.f, 0.f), FVector2D(2.f, 0.f) };
const TTetrominoCoordinate JMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(-1.f, 1.f), FVector2D(0.f, 0.f), FVector2D(1.f, 0.f) };
const TTetrominoCoordinate LMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(1.f, 1.f), FVector2D(0.f, 0.f), FVector2D(1.f, 0.f) };
const TTetrominoCoordinate OMinoCoordinate = { FVector2D(0.f, 0.f) , FVector2D(0.f, 1.f), FVector2D(1.f, 0.f), FVector2D(1.f, 1.f) };
const TTetrominoCoordinate SMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(0.f, 0.f), FVector2D(0.f, 1.f), FVector2D(1.f, 1.f) };
const TTetrominoCoordinate TMinoCoordinate = { FVector2D(0.f, 1.f) , FVector2D(-1.f, 0.f), FVector2D(0.f, 0.f), FVector2D(1.f, 0.f) };
const TTetrominoCoordinate ZMinoCoordinate = { FVector2D(-1.f, 1.f) , FVector2D(0.f, 0.f), FVector2D(0.f, 1.f), FVector2D(1.f, 0.f) };
const TTetrominoCoordinate TetrominoCoordinatesByType[] =
{
	IMinoCoordinate,
	JMinoCoordinate,
	LMinoCoordinate,
	OMinoCoordinate,
	SMinoCoordinate,
	TMinoCoordinate,
	ZMinoCoordinate,
};

struct FTNTetrominoInfo
{
public:
	FTNTetrominoInfo()
		: CurrentType(E_TNTetrominoType::None)
		, CurrentPosition(FVector2D(1, 1))
		, RotationState(0)
	{
		ApplyTetrominoType(CurrentType);
	}

	FTNTetrominoInfo(E_TNTetrominoType currentTetrominoType)
		: CurrentType(currentTetrominoType)
		, CurrentPosition(FVector2D(1, 1))
		, RotationState(0)
	{
		ApplyTetrominoType(CurrentType);
	}

	void ApplyTetrominoType(const E_TNTetrominoType tetrominoType)
	{
		CurrentType = tetrominoType;

		const uint32 typeIndex = static_cast<uint32>(CurrentType);
		if (typeIndex < UE_ARRAY_COUNT(TetrominoCoordinatesByType))
		{
			Coordinate = TetrominoCoordinatesByType[typeIndex];
			return;
		}

		Coordinate.Empty();
	}

	void SetPosition(int32 x, int32 y)
	{
		CurrentPosition = FVector2D(x, y);
	}

	E_TNTetrominoType CurrentType;
	TTetrominoCoordinate Coordinate;
	FVector2D CurrentPosition;
	FVector2D GuideTetrominoPosition;
	TArray<TTetrominoCoordinate> History;
	int32 RotationState;
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

struct FTNFieldContext
{
public:
	FTNFieldContext(E_TNFieldType fieldType)
		: FieldType(fieldType)
		, BufferHeight(RowMax)
		, BufferWidth(ColumnMax)
	{
		initializeTetrominoInfos();
		createBuffer(CheckBuffer);
		createBuffer(ReversedBuffer);
	}

	FTNFieldContext(E_TNFieldType fieldType, TArray<TArray<E_TNTetrominoType>> initialBuffer, int32 height = ColumnMax, int32 width = RowMax)
		: FieldType(fieldType)
		, BufferHeight(height)
		, BufferWidth(width)
		, CheckBuffer(MoveTemp(initialBuffer))
	{
		initializeTetrominoInfos();
		createBuffer(CheckBuffer);
		createBuffer(ReversedBuffer);
	}

	E_TNFieldType FieldType;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	bool bSpaceInverted = false;
	TArray<TArray<E_TNTetrominoType>> CheckBuffer;
	TArray<TArray<E_TNTetrominoType>> ReversedBuffer;
	TSharedPtr<FTNTetrominoInfo> PlayerTetrominoInfo;
	TSharedPtr<FTNTetrominoInfo> HoldTetrominoInfo;
	int32 PreviewTetrominoNum;
	TArray<TSharedPtr<FTNTetrominoInfo>> PreviewTetrominoInfos;

	void initializeTetrominoInfos()
	{
		PlayerTetrominoInfo = MakeShareable(new FTNTetrominoInfo());
		HoldTetrominoInfo = MakeShareable(new FTNTetrominoInfo());

		PreviewTetrominoNum = PreviewTetrominoMax;
		for (int32 i = 0; i < PreviewTetrominoNum; i++)
		{
			PreviewTetrominoInfos.Emplace(MakeShareable(new FTNTetrominoInfo()));
		}
	}

	void createBuffer(TArray<TArray<E_TNTetrominoType>>& checkBuffer) const
	{
		checkBuffer.Empty();
		checkBuffer.Reserve(BufferHeight * 2 + 2);
		
		for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
		{
			TArray<E_TNTetrominoType> buffer;
			buffer.Reserve(BufferWidth + 2);
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

			checkBuffer.Add(MoveTemp(buffer));
		}
	}
};