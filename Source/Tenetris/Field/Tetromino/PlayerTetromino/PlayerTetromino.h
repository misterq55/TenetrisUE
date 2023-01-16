// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/Tetromino/TetrominoBase.h"

class FPlayerTetromino
{
public:
	FPlayerTetromino() {}
	virtual ~FPlayerTetromino() {}
	virtual void Move(ETetrominoDirection InTetrominoDirection);
	virtual void Rotate(ETetrominoRotation InTetrominoRotation);
};