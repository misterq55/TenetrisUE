#include "TNRecorder.h"

FTNRecorder::FTNFieldRecord::FTNFieldRecord()
{
}

FTNRecorder::FTNFieldRecord::~FTNFieldRecord()
{
}

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
	CurrentFieldRecord = MakeShareable(new FTNFieldRecord());
}

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