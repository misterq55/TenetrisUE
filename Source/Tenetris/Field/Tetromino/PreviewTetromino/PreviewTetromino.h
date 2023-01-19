// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/Tetromino/TetrominoBase.h"

class FPreviewTetromino : public FTetrominoBase
{
public:
	FPreviewTetromino() 
		: FTetrominoBase()
	{}

	FPreviewTetromino(ATenetrisFieldBase* InCurrentTenetrisField) 
		: FTetrominoBase(InCurrentTenetrisField)
	{}

	virtual ~FPreviewTetromino() {}
	virtual bool Move(ETetrominoDirection InTetrominoDirection) override;
	virtual bool Rotate(ETetrominoRotation InTetrominoRotation) override;
};