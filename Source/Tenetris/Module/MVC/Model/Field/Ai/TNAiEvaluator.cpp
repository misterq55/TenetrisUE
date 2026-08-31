#include "TNAiEvaluator.h"

namespace
{
	// Rotation convention identical to FTNTetromino::calculateRotatedCoordinates:
	//   CW  (state 1): (x, y) -> ( y, -x)
	//   180 (state 2): (x, y) -> (-x, -y)
	//   CCW (state 3): (x, y) -> (-y,  x)
	FVector2D rotateOffset(const FVector2D& coord, int32 rotationState)
	{
		const int32 state = ((rotationState % 4) + 4) % 4;
		switch (state)
		{
		case 1: return FVector2D(coord.Y, -coord.X);
		case 2: return FVector2D(-coord.X, -coord.Y);
		case 3: return FVector2D(-coord.Y, coord.X);
		case 0:
		default: return coord;
		}
	}

	int32 getGridHeight(const FTNAiOccupancyGrid& grid)
	{
		return grid.Num();
	}

	int32 getGridWidth(const FTNAiOccupancyGrid& grid)
	{
		return grid.Num() > 0 ? grid[0].Num() : 0;
	}
}

TTetrominoCoordinate FTNAiEvaluator::getRotatedCells(E_TNTetrominoType pieceType, int32 rotationState)
{
	const uint32 typeIndex = static_cast<uint32>(pieceType);
	if (typeIndex >= UE_ARRAY_COUNT(TetrominoCoordinatesByType))
	{
		return TTetrominoCoordinate();
	}

	const TTetrominoCoordinate& baseCells = TetrominoCoordinatesByType[typeIndex];

	TTetrominoCoordinate result;
	result.Reserve(baseCells.Num());
	for (const FVector2D& coord : baseCells)
	{
		result.Add(rotateOffset(coord, rotationState));
	}
	return result;
}

bool FTNAiEvaluator::isValidPlacement(const FTNAiOccupancyGrid& grid, const TTetrominoCoordinate& cells,
	int32 originColumn, int32 originRow)
{
	const int32 height = getGridHeight(grid);
	const int32 width = getGridWidth(grid);

	for (const FVector2D& cell : cells)
	{
		const int32 x = originColumn + static_cast<int32>(cell.X);
		const int32 y = originRow + static_cast<int32>(cell.Y);

		// Side walls and floor are hard limits.
		if (x < 0 || x >= width || y < 0)
		{
			return false;
		}

		// Cells above the visible top are treated as empty space (spawn area), so skip them.
		if (y >= height)
		{
			continue;
		}

		if (grid[y][x])
		{
			return false;
		}
	}

	return true;
}

int32 FTNAiEvaluator::findLandingRow(const FTNAiOccupancyGrid& grid, const TTetrominoCoordinate& cells,
	int32 originColumn)
{
	const int32 height = getGridHeight(grid);
	if (height <= 0 || cells.Num() == 0)
	{
		return INDEX_NONE;
	}

	// Start high enough that the piece is guaranteed to fit, then drop until the next step collides.
	// The highest cell offset determines how far above the top we must start.
	int32 maxCellY = 0;
	for (const FVector2D& cell : cells)
	{
		maxCellY = FMath::Max(maxCellY, static_cast<int32>(cell.Y));
	}

	int32 originRow = height + maxCellY;

	// The starting position must itself be valid; if not, the column is unusable.
	if (!isValidPlacement(grid, cells, originColumn, originRow))
	{
		return INDEX_NONE;
	}

	while (isValidPlacement(grid, cells, originColumn, originRow - 1))
	{
		--originRow;
	}

	return originRow;
}

void FTNAiEvaluator::stampPiece(FTNAiOccupancyGrid& grid, const TTetrominoCoordinate& cells,
	int32 originColumn, int32 originRow)
{
	const int32 height = getGridHeight(grid);
	const int32 width = getGridWidth(grid);

	for (const FVector2D& cell : cells)
	{
		const int32 x = originColumn + static_cast<int32>(cell.X);
		const int32 y = originRow + static_cast<int32>(cell.Y);

		// Only cells inside the visible board contribute to the occupancy used for scoring.
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			grid[y][x] = true;
		}
	}
}

float FTNAiEvaluator::scoreBoard(const FTNAiOccupancyGrid& grid) const
{
	const int32 height = getGridHeight(grid);
	const int32 width = getGridWidth(grid);

	// Column heights: distance from the floor to the highest occupied cell (+1).
	TArray<int32> columnHeights;
	columnHeights.Init(0, width);

	int32 holes = 0;

	for (int32 x = 0; x < width; ++x)
	{
		bool bSeenBlock = false;
		for (int32 y = height - 1; y >= 0; --y)
		{
			if (grid[y][x])
			{
				if (!bSeenBlock)
				{
					columnHeights[x] = y + 1;
					bSeenBlock = true;
				}
			}
			else if (bSeenBlock)
			{
				// Empty cell with a filled cell above it == hole.
				++holes;
			}
		}
	}

	int32 aggregateHeight = 0;
	for (int32 x = 0; x < width; ++x)
	{
		aggregateHeight += columnHeights[x];
	}

	int32 bumpiness = 0;
	for (int32 x = 0; x < width - 1; ++x)
	{
		bumpiness += FMath::Abs(columnHeights[x] - columnHeights[x + 1]);
	}

	int32 linesCleared = 0;
	for (int32 y = 0; y < height; ++y)
	{
		bool bFull = true;
		for (int32 x = 0; x < width; ++x)
		{
			if (!grid[y][x])
			{
				bFull = false;
				break;
			}
		}
		if (bFull)
		{
			++linesCleared;
		}
	}

	// Higher score is better: cleared lines reward, everything else penalizes.
	return Weights.LinesCleared * static_cast<float>(linesCleared)
		- Weights.AggregateHeight * static_cast<float>(aggregateHeight)
		- Weights.Holes * static_cast<float>(holes)
		- Weights.Bumpiness * static_cast<float>(bumpiness);
}

FTNAiPlacement FTNAiEvaluator::FindBestPlacement(const FTNAiOccupancyGrid& grid, E_TNTetrominoType pieceType) const
{
	FTNAiPlacement best;

	const int32 width = getGridWidth(grid);
	if (width <= 0 || getGridHeight(grid) <= 0)
	{
		return best;
	}

	const uint32 typeIndex = static_cast<uint32>(pieceType);
	if (typeIndex >= UE_ARRAY_COUNT(TetrominoCoordinatesByType))
	{
		return best;
	}

	for (int32 rotation = 0; rotation < 4; ++rotation)
	{
		const TTetrominoCoordinate cells = getRotatedCells(pieceType, rotation);
		if (cells.Num() == 0)
		{
			continue;
		}

		// TODO: Reachability is simplified to all columns here (no SRS wall-kick, no path check).
		// Add kick simulation / horizontal-path validation for T-spin and wall placements later.
		for (int32 originColumn = 0; originColumn < width; ++originColumn)
		{
			const int32 originRow = findLandingRow(grid, cells, originColumn);
			if (originRow == INDEX_NONE)
			{
				continue;
			}

			// Score on a throwaway copy so the input board is never mutated.
			FTNAiOccupancyGrid candidate = grid;
			stampPiece(candidate, cells, originColumn, originRow);

			const float score = scoreBoard(candidate);
			if (!best.bValid || score > best.Score)
			{
				best.bValid = true;
				best.Rotation = rotation;
				best.OriginColumn = originColumn;
				best.OriginRow = originRow;
				best.Score = score;
			}
		}
	}

	// TODO: Currently greedy (current piece only). Extend to hold/next lookahead for higher difficulty.
	return best;
}