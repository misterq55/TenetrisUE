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

		// behavior별 pop: 필요한 값들을 out-param으로 채운 뒤 해당 behavior를 소비한다.
		// 사전조건은 각 함수 내부의 ensure로 검증한다.
		void PopSpawnValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType);
		void PopTransformValues(FVector2D& outPosition, int32& outRotationState);
		void PopRotateField();
		void PopHoldValues(E_TNTetrominoType& outCurrentType, E_TNTetrominoType& outHoldType);
		void PopLockDownValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType);
		void PopLineClearBuffers(TArray<TArray<FTNCellInfo>>& outNormalBuffer, TArray<TArray<FTNCellInfo>>& outReversedBuffer);

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

	// behavior별 pop: FTNFieldRecord로 위임한다. FieldRecords가 비어 있으면 no-op이며 out-param은 기본값을 갖는다.
	void PopSpawnValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType) const;
	void PopTransformValues(FVector2D& outPosition, int32& outRotationState) const;
	void PopRotateField() const;
	void PopHoldValues(E_TNTetrominoType& outCurrentType, E_TNTetrominoType& outHoldType) const;
	void PopLockDownValues(FVector2D& outPosition, int32& outRotationState, E_TNTetrominoType& outType) const;
	void PopLineClearBuffers(TArray<TArray<FTNCellInfo>>& outNormalBuffer, TArray<TArray<FTNCellInfo>>& outReversedBuffer) const;

	void PopFieldRecord();

	bool IsEmpty() const
	{
		return FieldRecords.IsEmpty();
	}

private:
	static constexpr int32 MaxRecords = 2;
	TArray<TSharedPtr<FTNFieldRecord>> FieldRecords;
};
