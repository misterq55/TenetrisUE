// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTetrominoGenerator
{
public:
	FTetrominoGenerator()
		: TetrominoArrayResetCounter(0)
	{}

	~FTetrominoGenerator() {}

	void Initialize();

	ETetrominoType GetTop();
	ETetrominoType GetAt(int32 InIndex);

private:
	void ShuffleTetrominoBag(TArray<ETetrominoType>& bag);
	void RefillTetrominoArray(int32 tetrominoArrayResetCounter);

private:
	TArray<ETetrominoType> TetrominoArray;
	int32 TetrominoLimitCounter;
	int32 TetrominoArrayResetCounter;
};