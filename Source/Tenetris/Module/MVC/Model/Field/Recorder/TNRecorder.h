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
		void RecordTransform(FVector2D position, int32 rotationState, E_TNBehaviorState reason = E_TNBehaviorState::Transform);
		void RecordRotateField(bool bRotateField);
		void RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType);
		void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
		void RecordLockDown();
		void RecordLineClear();

	public:
		FTNBehavior ConsumeLastBehavior() const;
		E_TNTetrominoType ConsumeTetrominoType();
		TArray<TArray<FTNCellInfo>> ConsumeNormalBuffer();
		TArray<TArray<FTNCellInfo>> ConsumeReversedBuffer();
		void PopBehavior();

	public:
		bool IsEmpty() const
		{
			return Behaviors.IsEmpty();
		}

	private:
		TArray<FTNBehavior> Behaviors;
		TArray<E_TNTetrominoType> TetrominoTypes;
		TArray<TArray<TArray<FTNCellInfo>>> NormalBuffers;
		TArray<TArray<TArray<FTNCellInfo>>> ReversedBuffers;
	};

public:
	FTNRecorder() = default;
	~FTNRecorder() = default;

public:
	void AddFieldRecord();
	void RecordTetrominoType(E_TNTetrominoType tetrominoType) const;
	void RecordTransform(FVector2D position, int32 rotationState, E_TNBehaviorState reason = E_TNBehaviorState::Transform) const;
	void RecordRotateField(bool bRotateField) const;
	void RecordHold(bool bCanHold, E_TNTetrominoType holdTetrominoType) const;
	void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	void RecordLineClear() const;

public:
	FTNBehavior ConsumeLastBehavior() const;
	E_TNTetrominoType ConsumeTetrominoType() const;
	TArray<TArray<FTNCellInfo>> ConsumeNormalBuffer() const;
	TArray<TArray<FTNCellInfo>> ConsumeReversedBuffer() const;
	void PopFieldRecord();

	bool IsEmpty() const
	{
		return FieldRecords.IsEmpty();
	}

private:
	static constexpr int32 MaxRecords = 2;
	TArray<TSharedPtr<FTNFieldRecord>> FieldRecords;
};
