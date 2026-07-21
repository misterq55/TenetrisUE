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
	FTNTetrominoInfo() = default;

	int32 Id = -1;
	E_TNTetrominoType TetrominoType = E_TNTetrominoType::None;
	FVector2D Position = FVector2D::ZeroVector;
	FVector2D GuideTetrominoPosition = FVector2D::ZeroVector;
	int32 RotationState = 0;
};

struct FTNMoveDirectionState
{
	public:
	FTNMoveDirectionState() = default;

	bool Pressed = false;
	float PressedTime = 0.f;
	bool AutoRepeatKickIn = false;
	float AutoRepeatTime = 0.f;
};

struct FTNCellInfo
{
	public:
	FTNCellInfo() = default;
	
	FTNCellInfo(int32 id, E_TNTetrominoType type)
		: Id(id)
		, Type(type)
	{
		
	}
	
	int32 Id = -1;
	E_TNTetrominoType Type = E_TNTetrominoType::None;
};

struct FTNFieldContext
{
	public:
	FTNFieldContext(E_TNFieldType fieldType)
		: FieldType(fieldType)
	{
		initializeTetrominoInfos();
	}

	E_TNFieldType FieldType = E_TNFieldType::None;
	bool bSpaceInverted = false;
	TArray<TArray<FTNCellInfo>> LockedGrid;
	TSharedPtr<FTNTetrominoInfo> PlayerTetrominoInfo;
	int32 PreviewTetrominoNum = 0;
	E_TNTetrominoType HoldTetrominoType = E_TNTetrominoType::None;
	TArray<E_TNTetrominoType> PreviewTetrominoTypes;

	void initializeTetrominoInfos()
	{
		PlayerTetrominoInfo = MakeShareable(new FTNTetrominoInfo());
		PreviewTetrominoNum = PreviewTetrominoMax;
		PreviewTetrominoTypes.Init(E_TNTetrominoType::None, PreviewTetrominoNum);
	}
};

struct FTNBehavior
{
	FTNBehavior() = default;
	~FTNBehavior() = default;

	E_TNBehaviorState BehaviorState = E_TNBehaviorState::None;
	FVector2D Position = FVector2D::ZeroVector;
	bool bRotateField = false;
	E_TNTetrominoType HoldTetrominoType = E_TNTetrominoType::None;
	bool bCanHold = false;
	int32 RotationState = 0;
};
