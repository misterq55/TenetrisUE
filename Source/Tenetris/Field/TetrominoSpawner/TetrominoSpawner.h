// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTetrominoSpawner
{
public:
	FTetrominoSpawner()
		: TetrominoArrayResetCounter(0)
	{}

	~FTetrominoSpawner() {}

	void Initialize();

	ETetrominoType GetTop();
	ETetrominoType GetAt(int32 InIndex);

private:
	void ShuffleTetrominoBag(TArray<ETetrominoType>& InBag);
	void RefillTetrominoArray(int32 InTetrominoArrayResetCounter);

private:
	TArray<ETetrominoType> TetrominoArray;
	int32 TetrominoLimitCounter;
	int32 TetrominoArrayResetCounter;
};