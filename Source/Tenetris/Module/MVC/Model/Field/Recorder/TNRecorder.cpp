#include "TNRecorder.h"

void FTNRecorder::FTNFieldRecord::RecordSpawn(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Spawn;
	behavior.Position = position;
	behavior.RotationState = rotationState;
	behavior.CurrentTetrominoType = tetrominoType;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordLockDown(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::LockDown;
	behavior.Position = position;
	behavior.RotationState = rotationState;
	behavior.CurrentTetrominoType = tetrominoType;
	Behaviors.Add(behavior);
}

void FTNRecorder::RecordSpawn(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordSpawn(position, rotationState, tetrominoType);
}

void FTNRecorder::RecordLockDown(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType)
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordLockDown(position, rotationState, tetrominoType);
}

void FTNRecorder::FTNFieldRecord::RecordTransform(FVector2D position, int32 rotationState)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Transform;
	behavior.Position = position;
	behavior.RotationState = rotationState;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordRotateField()
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::RotateField;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordHold(E_TNTetrominoType currentTetrominoType, E_TNTetrominoType holdTetrominoType)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Hold;
	behavior.CurrentTetrominoType = currentTetrominoType;
	behavior.HoldTetrominoType = holdTetrominoType;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer,
	const TArray<TArray<FTNCellInfo>>& reversedBuffer)
{
	NormalBuffer = normalBuffer;
	ReversedBuffer = reversedBuffer;
}

void FTNRecorder::FTNFieldRecord::RecordLineClear()
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::LineClear;
	Behaviors.Add(behavior);
}

E_TNBehaviorState FTNRecorder::FTNFieldRecord::PeekLastBehaviorState() const
{
	if (Behaviors.IsEmpty())
	{
		return E_TNBehaviorState::None;
	}
	
	return Behaviors.Last().BehaviorState;
}

void FTNRecorder::FTNFieldRecord::PopSpawnValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType)
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

	const FTNBehavior& behavior = Behaviors.Last();
	ensureMsgf(behavior.BehaviorState == E_TNBehaviorState::Spawn,
		TEXT("PopSpawnValues called with mismatched behavior state (%d)"), static_cast<int32>(behavior.BehaviorState));

	outPosition = behavior.Position;
	outRotationState = behavior.RotationState;
	outType = behavior.CurrentTetrominoType;
	Behaviors.Pop();
}

void FTNRecorder::FTNFieldRecord::PopTransformValues(FVector2D& outPosition, int32& outRotationState)
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

	const FTNBehavior& behavior = Behaviors.Last();
	ensureMsgf(behavior.BehaviorState == E_TNBehaviorState::Transform,
		TEXT("PopTransformValues called with mismatched behavior state (%d)"), static_cast<int32>(behavior.BehaviorState));

	outPosition = behavior.Position;
	outRotationState = behavior.RotationState;
	Behaviors.Pop();
}

void FTNRecorder::FTNFieldRecord::PopRotateField()
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

	ensureMsgf(Behaviors.Last().BehaviorState == E_TNBehaviorState::RotateField,
		TEXT("PopRotateField called with mismatched behavior state (%d)"), static_cast<int32>(Behaviors.Last().BehaviorState));

	Behaviors.Pop();
}

void FTNRecorder::FTNFieldRecord::PopHoldValues(E_TNTetrominoType& outCurrentType, E_TNTetrominoType& outHoldType)
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

	const FTNBehavior& behavior = Behaviors.Last();
	ensureMsgf(behavior.BehaviorState == E_TNBehaviorState::Hold,
		TEXT("PopHoldValues called with mismatched behavior state (%d)"), static_cast<int32>(behavior.BehaviorState));

	outCurrentType = behavior.CurrentTetrominoType;
	outHoldType = behavior.HoldTetrominoType;
	Behaviors.Pop();
}

void FTNRecorder::FTNFieldRecord::PopLockDownValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType)
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

	const FTNBehavior& behavior = Behaviors.Last();
	ensureMsgf(behavior.BehaviorState == E_TNBehaviorState::LockDown,
		TEXT("PopLockDownValues called with mismatched behavior state (%d)"), static_cast<int32>(behavior.BehaviorState));

	outPosition = behavior.Position;
	outRotationState = behavior.RotationState;
	outType = behavior.CurrentTetrominoType;
	Behaviors.Pop();
}

void FTNRecorder::FTNFieldRecord::PopLineClearBuffers(TArray<TArray<FTNCellInfo>>& outNormalBuffer, TArray<TArray<FTNCellInfo>>& outReversedBuffer)
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

	ensureMsgf(Behaviors.Last().BehaviorState == E_TNBehaviorState::LineClear,
		TEXT("PopLineClearBuffers called with mismatched behavior state (%d)"), static_cast<int32>(Behaviors.Last().BehaviorState));

	// 버퍼는 이 LineClear behavior 전용 데이터이므로 이동시켜 복사 비용을 회피한다.
	outNormalBuffer = MoveTemp(NormalBuffer);
	outReversedBuffer = MoveTemp(ReversedBuffer);
	NormalBuffer.Reset();
	ReversedBuffer.Reset();
	Behaviors.Pop();
}

void FTNRecorder::AddFieldRecord()
{
	FieldRecords.Emplace(MakeShareable(new FTNFieldRecord()));

	// pruning은 새 레코드가 추가될 때 일괄 처리하여 다른 Record* API와 정책을 통일한다.
	// (이전에는 RecordBuffers에서만 pruning되어 락다운 이외의 조작 기록이 비대칭으로 남았음)
	while (FieldRecords.Num() > MaxRecords)
	{
		FieldRecords.RemoveAt(0);
	}
}

void FTNRecorder::RecordTransform(FVector2D position, int32 rotationState) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordTransform(position, rotationState);
}

void FTNRecorder::RecordRotateField() const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordRotateField();
}

void FTNRecorder::RecordHold(E_TNTetrominoType currentTetrominoType, E_TNTetrominoType holdTetrominoType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordHold(currentTetrominoType, holdTetrominoType);
}

void FTNRecorder::RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer,
	const TArray<TArray<FTNCellInfo>>& reversedBuffer)
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordBuffers(normalBuffer, reversedBuffer);
}

void FTNRecorder::RecordLineClear() const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordLineClear();
}

E_TNBehaviorState FTNRecorder::PeekLastBehaviorState() const
{
	if (FieldRecords.IsEmpty())
	{
		return E_TNBehaviorState::None;
	}
	
	return FieldRecords.Last()->PeekLastBehaviorState();
}

void FTNRecorder::PopSpawnValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}

	FieldRecords.Last()->PopSpawnValues(outPosition, outRotationState, outType);
}

void FTNRecorder::PopTransformValues(FVector2D& outPosition, int32& outRotationState) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}

	FieldRecords.Last()->PopTransformValues(outPosition, outRotationState);
}

void FTNRecorder::PopRotateField() const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}

	FieldRecords.Last()->PopRotateField();
}

void FTNRecorder::PopHoldValues(E_TNTetrominoType& outCurrentType, E_TNTetrominoType& outHoldType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}

	FieldRecords.Last()->PopHoldValues(outCurrentType, outHoldType);
}

void FTNRecorder::PopLockDownValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}

	FieldRecords.Last()->PopLockDownValues(outPosition, outRotationState, outType);
}

void FTNRecorder::PopLineClearBuffers(TArray<TArray<FTNCellInfo>>& outNormalBuffer, TArray<TArray<FTNCellInfo>>& outReversedBuffer) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}

	FieldRecords.Last()->PopLineClearBuffers(outNormalBuffer, outReversedBuffer);
}

void FTNRecorder::PopFieldRecord()
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Pop();
}
