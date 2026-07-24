// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

DECLARE_DELEGATE_ThreeParams(FUpdateCheckBufferDelegate, const int32, const int32, const FTNCellInfo&);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FCheckMinoDelegate, const int32, const int32);
DECLARE_DELEGATE_RetVal_TwoParams(int32, FCalculateGuideMinoHeightDelegate, const int32, const int32);

DECLARE_DELEGATE(FUpdateTetromino)

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
	virtual void ReverseLockDown();
	virtual void Spawn();
	virtual void Despawn();
	virtual void HardDrop();
	virtual void ResetGuideTetromino()
	{
		calculateGuideTetromino();
		updateTetromino();
	}

	TArray<int32> GetMinoHeights() const;
	void SetTetrominoPosition(const int32 x, const int32 y) const;
	void ApplyTetrominoType(const E_TNTetrominoType currentTetrominoType);
	void ApplyRotationState(const int32 rotationState);
	void ResetCoordinate(const E_TNTetrominoType tetrominoType);
	void SetTetrominoType(const E_TNTetrominoType tetrominoType) const;
	void SetPosition(const FVector2D position) const;
	void SetRotationState(const int32 rotationState) const;
	int32 GetId() const;
	E_TNTetrominoType GetTetrominoType() const;
	void SetStartingLocation(const int32 x, const int32 y);
	void SetStartingLocation(const FVector2D& startingLocation);
	FVector2D GetStaringLocation() const;
	TSharedPtr<FTNTetrominoInfo> GetTetrominoInfo() const;
	void SetShowGuideTetromino(const bool showGuideTetromino) const;

private:
	bool checkMino(const FVector2D& simulationPosition) const;
	void updateCheckBuffer(const FTNCellInfo& cellInfo) const;
	void calculateGuideTetromino() const;
	void updateTetromino() const;
	FVector2D simulatePosition(const E_TNTetrominoDirection tetrominoDirection) const;
	TTetrominoCoordinate calculateRotatedCoordinates(int32 newRotationState) const;
	static int32 mod(int32 n, int32 m) { return ((n % m) + m) % m; }

public:
	FUpdateCheckBufferDelegate OnUpdateCheckBuffer;
	FCheckMinoDelegate OnCheckMino;
	FCalculateGuideMinoHeightDelegate OnCalculateGuideMino;
	FUpdateTetromino OnUpdateTetromino;

private:
	TSharedPtr<FTNTetrominoInfo> TetrominoInfo;
	TTetrominoCoordinate Coordinate;
	FVector2D StartingLocation;
};
