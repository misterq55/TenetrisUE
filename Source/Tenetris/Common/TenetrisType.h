#pragma once

#include "Tenetris/Common/TenetrisConstant.h"
#include "Tenetris/Common/TenetrisEnum.h"

using TTetrominoCoordinate = TArray<FVector2D>;

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
		: Id(-1)
		, CurrentType(E_TNTetrominoType::None)
		, CurrentPosition(FVector2D(1, 1))
		, RotationState(0)
	{
		
	}

	void SetPosition(int32 x, int32 y)
	{
		CurrentPosition = FVector2D(x, y);
	}

	int32 Id;
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

struct FTNCellInfo
{
	public:
	FTNCellInfo()
		: Id(-1)
		, Type(E_TNTetrominoType::None)
	{
		
	}
	
	FTNCellInfo(int32 id, E_TNTetrominoType type)
		: Id(id)
		, Type(type)
	{
		
	}
	
	int32 Id;
	E_TNTetrominoType Type;
};

struct FTNFieldContext
{
	public:
	FTNFieldContext(E_TNFieldType fieldType)
		: FieldType(fieldType)
	{
		initializeTetrominoInfos();
	}

	E_TNFieldType FieldType;
	bool bSpaceInverted = false;
	TArray<TArray<FTNCellInfo>> LockedGrid;
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
};

