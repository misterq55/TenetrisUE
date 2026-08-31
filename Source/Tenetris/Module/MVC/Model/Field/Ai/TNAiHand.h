#pragma once

#include "Tenetris/Common/TenetrisEnum.h"

// FTNAiHand (name not final; alt: FTNAiActuator / FTNAiMover)
//
// Phase B "hand" of the CPU AI, pure-decision part only.
//
// Given the piece's actual current (rotation, column) and the target (rotation, column)
// chosen by Phase A, decide the SINGLE next input to emit this Decide tick.
// The integration seam is responsible for:
//   - reading the real (rotation, column) from the model each tick,
//   - routing the returned E_TNControlType to the target board's HandleControlInput.
// This class holds no model references; it is fully testable in isolation.
//
// Movement model: "method B" (tap, one cell at a time) - emit one step, then re-read next tick.
// Order: rotation first, then horizontal move, then hard drop.
// TODO: A movement decision returning a Start* enum must be paired with the matching Stop* at the
//       write path so exactly one cell is moved (otherwise DAS/ARR auto-repeats and overshoots).
// TODO: DAS/ARR-based hold-and-watch movement for human-like feel (method A).
// TODO: Use soft drop for finer control / faster placement when needed.

// Result of a single Phase B decision.
struct FTNAiHandDecision
{
	// The input to emit this tick. Only meaningful when bHasInput is true.
	E_TNControlType Input = E_TNControlType::HardDrop;
	bool bHasInput = false;

	// True once the piece is aligned and HardDrop has been issued: the current target is consumed.
	bool bPlacementComplete = false;
};

class FTNAiHand
{
public:
	FTNAiHand() = default;
	~FTNAiHand() = default;

public:
	// Decide the next single input from the actual vs. target pose.
	// currentRotation is normalized to 0..3 internally.
	FTNAiHandDecision DecideNextInput(int32 currentRotation, int32 currentColumn,
		int32 targetRotation, int32 targetColumn);

	// Reset the internal progress watchdog. Call when a new target is assigned.
	void ResetProgress();

private:
	// Choose the cheaper rotation direction (CW vs CCW) to reach the target rotation.
	static E_TNControlType chooseRotationInput(int32 currentRotation, int32 targetRotation);

	// Advance / evaluate the stuck watchdog. Returns true if considered stuck.
	bool updateStuckWatchdog(int32 currentRotation, int32 currentColumn);

private:
	// Progress watchdog: if the pose does not change for too many ticks, give up and hard-drop.
	// TODO: Smarter stuck fallback (re-evaluate instead of forced hard drop).
	static constexpr int32 MaxStuckTicks = 8;

	int32 StuckTicks = 0;
	int32 LastRotation = -1;
	int32 LastColumn = TNumericLimits<int32>::Max();
};