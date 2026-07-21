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
		void RecordTetrominoType(E_TNTetrominoType tetrominoType);
		void RecordSpawn();
		void RecordTransform(FVector2D position, int32 rotationState);
		void RecordRotateField(bool bRotateField);
		void RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType);
		void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);

	public:
		FTNBehavior ConsumeLastBehavior() const;
		E_TNTetrominoType ConsumeTetrominoType();
		TArray<TArray<FTNCellInfo>> GetNormalBuffer() const;
		TArray<TArray<FTNCellInfo>> GetReversedBuffer() const;
		void PopBehavior();

	public:
		bool IsEmpty() const
		{
			return Behaviors.IsEmpty();
		}

	private:
		TArray<FTNBehavior> Behaviors;
		TArray<E_TNTetrominoType> TetrominoTypes;
		TArray<TArray<FTNCellInfo>> NormalBuffer;
		TArray<TArray<FTNCellInfo>> ReversedBuffer;
	};

public:
	FTNRecorder() = default;
	~FTNRecorder() = default;

public:
	void Initialize();
	void RecordTetrominoType(E_TNTetrominoType tetrominoType) const;
	void RecordSpawn() const;
	void RecordTransform(FVector2D position, int32 rotationState) const;
	void RecordRotateField(bool bRotateField) const;
	void RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType) const;
	void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);

public:
	FTNBehavior ConsumeLastBehavior() const;
	E_TNTetrominoType ConsumeTetrominoType() const;
	TArray<TArray<FTNCellInfo>> ConsumeNormalBuffer() const;
	TArray<TArray<FTNCellInfo>> ConsumeReversedBuffer() const;
	void PopFieldRecord();

	bool IsEmpty() const
	{
		return !CurrentFieldRecord.IsValid();
	}

private:
	static constexpr int32 MaxRecords = 2;
	TSharedPtr<FTNFieldRecord> CurrentFieldRecord;
	TArray<TSharedPtr<FTNFieldRecord>> FieldRecords;
};
