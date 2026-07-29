#pragma once

#include "Tenetris/Common/TenetrisType.h"
#include "CoreMinimal.h"

class FTNRecorder
{
private:
	struct FTNBehavior
	{
		E_TNBehaviorState BehaviorState = E_TNBehaviorState::None;
		FVector2D Position = FVector2D::ZeroVector;
		E_TNTetrominoType CurrentTetrominoType = E_TNTetrominoType::None;
		E_TNTetrominoType HoldTetrominoType = E_TNTetrominoType::None;
		int32 RotationState = 0;
	};

	class FTNFieldRecord
	{
	public:
		FTNFieldRecord() = default;
		~FTNFieldRecord() = default;

	public:
		void RecordSpawn(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType);
		void RecordTransform(FVector2D position, int32 rotationState);
		void RecordLockDown(FVector2D position, int32 rotationState, E_TNTetrominoType tetrominoType);
		void RecordRotateField();
		void RecordHold(E_TNTetrominoType currentTetrominoType, E_TNTetrominoType holdTetrominoType);
		void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
		void RecordLineClear();

	public:
		E_TNBehaviorState PeekLastBehaviorState() const;
		FVector2D GetLastPosition() const;
		int32 GetLastRotationState() const;
		E_TNTetrominoType GetLastCurrentTetrominoType() const;
		E_TNTetrominoType GetLastHoldTetrominoType() const;
		void PopLastBehavior();

		const TArray<TArray<FTNCellInfo>>& GetNormalBuffer() const;
		const TArray<TArray<FTNCellInfo>>& GetReversedBuffer() const;

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
	void RecordRotateField() const;
	void RecordHold(E_TNTetrominoType currentTetrominoType, E_TNTetrominoType holdTetrominoType) const;
	void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	void RecordLineClear() const;

public:
	E_TNBehaviorState PeekLastBehaviorState() const;
	FVector2D GetLastPosition() const;
	int32 GetLastRotationState() const;
	E_TNTetrominoType GetLastCurrentTetrominoType() const;
	E_TNTetrominoType GetLastHoldTetrominoType() const;
	void PopLastBehavior() const;

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
