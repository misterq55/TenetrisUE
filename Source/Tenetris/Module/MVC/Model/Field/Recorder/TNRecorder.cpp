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

FVector2D FTNRecorder::FTNFieldRecord::GetLastPosition() const
{
	if (Behaviors.IsEmpty())
	{
		return FVector2D::ZeroVector;
	}
	
	return Behaviors.Last().Position;
}

int32 FTNRecorder::FTNFieldRecord::GetLastRotationState() const
{
	if (Behaviors.IsEmpty())
	{
		return 0;
	}
	
	return Behaviors.Last().RotationState;
}

E_TNTetrominoType FTNRecorder::FTNFieldRecord::GetLastCurrentTetrominoType() const
{
	if (Behaviors.IsEmpty())
	{
		return E_TNTetrominoType::None;
	}
	
	return Behaviors.Last().CurrentTetrominoType;
}

E_TNTetrominoType FTNRecorder::FTNFieldRecord::GetLastHoldTetrominoType() const
{
	if (Behaviors.IsEmpty())
	{
		return E_TNTetrominoType::None;
	}
	
	return Behaviors.Last().HoldTetrominoType;
}

const TArray<TArray<FTNCellInfo>>& FTNRecorder::FTNFieldRecord::GetNormalBuffer() const
{
	return NormalBuffer;
}

const TArray<TArray<FTNCellInfo>>& FTNRecorder::FTNFieldRecord::GetReversedBuffer() const
{
	return ReversedBuffer;
}

void FTNRecorder::FTNFieldRecord::PopLastBehavior()
{
	if (Behaviors.IsEmpty())
	{
		return;
	}

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

FVector2D FTNRecorder::GetLastPosition() const
{
	if (FieldRecords.IsEmpty())
	{
		return FVector2D::ZeroVector;
	}
	
	return FieldRecords.Last()->GetLastPosition();
}

int32 FTNRecorder::GetLastRotationState() const
{
	if (FieldRecords.IsEmpty())
	{
		return 0;
	}
	
	return FieldRecords.Last()->GetLastRotationState();
}

E_TNTetrominoType FTNRecorder::GetLastCurrentTetrominoType() const
{
	if (FieldRecords.IsEmpty())
	{
		return E_TNTetrominoType::None;
	}
	
	return FieldRecords.Last()->GetLastCurrentTetrominoType();
}

E_TNTetrominoType FTNRecorder::GetLastHoldTetrominoType() const
{
	if (FieldRecords.IsEmpty())
	{
		return E_TNTetrominoType::None;
	}
	
	return FieldRecords.Last()->GetLastHoldTetrominoType();
}

void FTNRecorder::PopLastBehavior() const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->PopLastBehavior();
}

const TArray<TArray<FTNCellInfo>>& FTNRecorder::GetNormalBuffer() const
{
	static const TArray<TArray<FTNCellInfo>> EmptyBuffer;
	if (FieldRecords.IsEmpty())
	{
		return EmptyBuffer;
	}
	
	return FieldRecords.Last()->GetNormalBuffer();
}

const TArray<TArray<FTNCellInfo>>& FTNRecorder::GetReversedBuffer() const
{
	static const TArray<TArray<FTNCellInfo>> EmptyBuffer;
	if (FieldRecords.IsEmpty())
	{
		return EmptyBuffer;
	}
	
	return FieldRecords.Last()->GetReversedBuffer();
}

void FTNRecorder::PopFieldRecord()
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Pop();
}
