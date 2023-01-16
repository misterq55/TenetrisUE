// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/TenetrisDefine.h"

class ATenetrisFieldBase;

class FTetrominoBase
{
public:
	FTetrominoBase() 
		: CurrentTenetrisField(nullptr)
		, CurrentTetrominoType(ETetrominoType::None)
	{}

	FTetrominoBase(ATenetrisFieldBase* InCurrentTenetrisField)
		: CurrentTenetrisField(InCurrentTenetrisField)
		, CurrentTetrominoType(ETetrominoType::None)
	{}

	virtual ~FTetrominoBase() {}
	virtual void Move(ETetrominoDirection InTetrominoDirection);
	virtual void Rotate(ETetrominoRotation InTetrominoRotation);
	
	void SetTenetrisField(ATenetrisFieldBase* InTenetrisField)
	{
		CurrentTenetrisField = InTenetrisField;
	}

	void SetTetrominoPosition(int32 X, int32 Y);

	void SetTetrominoType(ETetrominoType InCurrentTetrominoType)
	{
		CurrentTetrominoType = InCurrentTetrominoType;
	}

protected:
	ATenetrisFieldBase* CurrentTenetrisField;
	ETetrominoType CurrentTetrominoType;
	TArray<FVector2D> TetrominoCoordinate;
	FVector2D TetrominoCurrentPosition;
};