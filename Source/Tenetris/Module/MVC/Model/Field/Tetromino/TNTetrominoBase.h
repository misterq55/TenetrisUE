// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

DECLARE_DELEGATE_ThreeParams(FSetBackgroundCubeTypeDelegate, const int32, const int32, const E_TNTetrominoType);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FCheckMinoDelegate, const int32, const int32);
DECLARE_DELEGATE_RetVal_TwoParams(int32, FCalculateGuideMinoHeightDelegate, const int32, const int32);

DECLARE_DELEGATE(FHideTetromino)
DECLARE_DELEGATE(FSetTetromino)

class ATNFieldBase;

class FTNTetrominoBase
{
public:
	FTNTetrominoBase() {}
	FTNTetrominoBase(TSharedPtr<FTNTetrominoInfo> tetrominoInfo) 
	{
		TetrominoInfo = tetrominoInfo;
	}

	virtual ~FTNTetrominoBase() 
	{
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
	E_TNTetrominoType GetTetrominoType();
	void SetStartingLocation(const int32 x, const int32 y);
	void SetStartingLocation(const FVector2D& startingLocation);
	FVector2D GetStaringLocation();
	void HideTetromino();
	TSharedPtr<FTNTetrominoInfo> GetTetrominoInfo();

protected:
	bool checkMino(const FVector2D& simulationPosition);
	void setTetromino();
	void setTetrominoBackground();
	
public:
	FSetBackgroundCubeTypeDelegate OnBackgroundCubeType;
	FCheckMinoDelegate OnCheckMino;
	FCalculateGuideMinoHeightDelegate OnCalulateGuideMino;

	FHideTetromino OnHideTetromino;
	FSetTetromino OnSetTetromino;

protected:
	TSharedPtr<FTNTetrominoInfo> TetrominoInfo;
	FVector2D StartingLocation;
};