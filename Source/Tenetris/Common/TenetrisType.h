#pragma once

#include "Tenetris/Common/TenetrisConstant.h"
#include "Tenetris/Common/TenetrisEnum.h"

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
		
	}

	FTNTetrominoInfo(E_TNTetrominoType currentTetrominoType)
		: CurrentType(currentTetrominoType)
		, CurrentPosition(FVector2D(1, 1))
		, RotationState(0)
	{
		
	}

	void SetPosition(int32 x, int32 y)
	{
		CurrentPosition = FVector2D(x, y);
	}

	E_TNTetrominoType CurrentType;
	FVector2D CurrentPosition;
	FVector2D GuideTetrominoPosition;
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

struct FTNFieldContext
{
	public:
	FTNFieldContext(E_TNFieldType fieldType)
		: FieldType(fieldType)
		, BufferHeight(RowMax)
		, BufferWidth(ColumnMax)
	{
		initializeTetrominoInfos();
		initializeLockedGrid(LockedGrid);
	}

	E_TNFieldType FieldType;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	bool bSpaceInverted = false;
	TArray<TArray<E_TNTetrominoType>> LockedGrid;
	TSharedPtr<FTNTetrominoInfo> PlayerTetrominoInfo;
	int32 PreviewTetrominoNum;
	E_TNTetrominoType HoldTetrominoType = E_TNTetrominoType::None;
	TArray<E_TNTetrominoType> PreviewTetrominoTypes;

	void initializeTetrominoInfos()
	{
		PlayerTetrominoInfo = MakeShareable(new FTNTetrominoInfo());
		PreviewTetrominoNum = PreviewTetrominoMax;
		PreviewTetrominoTypes.Init(E_TNTetrominoType::None, PreviewTetrominoNum);
	}
	
	void initializeLockedGrid(TArray<TArray<E_TNTetrominoType>>& grid) const
	{
		for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
		{
			TArray<E_TNTetrominoType> row;
			row.Reserve(BufferWidth + 2);
			for (int32 j = 0; j < BufferWidth + 2; j++)
			{
				if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				{
					row.Add(E_TNTetrominoType::Obstacle);
				}
				else
				{
					row.Add(E_TNTetrominoType::None);
				}
			}

			grid.Add(MoveTemp(row));
		}
	}
};

