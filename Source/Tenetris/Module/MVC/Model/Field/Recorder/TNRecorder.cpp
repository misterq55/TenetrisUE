#include "TNRecorder.h"

void FTNRecorder::FTNFieldRecord::RecordTetrominoType(E_TNTetrominoType tetrominoType)
{
	CurrentTetrominoType = tetrominoType;
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
	NormalBuffer = normalBuffer;
	ReversedBuffer = reversedBuffer;
}

FTNBehavior FTNRecorder::FTNFieldRecord::GetLastBehavior() const
{
	if (Behaviors.IsEmpty())
	{
		return FTNBehavior();
	}
	
	return Behaviors.Last();
}

E_TNTetrominoType FTNRecorder::FTNFieldRecord::GetTetrominoType() const
{
	return CurrentTetrominoType;
}

void FTNRecorder::FTNFieldRecord::Pop()
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
	
	FTNBehavior lastBehavior = CurrentFieldRecord->GetLastBehavior();
	CurrentFieldRecord->Pop();
	
	return lastBehavior;
}

E_TNTetrominoType FTNRecorder::GetTetrominoType() const
{
	if (!CurrentFieldRecord.IsValid())
	{
		return E_TNTetrominoType::None;
	}
	
	return CurrentFieldRecord->GetTetrominoType();
}

void FTNRecorder::Pop()
{
	if (FieldRecords.IsEmpty())
	{
		CurrentFieldRecord = nullptr;
		return;
	}

	CurrentFieldRecord = FieldRecords.Last();
	FieldRecords.Pop();
}
