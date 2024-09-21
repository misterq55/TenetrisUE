// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetrominoBase.h"

DECLARE_DELEGATE(FHideGuideTetromino)
DECLARE_DELEGATE(FSetGuideTetromino)

class FTNPlayerTetromino : public FTNTetrominoBase
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
	FTNPlayerTetromino() 
		: FTNTetrominoBase()
	{}

	FTNPlayerTetromino(TSharedPtr<FTNTetrominoInfo> tetrominoInfo)
		: FTNTetrominoBase(tetrominoInfo)
	{}

	virtual ~FTNPlayerTetromino() {}
	virtual bool Move(const E_TNTetrominoDirection tetrominoDirection) override;
	virtual bool Rotate(const E_TNTetrominoRotation tetrominoRotation) override;
	virtual void LockDown() override;
	virtual void Spawn() override;
	virtual void SetGuideTetromino() override;
	virtual void HardDrop() override;
	virtual void HideGuideTetromino() override;

private:
	FVector2D simulatePosition(const E_TNTetrominoDirection tetrominoDirection);
	
	int32 mod(int32 n, int32 m) { return ((n % m) + m) % m; }

public:
	FHideGuideTetromino OnHideGuideTetromino;
	FSetGuideTetromino OnSetGuideTetromino;
};
