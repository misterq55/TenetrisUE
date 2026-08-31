#pragma once

#include "Tenetris/Common/TenetrisType.h"
#include "Tenetris/Common/TenetrisEnum.h"

// FTNAiEvaluator (name not final; alt: FTNAiBrain / FTNAiPlanner)
//
// Phase A "brain" of the CPU AI: a pure, stateless evaluation stage.
// Given a board occupancy snapshot and the current piece type, it enumerates every
// (rotation x column) placement, stamps it onto a virtual board, scores the result
// with a weighted sum of four metrics, and returns the best target.
//
// This class is intentionally decoupled from FTNFieldModel and its coordinate system.
// The board is passed in as a normalized occupancy grid:
//   - grid[row][col], row 0 == bottom row, increasing row == upward (matches gravity: down == decreasing row)
//   - true  == cell occupied, false == cell empty
//   - dimensions: exactly the logical play area (Width columns x Height rows), i.e. the region
//     checkMino treats as in-bounds (x in [0,Width), y in [0,Height)).
//
// IMPORTANT: The authoritative collision data in FTNFieldModel is the CheckBuffer
// (getValueFromCheckBuffer / bSpaceInverted), NOT LockedGrid (LockedGrid is view-only for rendering).
// The integration seam must therefore build this occupancy grid from the CheckBuffer:
//   occupancy[y][x] = (getValueFromCheckBuffer(x, y).Type != E_TNTetrominoType::None)
//   for x in [0,Width), y in [0,Height).
// Note that any non-None type (including Obstacle) counts as occupied, matching checkMino.
//
// Difficulty note: this is "Selection 1" - SRS piece shapes are used, but SRS wall-kick
// reachability is ignored. Only placements reachable by horizontal movement are intended.

// Occupancy board: row-major, row 0 == bottom. true == occupied.
using FTNAiOccupancyGrid = TArray<TArray<bool>>;

// Tunable evaluation weights. Signs are baked into scoreBoard (see .cpp).
struct FTNAiEvalWeights
{
	float AggregateHeight = 0.510066f;
	float Holes = 0.35663f;
	float Bumpiness = 0.184483f;
	float LinesCleared = 0.760666f;
};

// A concrete placement decision produced by Phase A.
struct FTNAiPlacement
{
	// Target rotation state (0..3) and the piece origin column for that placement.
	int32 Rotation = 0;
	int32 OriginColumn = 0;

	// Landing row of the piece origin (informational; Phase B re-reads real state anyway).
	int32 OriginRow = 0;

	// Evaluation score of this placement (higher is better).
	float Score = -TNumericLimits<float>::Max();

	// False when no legal placement was found for the given piece/board.
	bool bValid = false;
};

class FTNAiEvaluator
{
public:
	FTNAiEvaluator() = default;
	explicit FTNAiEvaluator(const FTNAiEvalWeights& weights)
		: Weights(weights)
	{
	}
	~FTNAiEvaluator() = default;

public:
	// Enumerate all (rotation x column) placements for the given piece over the board,
	// score each, and return the best. Pure: does not mutate any external state.
	FTNAiPlacement FindBestPlacement(const FTNAiOccupancyGrid& grid, E_TNTetrominoType pieceType) const;

	const FTNAiEvalWeights& GetWeights() const
	{
		return Weights;
	}

	void SetWeights(const FTNAiEvalWeights& weights)
	{
		Weights = weights;
	}

private:
	// Rotated cell offsets for a piece type at a rotation state (0..3).
	// Reuses the base coordinates and rotation convention of FTNTetromino to stay rule-consistent.
	static TTetrominoCoordinate getRotatedCells(E_TNTetrominoType pieceType, int32 rotationState);

	// True if the piece (given rotated cells) fits at (originColumn, originRow) without
	// overlapping the floor, side walls or an occupied cell. Cells above the top are allowed (empty space).
	static bool isValidPlacement(const FTNAiOccupancyGrid& grid, const TTetrominoCoordinate& cells,
		int32 originColumn, int32 originRow);

	// Lowest origin row the piece can reach in the given column (hard-drop landing).
	// Returns INDEX_NONE if the piece cannot be placed in that column at all.
	static int32 findLandingRow(const FTNAiOccupancyGrid& grid, const TTetrominoCoordinate& cells,
		int32 originColumn);

	// Stamp the piece into a copy of the grid at the landing position (occupied cells only).
	static void stampPiece(FTNAiOccupancyGrid& grid, const TTetrominoCoordinate& cells,
		int32 originColumn, int32 originRow);

	// Weighted-sum score for a fully-stamped board.
	float scoreBoard(const FTNAiOccupancyGrid& grid) const;

private:
	FTNAiEvalWeights Weights;
};