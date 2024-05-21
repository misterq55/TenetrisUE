// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

DECLARE_DELEGATE_ThreeParams(FSetBackgroundCubeTypeDelegate, const int32, const int32, const E_TNTetrominoType);
DECLARE_DELEGATE_ThreeParams(FSetVisibilityBackgroundCubeTypeDelegate, const int32, const int32, const bool);
DECLARE_DELEGATE_ThreeParams(FSetMinoTypeDelegate, const int32, const int32, const E_TNTetrominoType);
DECLARE_DELEGATE_ThreeParams(FSetVisibilityMinoTypeDelegate, const int32, const int32, const bool);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FCheckMinoDelegate, const int32, const int32);
DECLARE_DELEGATE_RetVal_TwoParams(int32, FCalculateGuideMinoHeightDelegate, const int32, const int32);


class ATNFieldBase;

class FTNTetrominoBase
{
	typedef TArray<FVector2D> TTetrominoCoordinate;
	
	const TTetrominoCoordinate IMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(0.f, 0.f), FVector2D(1.f, 0.f), FVector2D(2.f, 0.f) };
	const TTetrominoCoordinate JMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(-1.f, 1.f), FVector2D(0.f, 0.f), FVector2D(1.f, 0.f) };
	const TTetrominoCoordinate LMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(1.f, 1.f), FVector2D(0.f, 0.f), FVector2D(1.f, 0.f) };
	const TTetrominoCoordinate OMinoCoordinate = { FVector2D(0.f, 0.f) , FVector2D(0.f, 1.f), FVector2D(1.f, 0.f), FVector2D(1.f, 1.f) };
	const TTetrominoCoordinate SMinoCoordinate = { FVector2D(-1.f, 0.f) , FVector2D(0.f, 0.f), FVector2D(0.f, 1.f), FVector2D(1.f, 1.f) };
	const TTetrominoCoordinate TMinoCoordinate = { FVector2D(0.f, 1.f) , FVector2D(-1.f, 0.f), FVector2D(0.f, 0.f), FVector2D(1.f, 0.f) };
	const TTetrominoCoordinate ZMinoCoordinate = { FVector2D(-1.f, 1.f) , FVector2D(0.f, 0.f), FVector2D(0.f, 1.f), FVector2D(1.f, 0.f) };

	struct FTNTetrominoInfo
	{
	public:
		FTNTetrominoInfo()
			: CurrentType(E_TNTetrominoType::None)
			, CurrentPosition(FVector2D(1, 1))
			, RotationState(0)
		{}

		FTNTetrominoInfo(E_TNTetrominoType currentTetrominoType)
			: CurrentType(currentTetrominoType)
			, CurrentPosition(FVector2D(1, 1))
			, RotationState(0)
		{}

		void SetPosition(int32 x, int32 y)
		{
			CurrentPosition = FVector2D(x, y);
		}
		
		E_TNTetrominoType CurrentType;
		TTetrominoCoordinate Coordinate;
		FVector2D CurrentPosition;
		TArray<TTetrominoCoordinate> History;
		int32 RotationState;
	};

public:
	FTNTetrominoBase() {}

	virtual ~FTNTetrominoBase() 
	{
		OnBackgroundCubeType.Unbind();
		OnVisibilityBackgroundCubeType.Unbind();
		OnMinoType.Unbind();
		OnVisibilityMinoType.Unbind();
		OnCheckMino.Unbind();
		OnCalulateGuideMino.Unbind();
	}

	virtual bool Move(const E_TNTetrominoDirection tetrominoDirection) { return true; }
	virtual bool Rotate(const E_TNTetrominoRotation tetrominoRotation) { return true; }
	virtual void LockDown() {}
	virtual void Spawn();
	virtual void SetGuideTetromino() {}
	virtual void HardDrop() {}
	virtual void HideGuideTetromino() {}
	virtual void ResetGuideTetromino()
	{
		HideGuideTetromino();
		SetGuideTetromino();
	}
	
	TArray<int32> GetMinoHeights();
	void SetTetrominoPosition(const int32 x, const int32 y);
	void SetTetrominoType(const E_TNTetrominoType currentTetrominoType);
	E_TNTetrominoType GetTetrominoType() { return TetrominoInfo.CurrentType; }
	void SetStartingLocation(const int32 x, const int32 y);
	void SetStartingLocation(const FVector2D& startingLocation);
	FVector2D GetStaringLocation();
	void HideTetromino();

protected:
	bool checkMino(const FVector2D& simulationPosition);
	void setTetromino();
	void setTetrominoBackground();
	
public:
	FSetBackgroundCubeTypeDelegate OnBackgroundCubeType;
	FSetVisibilityBackgroundCubeTypeDelegate OnVisibilityBackgroundCubeType;
	FSetMinoTypeDelegate OnMinoType;
	FSetVisibilityMinoTypeDelegate OnVisibilityMinoType;
	FCheckMinoDelegate OnCheckMino;
	FCalculateGuideMinoHeightDelegate OnCalulateGuideMino;

protected:
	FTNTetrominoInfo TetrominoInfo;
	FTNTetrominoInfo PrevTetrominoInfo;
	FVector2D StartingLocation;
};