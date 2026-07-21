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
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::LockDown;
	Behaviors.Add(behavior);
	
	NormalBuffer = normalBuffer;
	ReversedBuffer = reversedBuffer;
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

void FTNRecorder::Initialize()
{
	if (!CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord = MakeShareable(new FTNFieldRecord());
	}
}

void FTNRecorder::RecordTetrominoType(E_TNTetrominoType tetrominoType) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordTetrominoType(tetrominoType);
	}
}

void FTNRecorder::RecordSpawn() const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordSpawn();
	}
}

void FTNRecorder::RecordTransform(FVector2D position, int32 rotationState) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordTransform(position, rotationState);
	}
}

void FTNRecorder::RecordRotateField(bool bRotateField) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordRotateField(bRotateField);
	}
}

void FTNRecorder::RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordHold(bCanHold, holdTetrominoType);
	}
}

void FTNRecorder::RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer,
	const TArray<TArray<FTNCellInfo>>& reversedBuffer)
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordBuffers(normalBuffer, reversedBuffer);
	}

	FieldRecords.Add(CurrentFieldRecord);
	CurrentFieldRecord = MakeShareable(new FTNFieldRecord());

	const int32 recordsNum = FieldRecords.Num();
	if (recordsNum > MaxRecords)
	{
		FieldRecords.RemoveAt(0);
	}
}

FTNBehavior FTNRecorder::ConsumeLastBehavior() const
{
	if (!CurrentFieldRecord.IsValid())
	{
		return FTNBehavior();
	}
	
	const FTNBehavior lastBehavior = CurrentFieldRecord->ConsumeLastBehavior();
	CurrentFieldRecord->PopBehavior();
	
	return lastBehavior;
}

E_TNTetrominoType FTNRecorder::ConsumeTetrominoType() const
{
	if (!CurrentFieldRecord.IsValid())
	{
		return E_TNTetrominoType::None;
	}
	
	return CurrentFieldRecord->ConsumeTetrominoType();
}

TArray<TArray<FTNCellInfo>> FTNRecorder::ConsumeNormalBuffer() const
{
	if (!CurrentFieldRecord.IsValid())
	{
		return TArray<TArray<FTNCellInfo>>();
	}
	
	return CurrentFieldRecord->GetNormalBuffer();
}

TArray<TArray<FTNCellInfo>> FTNRecorder::ConsumeReversedBuffer() const
{
	if (!CurrentFieldRecord.IsValid())
	{
		return TArray<TArray<FTNCellInfo>>();
	}
	
	return CurrentFieldRecord->GetReversedBuffer();
}

void FTNRecorder::PopFieldRecord()
{
	const bool bIsEmpty = FieldRecords.IsEmpty();
	
	CurrentFieldRecord = bIsEmpty ? nullptr : FieldRecords.Last();
	if (!bIsEmpty)
	{
		FieldRecords.Pop();
	}
}
