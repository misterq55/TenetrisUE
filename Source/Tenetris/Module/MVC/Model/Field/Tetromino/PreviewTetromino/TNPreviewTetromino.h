// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetrominoBase.h"

class FTNPreviewTetromino : public FTNTetrominoBase
{
public:
	FTNPreviewTetromino() 
		: FTNTetrominoBase()
	{}

	FTNPreviewTetromino(TSharedPtr<FTNTetrominoInfo> tetrominoInfo)
		: FTNTetrominoBase(tetrominoInfo)
	{}

	virtual ~FTNPreviewTetromino() {}
	virtual bool Move(E_TNTetrominoDirection tetrominoDirection) override;
	virtual bool Rotate(E_TNTetrominoRotation tetrominoRotation) override;
};