// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/Tetromino/TetrominoBase.h"

class FPlayerTetromino : public FTetrominoBase
{
public:
	FPlayerTetromino() 
		: FTetrominoBase()
	{}

	virtual ~FPlayerTetromino() {}
	virtual bool Move(ETetrominoDirection InTetrominoDirection) override;
	virtual bool Rotate(ETetrominoRotation InTetrominoRotation) override;
	virtual void LockDown() override;
	virtual void Spawn() override;
	virtual void SetGuideTetromino() override;

private:
	FVector2D SimulatePosition(ETetrominoDirection InTetrominoDirection);
};