// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/Tetromino/TetrominoBase.h"

class FRotationSystemBase;

class FPlayerTetromino : public FTetrominoBase
{
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
	FPlayerTetromino() 
		: FTetrominoBase()
		, GuideTetrominoPosition(FVector2D(0, 0))
	{}

	virtual ~FPlayerTetromino() {}
	virtual bool Move(ETetrominoDirection tetrominoDirection) override;
	virtual bool Rotate(ETetrominoRotation tetrominoRotation) override;
	virtual void LockDown() override;
	virtual void Spawn() override;
	virtual void SetGuideTetromino() override;
	virtual void HardDrop() override;
	virtual void HideGuideTetromino() override;

private:
	FVector2D simulatePosition(ETetrominoDirection tetrominoDirection);
	
	int32 Mod(int32 n, int32 m) { return ((n % m) + m) % m; }

private:
	FVector2D GuideTetrominoPosition;
	FRotationSystemBase* RotationSystem;
};