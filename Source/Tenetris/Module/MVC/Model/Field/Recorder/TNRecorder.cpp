#include "TNRecorder.h"

void FTNRecorder::FTNFieldRecord::RecordTetrominoType(E_TNTetrominoType tetrominoType)
{
	CurrentTetrominoType = tetrominoType;
}

void FTNRecorder::FTNFieldRecord::RecordPosition(FVector2D Position)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Move;
	behavior.Position = Position;
	Behaviors.Add(behavior);
}

void FTNRecorder::FTNFieldRecord::RecordRotation(int32 rotationState)
{
	FTNBehavior behavior;
	behavior.BehaviorState = E_TNBehaviorState::Rotate;
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

// void FTNRecorder::FTNFieldRecord::RecordHold(bool bHold)
// {
// 	FTNBehavior behavior;
// 	behavior.BehaviorState = E_TNBehaviorState::Hold;
// 	Behaviors.Add(behavior);
// }

void FTNRecorder::FTNFieldRecord::RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer,
                                                const TArray<TArray<FTNCellInfo>>& reversedBuffer)
{
	NormalBuffer = normalBuffer;
	ReversedBuffer = reversedBuffer;
}

FTNRecorder::FTNRecorder()
{
}

FTNRecorder::~FTNRecorder()
{
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

void FTNRecorder::RecordPosition(FVector2D Position) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordPosition(Position);
	}
}

void FTNRecorder::RecordRotation(int32 rotationState) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordRotation(rotationState);
	}
}

void FTNRecorder::RecordRotateField(bool bRotateField) const
{
	if (CurrentFieldRecord.IsValid())
	{
		CurrentFieldRecord->RecordRotateField(bRotateField);
	}
}

// void FTNRecorder::RecordHold(bool bHold) const
// {
// 	if (CurrentFieldRecord.IsValid())
// 	{
// 		CurrentFieldRecord->RecordHold(bHold);
// 	}
// }

void FTNRecorder::RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer)
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
