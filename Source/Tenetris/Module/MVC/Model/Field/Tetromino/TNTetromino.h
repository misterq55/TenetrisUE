// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

DECLARE_DELEGATE_ThreeParams(FSetBackgroundCubeTypeDelegate, const int32, const int32, const E_TNTetrominoType);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FCheckMinoDelegate, const int32, const int32);
DECLARE_DELEGATE_RetVal_TwoParams(int32, FCalculateGuideMinoHeightDelegate, const int32, const int32);

DECLARE_DELEGATE(FHideTetromino)
DECLARE_DELEGATE(FShowTetromino)
DECLARE_DELEGATE(FHideGuideTetromino)
DECLARE_DELEGATE(FShowGuideTetromino)

class FTNTetromino
{
private:
	const TArray<TArray<FVector2D>> JLSTZOffset = {
		{FVector2D(0,0), FVector2D(0,0), FVector2D(0,0), FVector2D(0,0), FVector2D(0,0)} ,
		{FVector2D(0,0), FVector2D(1,0), FVector2D(1,-1), FVector2D(0,2), FVector2D(1,2)} ,
		{FVector2D(0,0), FVector2D(0,0), FVector2D(0,0), FVector2D(0,0), FVector2D(0,0)} ,
		{FVector2D(0,0), FVector2D(-1,0), FVector2D(-1,-1), FVector2D(0,0), FVector2D(-1,2)}
	};

	const TArray<TArray<FVector2D>> IOffset = {
		{FVector2D(0,0), FVector2D(-1,0), FVector2D(2,0), FVector2D(-1,0), FVector2D(2,0)} ,
		{FVector2D(-1,0), FVector2D(0,0), FVector2D(0,0), FVector2D(0,1), FVector2D(0,-2)} ,
		{FVector2D(-1,1), FVector2D(1,1), FVector2D(-2,1), FVector2D(1,0), FVector2D(-2,0)} ,
		{FVector2D(0,1), FVector2D(0,1), FVector2D(0,1), FVector2D(0,-1), FVector2D(0,2)}
	};

	const TArray<TArray<FVector2D>> OOffset = {
		{FVector2D(0,0)} ,
		{FVector2D(0,-1)} ,
		{FVector2D(-1,-1)} ,
		{FVector2D(-1,0)}
	};

public:
	FTNTetromino() {}
	FTNTetromino(TSharedPtr<FTNTetrominoInfo> tetrominoInfo)
	{
		TetrominoInfo = tetrominoInfo;
	}

	virtual ~FTNTetromino()
	{
		OnCheckMino.Unbind();
		OnCalculateGuideMino.Unbind();
	}

	virtual bool Move(const E_TNTetrominoDirection tetrominoDirection);
	virtual bool Rotate(const E_TNTetrominoRotation tetrominoRotation);
	virtual void LockDown();
	virtual void Spawn();
	virtual void HardDrop();
	virtual void ResetGuideTetromino()
	{
		hideGuideTetromino();
		showGuideTetromino();
	}

	TArray<int32> GetMinoHeights() const;
	void SetTetrominoPosition(const int32 x, const int32 y) const;
	void SetTetrominoType(const E_TNTetrominoType currentTetrominoType) const;
	E_TNTetrominoType GetTetrominoType() const;
	void SetStartingLocation(const int32 x, const int32 y);
	void SetStartingLocation(const FVector2D& startingLocation);
	FVector2D GetStaringLocation() const;
	TSharedPtr<FTNTetrominoInfo> GetTetrominoInfo() const;

private:
	bool checkMino(const FVector2D& simulationPosition) const;
	void showTetromino() const;
	void moveTetrominoToCheckBuffer() const;
	void hideGuideTetromino();
	void showGuideTetromino();
	void hideTetromino() const;
	FVector2D simulatePosition(const E_TNTetrominoDirection tetrominoDirection) const;
	int32 mod(int32 n, int32 m) { return ((n % m) + m) % m; }

public:
	FSetBackgroundCubeTypeDelegate OnMoveTetrominoToCheckBuffer;
	FCheckMinoDelegate OnCheckMino;
	FCalculateGuideMinoHeightDelegate OnCalculateGuideMino;

	FHideTetromino OnHideTetromino;
	FShowTetromino OnShowTetromino;
	FHideGuideTetromino OnHideGuideTetromino;
	FShowGuideTetromino OnShowGuideTetromino;

protected:
	TSharedPtr<FTNTetrominoInfo> TetrominoInfo;
	FVector2D StartingLocation;
};
