#include "TNAiHand.h"

namespace
{
	int32 normalizeRotation(int32 rotation)
	{
		return ((rotation % 4) + 4) % 4;
	}
}

void FTNAiHand::ResetProgress()
{
	StuckTicks = 0;
	LastRotation = -1;
	LastColumn = TNumericLimits<int32>::Max();
}

E_TNControlType FTNAiHand::chooseRotationInput(int32 currentRotation, int32 targetRotation)
{
	const int32 cur = normalizeRotation(currentRotation);
	const int32 tgt = normalizeRotation(targetRotation);

	// Clockwise distance from cur to tgt in [0,3]. CCW distance is the complement.
	const int32 cwSteps = normalizeRotation(tgt - cur);

	// Prefer the direction with fewer steps; tie (2 steps) defaults to clockwise.
	if (cwSteps <= 4 - cwSteps)
	{
		return E_TNControlType::RotateClockWise;
	}
	return E_TNControlType::RotateCounterClockWise;
}

bool FTNAiHand::updateStuckWatchdog(int32 currentRotation, int32 currentColumn)
{
	const int32 cur = normalizeRotation(currentRotation);

	if (cur == LastRotation && currentColumn == LastColumn)
	{
		++StuckTicks;
	}
	else
	{
		StuckTicks = 0;
		LastRotation = cur;
		LastColumn = currentColumn;
	}

	return StuckTicks >= MaxStuckTicks;
}

FTNAiHandDecision FTNAiHand::DecideNextInput(int32 currentRotation, int32 currentColumn,
	int32 targetRotation, int32 targetColumn)
{
	FTNAiHandDecision decision;

	const int32 cur = normalizeRotation(currentRotation);
	const int32 tgt = normalizeRotation(targetRotation);

	// If we have been unable to make progress for too long, cut losses and hard-drop.
	if (updateStuckWatchdog(currentRotation, currentColumn))
	{
		decision.Input = E_TNControlType::HardDrop;
		decision.bHasInput = true;
		decision.bPlacementComplete = true;
		return decision;
	}

	// 1. Match rotation first: real column may shift due to SRS kicks, so we re-read next tick.
	if (cur != tgt)
	{
		decision.Input = chooseRotationInput(cur, tgt);
		decision.bHasInput = true;
		return decision;
	}

	// 2. Match column: one cell per tick (tap), then re-read and re-decide.
	if (currentColumn != targetColumn)
	{
		if (currentColumn > targetColumn)
		{
			decision.Input = E_TNControlType::StartMoveLeft;
		}
		else
		{
			decision.Input = E_TNControlType::StartMoveRight;
		}
		decision.bHasInput = true;
		return decision;
	}

	// 3. Aligned: finish with a hard drop and consume the target.
	decision.Input = E_TNControlType::HardDrop;
	decision.bHasInput = true;
	decision.bPlacementComplete = true;
	return decision;
}