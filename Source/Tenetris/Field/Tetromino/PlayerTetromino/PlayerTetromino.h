// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/Tetromino/TetrominoBase.h"

class FPlayerTetromino : public FTetrominoBase
{
public:
	FPlayerTetromino() 
		: FTetrominoBase()
	{}

	FPlayerTetromino(ATenetrisFieldBase* InCurrentTenetrisField) 
		: FTetrominoBase(InCurrentTenetrisField)
	{}

	virtual ~FPlayerTetromino() {}
	virtual void Move(ETetrominoDirection InTetrominoDirection) override;
	virtual void Rotate(ETetrominoRotation InTetrominoRotation) override;
	virtual void LockDown() override;
};