// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTetrominoBase
{
public:
	FTetrominoBase() {}
	virtual ~FTetrominoBase() {}
	virtual void Move(ETetrominoDirection InTetrominoDirection) {}
	virtual void Rotate(ETetrominoRotation InTetrominoRotation) {}
};