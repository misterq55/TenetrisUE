// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTNTetrominoGenerator
{
public:
	FTNTetrominoGenerator() = default;
	~FTNTetrominoGenerator() = default;
	
public:
	void Initialize();

	E_TNTetrominoType GetTop();
	E_TNTetrominoType GetAt(const int32 index);

private:
	static void shuffleTetrominoBag(TArray<E_TNTetrominoType>& bag);
	void refillTetrominoArray(const int32 tetrominoArrayResetCounter);

private:
	TArray<E_TNTetrominoType> TetrominoArray;
	int32 TetrominoLimitCounter = 0;
	int32 TetrominoArrayResetCounter = 0;
};