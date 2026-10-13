#pragma once

#include "Tenetris/Common/TenetrisType.h"
#include "CoreMinimal.h"

// A passive, shared lookup store (a "blackboard") that gathers the "latest
// confirmed field state" of multiple boards (my board + n opponent boards) in
// one place. It never pushes data to models; anyone reads it on demand. It is a
// passive key->state map.
//
// The data flow is always one-directional:
//   - a model writes its own confirmed state into this store
//   - views (renderers) / CPU AI read from this store
// This store never knows, references, or calls back into models
// ("provider->model" dependency is forbidden). From the store's point of view,
// the writer and the reader are indistinguishable.
//
// The stored value (fieldBuffer) is the confirmed "logic buffer" used for
// checks (e.g. FTNFieldModel::CheckBuffer, or the current buffer when the field
// is inverted). Which orientation buffer to write is the writer's decision; the
// store does not care. The display-only LockedGrid is not stored here.
// Reason for keeping the value type identical to LockedGrid
// (TArray<TArray<FTNCellInfo>>): the buffer the model uses is stored as-is
// (padding included) with zero conversion or coordinate recomputation.
//
// Ownership: a match/game-level object owns a single instance (currently the
// local-multiple test model owns it). The timing of reads/writes (Decide/Resolve
// phases) is the responsibility of the calling tick loop; this store does not
// enforce it.
//
// Alternative names: FTNBlackboard / FTNFieldStateRegistry / FTNFieldStateStore
class FTNFieldStateProvider
{
public:
	FTNFieldStateProvider() = default;
	~FTNFieldStateProvider() = default;

public:
	// write: replaces one board's latest confirmed field buffer as a whole.
	void WriteFieldState(int32 boardId, const TArray<TArray<FTNCellInfo>>& fieldBuffer);

	// read: returns the latest state of a specific board. nullptr if absent.
	const TArray<TArray<FTNCellInfo>>* ReadFieldState(int32 boardId) const;

	// For full iteration: returns the list of all stored BoardIds.
	// The reader iterates this list and calls ReadFieldState for each board.
	// (If ordering is needed, the reader sorts it. The store does not enforce order.)
	TArray<int32> GetBoardIds() const;

private:
	// Simple key(BoardId) -> confirmed field buffer map.
	// No slot abstraction / LOD / serialization.
	TMap<int32, TArray<TArray<FTNCellInfo>>> FieldStates;
};
