#include "TNRecorder.h"

void FTNRecorder::FTNFieldRecord::RecordSpawn(FVector2D position, int32 rotationState)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Spawn;
	behavior.Position = position;
	behavior.RotationState = rotationState;
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

void FTNRecorder::RecordSpawn(FVector2D position, int32 rotationState) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordSpawn(position, rotationState);
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

void FTNRecorder::FTNFieldRecord::RecordRotateField(bool bRotateField)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::RotateField;
	behavior.bRotateField = bRotateField;
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

void FTNRecorder::FTNFieldRecord::RecordLockDown()
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::LockDown;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordLineClear()
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::LineClear;
	Behaviors.Add(behavior);
}

FTNBehavior FTNRecorder::FTNFieldRecord::ConsumeLastBehavior() const
{
	if (Behaviors.IsEmpty())
	{
		return FTNBehavior();
	}
	
	return Behaviors.Last();
}

TArray<TArray<FTNCellInfo>> FTNRecorder::FTNFieldRecord::GetNormalBuffer() const
{
	return NormalBuffer;
}

TArray<TArray<FTNCellInfo>> FTNRecorder::FTNFieldRecord::GetReversedBuffer() const
{
	return ReversedBuffer;
}

void FTNRecorder::FTNFieldRecord::PopBehavior()
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
}

void FTNRecorder::RecordTransform(FVector2D position, int32 rotationState) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordTransform(position, rotationState);
}

void FTNRecorder::RecordRotateField(bool bRotateField) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordRotateField(bRotateField);
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

	const int32 recordsNum = FieldRecords.Num();
	if (recordsNum > MaxRecords)
	{
		FieldRecords.RemoveAt(0);
	}
}

void FTNRecorder::RecordLineClear() const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordLineClear();
}

FTNBehavior FTNRecorder::ConsumeLastBehavior() const
{
	if (FieldRecords.IsEmpty())
	{
		return FTNBehavior();
	}
	
	const FTNBehavior lastBehavior = FieldRecords.Last()->ConsumeLastBehavior();
	FieldRecords.Last()->PopBehavior();
	
	return lastBehavior;
}

TArray<TArray<FTNCellInfo>> FTNRecorder::ConsumeNormalBuffer() const
{
	if (FieldRecords.IsEmpty())
	{
		return TArray<TArray<FTNCellInfo>>();
	}
	
	return FieldRecords.Last()->GetNormalBuffer();
}

TArray<TArray<FTNCellInfo>> FTNRecorder::ConsumeReversedBuffer() const
{
	if (FieldRecords.IsEmpty())
	{
		return TArray<TArray<FTNCellInfo>>();
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
