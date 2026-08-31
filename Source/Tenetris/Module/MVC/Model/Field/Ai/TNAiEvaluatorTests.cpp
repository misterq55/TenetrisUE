#include "Misc/AutomationTest.h"
#include "TNAiEvaluator.h"
#include "TNAiHand.h"
#include "TNAiBoardSnapshot.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
	// Build an empty WxH occupancy grid (all false).
	FTNAiOccupancyGrid makeEmptyGrid(int32 width, int32 height)
	{
		FTNAiOccupancyGrid grid;
		grid.Reserve(height);
		for (int32 y = 0; y < height; ++y)
		{
			TArray<bool> row;
			row.Init(false, width);
			grid.Add(MoveTemp(row));
		}
		return grid;
	}
}

// Phase A: a placement must be found on an empty standard board for a simple piece.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTNAiEvaluatorFindsPlacementTest,
	"Tenetris.Ai.Evaluator.FindsPlacementOnEmptyBoard",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTNAiEvaluatorFindsPlacementTest::RunTest(const FString& Parameters)
{
	const FTNAiEvaluator evaluator;
	const FTNAiOccupancyGrid grid = makeEmptyGrid(10, 20);

	const FTNAiPlacement placement = evaluator.FindBestPlacement(grid, E_TNTetrominoType::O);

	TestTrue(TEXT("A valid placement is found for O on an empty board"), placement.bValid);
	TestTrue(TEXT("Origin column is within board bounds"),
		placement.OriginColumn >= 0 && placement.OriginColumn < 10);
	return true;
}

// Phase A: on an empty board the piece must land on the floor (no floating placement).
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTNAiEvaluatorPrefersLowFlatTest,
	"Tenetris.Ai.Evaluator.PrefersLowFlatPlacement",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTNAiEvaluatorPrefersLowFlatTest::RunTest(const FString& Parameters)
{
	const FTNAiEvaluator evaluator;
	const FTNAiOccupancyGrid grid = makeEmptyGrid(10, 20);

	const FTNAiPlacement placement = evaluator.FindBestPlacement(grid, E_TNTetrominoType::I);

	TestTrue(TEXT("A valid placement is found for I on an empty board"), placement.bValid);
	// On an empty board the piece must land on the floor (origin row near the bottom).
	TestTrue(TEXT("Piece lands near the floor on an empty board"), placement.OriginRow <= 2);
	return true;
}

// Phase B: rotation is matched before movement.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTNAiHandRotatesFirstTest,
	"Tenetris.Ai.Hand.RotatesBeforeMoving",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTNAiHandRotatesFirstTest::RunTest(const FString& Parameters)
{
	FTNAiHand hand;
	hand.ResetProgress();

	// current rotation 0, target rotation 1, and a column mismatch too.
	const FTNAiHandDecision decision = hand.DecideNextInput(/*curRot*/0, /*curCol*/5, /*tgtRot*/1, /*tgtCol*/2);

	TestTrue(TEXT("An input is produced"), decision.bHasInput);
	TestTrue(TEXT("Rotation is issued before movement"),
		decision.Input == E_TNControlType::RotateClockWise);
	TestFalse(TEXT("Placement is not complete yet"), decision.bPlacementComplete);
	return true;
}

// Phase B: once rotation matches, move toward the target column.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTNAiHandMovesTowardTargetTest,
	"Tenetris.Ai.Hand.MovesTowardTargetColumn",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTNAiHandMovesTowardTargetTest::RunTest(const FString& Parameters)
{
	FTNAiHand hand;

	hand.ResetProgress();
	const FTNAiHandDecision moveLeft = hand.DecideNextInput(/*curRot*/1, /*curCol*/5, /*tgtRot*/1, /*tgtCol*/2);
	TestTrue(TEXT("Moves left when current column is to the right of target"),
		moveLeft.Input == E_TNControlType::StartMoveLeft);

	hand.ResetProgress();
	const FTNAiHandDecision moveRight = hand.DecideNextInput(/*curRot*/1, /*curCol*/2, /*tgtRot*/1, /*tgtCol*/5);
	TestTrue(TEXT("Moves right when current column is to the left of target"),
		moveRight.Input == E_TNControlType::StartMoveRight);
	return true;
}

// Phase B: aligned pose issues a hard drop and completes the placement.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTNAiHandHardDropsWhenAlignedTest,
	"Tenetris.Ai.Hand.HardDropsWhenAligned",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTNAiHandHardDropsWhenAlignedTest::RunTest(const FString& Parameters)
{
	FTNAiHand hand;
	hand.ResetProgress();

	const FTNAiHandDecision decision = hand.DecideNextInput(/*curRot*/2, /*curCol*/4, /*tgtRot*/2, /*tgtCol*/4);

	TestTrue(TEXT("Hard drop is issued when aligned"), decision.Input == E_TNControlType::HardDrop);
	TestTrue(TEXT("Placement is marked complete"), decision.bPlacementComplete);
	return true;
}

// Snapshot: non-None cells (including Obstacle) become occupied; None becomes empty.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTNAiSnapshotOccupancyTest,
	"Tenetris.Ai.Snapshot.OccupancyFromCells",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTNAiSnapshotOccupancyTest::RunTest(const FString& Parameters)
{
	// Bottom row fully filled with Obstacle, everything else None.
	const auto sampler = [](int32 x, int32 y) -> FTNCellInfo
	{
		if (y == 0)
		{
			return FTNCellInfo(-1, E_TNTetrominoType::Obstacle);
		}
		return FTNCellInfo(-1, E_TNTetrominoType::None);
	};

	const FTNAiOccupancyGrid grid = FTNAiBoardSnapshot::BuildFromCellSampler(4, 3, sampler);

	TestTrue(TEXT("Obstacle cell is occupied"), grid[0][0]);
	TestFalse(TEXT("None cell is empty"), grid[1][0]);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS