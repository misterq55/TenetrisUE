// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

DECLARE_DELEGATE_ThreeParams(FSetBackgroundCubeTypeDelegate, int32, int32, ETetrominoType);
DECLARE_DELEGATE_ThreeParams(FSetVisibilityBackgroundCubeTypeDelegate, int32, int32, bool);
DECLARE_DELEGATE_ThreeParams(FSetTetrominoCubeTypeDelegate, int32, int32, ETetrominoType);
DECLARE_DELEGATE_ThreeParams(FSetVisibilityTetrominoCubeTypeDelegate, int32, int32, bool);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FCheckTetrominoCubeDelegate, int32, int32);

class ATenetrisFieldBase;

class FTetrominoBase
{
	typedef TArray<FVector2D> TTetrominoCoordinate;
	
	const TTetrominoCoordinate IMinoCoordinate = { FVector2D(0.f, 0.f) , FVector2D(1.f, 0.f), FVector2D(2.f, 0.f), FVector2D(3.f, 0.f) };
	const TTetrominoCoordinate JMinoCoordinate = { FVector2D(0.f, 0.f) , FVector2D(0.f, 1.f), FVector2D(1.f, 0.f), FVector2D(2.f, 0.f) };
	const TTetrominoCoordinate LMinoCoordinate = { FVector2D(0.f, 0.f) , FVector2D(2.f, 1.f), FVector2D(1.f, 0.f), FVector2D(2.f, 0.f) };
	const TTetrominoCoordinate OMinoCoordinate = { FVector2D(1.f, 0.f) , FVector2D(1.f, 1.f), FVector2D(2.f, 0.f), FVector2D(2.f, 1.f) };
	const TTetrominoCoordinate SMinoCoordinate = { FVector2D(0.f, 0.f) , FVector2D(1.f, 0.f), FVector2D(1.f, 1.f), FVector2D(2.f, 1.f) };
	const TTetrominoCoordinate TMinoCoordinate = { FVector2D(1.f, 1.f) , FVector2D(0.f, 0.f), FVector2D(1.f, 0.f), FVector2D(2.f, 0.f) };
	const TTetrominoCoordinate ZMinoCoordinate = { FVector2D(0.f, 1.f) , FVector2D(1.f, 0.f), FVector2D(1.f, 1.f), FVector2D(2.f, 0.f) };
	
	typedef struct FTetrominoInfo
	{
	public:
		FTetrominoInfo()
			: CurrentTetrominoType(ETetrominoType::None)
			, TetrominoCurrentPosition(FVector2D())
		{}

		FTetrominoInfo(ETetrominoType InCurrentTetrominoType)
			: CurrentTetrominoType(InCurrentTetrominoType)
			, TetrominoCurrentPosition(FVector2D())
		{}

		void SetPosition(int32 X, int32 Y)
		{
			TetrominoCurrentPosition = FVector2D(X, Y);
		}

		void SetType(ETetrominoType InCurrentTetrominoType);
		
		ETetrominoType CurrentTetrominoType;
		TTetrominoCoordinate TetrominoCoordinate;
		FVector2D TetrominoCurrentPosition;
		TArray<TTetrominoCoordinate> History;
	}FTetrominoInfo;

public:
	FTetrominoBase() 
		: CurrentTenetrisField(nullptr)
	{}

	FTetrominoBase(ATenetrisFieldBase* InCurrentTenetrisField)
		: CurrentTenetrisField(InCurrentTenetrisField)
	{}

	virtual ~FTetrominoBase() {}
	virtual bool Move(ETetrominoDirection InTetrominoDirection) { return true; }
	virtual bool Rotate(ETetrominoRotation InTetrominoRotation) { return true; }
	virtual void LockDown() {}
	
	void SetTenetrisField(ATenetrisFieldBase* InTenetrisField)
	{
		CurrentTenetrisField = InTenetrisField;
	}

	void SetTetrominoPosition(int32 X, int32 Y);
	void SetTetrominoType(ETetrominoType InCurrentTetrominoType);

	void SetStartingLocation(int32 X, int32 Y)
	{
		StartingLocation = FVector2D(X, Y);
	}

	FVector2D GetStaringLocation()
	{
		return StartingLocation;
	}

	void Spawn();

protected:
	bool CheckTetrominoCube(FVector2D InSimulationPosition);
	void HideTetromino();
	void SetTetromino();
	void SetTetrominoBackground();
	
public:
	FSetBackgroundCubeTypeDelegate OnBackgroundCubeType;
	FSetVisibilityBackgroundCubeTypeDelegate OnVisibilityBackgroundCubeType;
	FSetTetrominoCubeTypeDelegate OnTetrominoCubeType;
	FSetVisibilityTetrominoCubeTypeDelegate OnVisibilityTetrominoCubeType;
	FCheckTetrominoCubeDelegate OnCheckTetrominoCube;

protected:
	ATenetrisFieldBase* CurrentTenetrisField;
	FTetrominoInfo TetrominoInfo;
	FTetrominoInfo PrevTetrominoInfo;
	FVector2D StartingLocation;
};