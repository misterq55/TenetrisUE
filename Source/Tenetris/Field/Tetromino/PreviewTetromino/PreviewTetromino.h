// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/Tetromino/TetrominoBase.h"

class FPreviewTetromino : public FTetrominoBase
{
public:
	FPreviewTetromino() 
		: FTetrominoBase()
	{}

	virtual ~FPreviewTetromino() {}
	virtual bool Move(ETetrominoDirection tetrominoDirection) override;
	virtual bool Rotate(ETetrominoRotation tetrominoRotation) override;
};