// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

DECLARE_DELEGATE_ThreeParams(FSetBackgroundCubeTypeDelegate, int32, int32, ETetrominoType);
DECLARE_DELEGATE_ThreeParams(FSetVisibilityBackgroundCubeTypeDelegate, int32, int32, bool);
DECLARE_DELEGATE_ThreeParams(FSetMinoTypeDelegate, int32, int32, ETetrominoType);
DECLARE_DELEGATE_ThreeParams(FSetVisibilityMinoTypeDelegate, int32, int32, bool);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FCheckMinoDelegate, int32, int32);
DECLARE_DELEGATE_RetVal_TwoParams(int32, FCalculateGuideMinoHeightDelegate, int32, int32);


class AFieldBase;

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
		
		ETetrominoType CurrentTetrominoType;
		TTetrominoCoordinate TetrominoCoordinate;
		FVector2D TetrominoCurrentPosition;
		TArray<TTetrominoCoordinate> History;
	}FTetrominoInfo;

public:
	FTetrominoBase() {}

	virtual ~FTetrominoBase() 
	{
		OnBackgroundCubeType.Unbind();
		OnVisibilityBackgroundCubeType.Unbind();
		OnMinoType.Unbind();
		OnVisibilityMinoType.Unbind();
		OnCheckMino.Unbind();
		OnCalulateGuideMino.Unbind();
	}

	virtual bool Move(ETetrominoDirection InTetrominoDirection) { return true; }
	virtual bool Rotate(ETetrominoRotation InTetrominoRotation) { return true; }
	virtual void LockDown() {}
	virtual void Spawn();
	virtual void SetGuideTetromino() {}

	void SetTetrominoPosition(int32 X, int32 Y);
	void SetTetrominoType(ETetrominoType InCurrentTetrominoType);
	void SetStartingLocation(int32 X, int32 Y);
	FVector2D GetStaringLocation();
	void HideTetromino();

protected:
	bool CheckMino(FVector2D InSimulationPosition);
	void SetTetromino();
	void SetTetrominoBackground();
	
public:
	FSetBackgroundCubeTypeDelegate OnBackgroundCubeType;
	FSetVisibilityBackgroundCubeTypeDelegate OnVisibilityBackgroundCubeType;
	FSetMinoTypeDelegate OnMinoType;
	FSetVisibilityMinoTypeDelegate OnVisibilityMinoType;
	FCheckMinoDelegate OnCheckMino;
	FCalculateGuideMinoHeightDelegate OnCalulateGuideMino;

protected:
	FTetrominoInfo TetrominoInfo;
	FTetrominoInfo PrevTetrominoInfo;
	FVector2D StartingLocation;
};