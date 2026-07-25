#include "TNRecorder.h"

void FTNRecorder::FTNFieldRecord::RecordTetrominoType(E_TNTetrominoType tetrominoType)
{
	TetrominoTypes.Add(tetrominoType);
}

void FTNRecorder::FTNFieldRecord::RecordSpawn()
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Spawn;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordTransform(FVector2D position, int32 rotationState, E_TNBehaviorState reason)
{
	FTNBehavior behavior;
	behavior.BehaviorState = reason;
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

void FTNRecorder::FTNFieldRecord::RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Hold;
	behavior.bCanHold = bCanHold;
	behavior.HoldTetrominoType = holdTetrominoType;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer,
	const TArray<TArray<FTNCellInfo>>& reversedBuffer)
{
	NormalBuffers.Add(normalBuffer);
	ReversedBuffers.Add(reversedBuffer);
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

E_TNTetrominoType FTNRecorder::FTNFieldRecord::ConsumeTetrominoType()
{
	if (TetrominoTypes.IsEmpty())
	{
		return E_TNTetrominoType::None;	
	}
	
	const E_TNTetrominoType tetrominoType = TetrominoTypes.Last();
	TetrominoTypes.Pop();
	
	return tetrominoType;
}

TArray<TArray<FTNCellInfo>> FTNRecorder::FTNFieldRecord::ConsumeNormalBuffer()
{
	if (NormalBuffers.IsEmpty())
	{
		return TArray<TArray<FTNCellInfo>>();
	}

	const TArray<TArray<FTNCellInfo>> normalBuffer = NormalBuffers.Last();
	NormalBuffers.Pop();
	
	return normalBuffer;
}

TArray<TArray<FTNCellInfo>> FTNRecorder::FTNFieldRecord::ConsumeReversedBuffer()
{
	if (ReversedBuffers.IsEmpty())
	{
		return TArray<TArray<FTNCellInfo>>();
	}

	const TArray<TArray<FTNCellInfo>> reversedBuffer = ReversedBuffers.Last();
	ReversedBuffers.Pop();
	
	return reversedBuffer;
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

void FTNRecorder::RecordTetrominoType(E_TNTetrominoType tetrominoType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordTetrominoType(tetrominoType);
}

void FTNRecorder::RecordTransform(FVector2D position, int32 rotationState, E_TNBehaviorState reason) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordTransform(position, rotationState, reason);
}

void FTNRecorder::RecordRotateField(bool bRotateField) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordRotateField(bRotateField);
}

void FTNRecorder::RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType) const
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Last()->RecordHold(bCanHold, holdTetrominoType);
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

E_TNTetrominoType FTNRecorder::ConsumeTetrominoType() const
{
	if (FieldRecords.IsEmpty())
	{
		return E_TNTetrominoType::None;
	}
	
	return FieldRecords.Last()->ConsumeTetrominoType();
}

TArray<TArray<FTNCellInfo>> FTNRecorder::ConsumeNormalBuffer() const
{
	if (FieldRecords.IsEmpty())
	{
		return TArray<TArray<FTNCellInfo>>();
	}
	
	return FieldRecords.Last()->ConsumeNormalBuffer();
}

TArray<TArray<FTNCellInfo>> FTNRecorder::ConsumeReversedBuffer() const
{
	if (FieldRecords.IsEmpty())
	{
		return TArray<TArray<FTNCellInfo>>();
	}
	
	return FieldRecords.Last()->ConsumeReversedBuffer();
}

void FTNRecorder::PopFieldRecord()
{
	if (FieldRecords.IsEmpty())
	{
		return;
	}
	
	FieldRecords.Pop();
}
