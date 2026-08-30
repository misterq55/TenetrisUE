#include "TNFieldStateProvider.h"

void FTNFieldStateProvider::WriteFieldState(int32 boardId, const TArray<TArray<FTNCellInfo>>& fieldBuffer)
{
	// FindOrAdd then assign: overwrite this board's state as a whole with its latest confirmed state.
	FieldStates.Emplace(boardId, fieldBuffer);
}

const TArray<TArray<FTNCellInfo>>* FTNFieldStateProvider::ReadFieldState(int32 boardId) const
{
	return FieldStates.Find(boardId);
}

TArray<int32> FTNFieldStateProvider::GetBoardIds() const
{
	TArray<int32> boardIds;
	FieldStates.GenerateKeyArray(boardIds);
	return boardIds;
}
