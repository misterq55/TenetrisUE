// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

const int32 RowMax = 20;
const int32 ColumnMax = 10;

const float MinoRatio = 0.25f;

enum class E_TNTetrominoType : uint32
{
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
	Guide,
	Obstacle,
	None,
};

enum class E_TNTetrominoDirection : uint32
{
	Down,
	Left,
	Right,
	None,
};

enum class E_TNTetrominoRotation : uint32
{
	ClockWise,
	CounterClockWise,
};

enum class E_TNTetrominoBufferDataType : uint32
{
	Blank,
	Tetromino,
	Item,
};