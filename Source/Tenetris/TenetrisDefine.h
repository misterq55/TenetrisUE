// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

const int32 RowMax = 20;
const int32 ColumnMax = 10;

enum class ETetrominoType : uint32
{
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
	Obstacle,
};

enum class ETetrominoDirection : uint32
{
	Down,
	Left,
	Right,
};

enum class ETetrominoRotation : uint32
{
	ClockWise,
	CounterClockWise,
};

enum class ETetrominoBufferDataType : uint32
{
	Blank,
	Tetromino,
	Item,
};