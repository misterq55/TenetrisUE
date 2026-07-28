#pragma once

#include "Tenetris/Common/TenetrisType.h"
#include "CoreMinimal.h"

class FTNRecorder
{
private:
	
	class FTNFieldRecord
	{
	public:
		FTNFieldRecord() = default;
		~FTNFieldRecord() = default;

	public:
		void RecordSpawn(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType);
		void RecordTransform(FVector2D position, int32 rotationState);
		void RecordLockDown(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType);
		void RecordRotateField(bool bRotateField);
		void RecordHold(E_TNTetrominoType currentTetrominoType, E_TNTetrominoType holdTetrominoType);
		void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
		void RecordLockDown();
		void RecordLineClear();

	public:
		FTNBehavior ConsumeLastBehavior() const;
		const TArray<TArray<FTNCellInfo>>& GetNormalBuffer() const;
		const TArray<TArray<FTNCellInfo>>& GetReversedBuffer() const;
		void PopBehavior();

	public:
		bool IsEmpty() const
		{
			return Behaviors.IsEmpty();
		}

	private:
		TArray<FTNBehavior> Behaviors;
		TArray<TArray<FTNCellInfo>> NormalBuffer;
		TArray<TArray<FTNCellInfo>> ReversedBuffer;
	};

public:
	FTNRecorder() = default;
	~FTNRecorder() = default;

public:
	void AddFieldRecord();
	void RecordSpawn(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType) const;
	void RecordTransform(FVector2D position, int32 rotationState) const;
	void RecordLockDown(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType);
	void RecordRotateField(bool bRotateField) const;
	void RecordHold(E_TNTetrominoType currentTetrominoType, E_TNTetrominoType holdTetrominoType) const;
	void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	void RecordLineClear() const;

public:
	FTNBehavior ConsumeLastBehavior() const;
	const TArray<TArray<FTNCellInfo>>& GetNormalBuffer() const;
	const TArray<TArray<FTNCellInfo>>& GetReversedBuffer() const;
	void PopFieldRecord();

	bool IsEmpty() const
	{
		return FieldRecords.IsEmpty();
	}

private:
	static constexpr int32 MaxRecords = 2;
	TArray<TSharedPtr<FTNFieldRecord>> FieldRecords;
};
