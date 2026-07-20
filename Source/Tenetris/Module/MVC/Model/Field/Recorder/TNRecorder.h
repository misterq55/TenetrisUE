#pragma once

#include "Tenetris/Common/TenetrisType.h"
#include "CoreMinimal.h"

class FTNRecorder
{
private:
	enum E_TNBehaviorState
	{
		Move,
		Rotate,
		Hold,
		RotateField,
		None,
	};
	
	struct FTNBehavior
	{
		FTNBehavior() = default;
		~FTNBehavior() = default;
		
		E_TNBehaviorState BehaviorState = E_TNBehaviorState::None;
		FVector2D Position = FVector2D::ZeroVector;
		bool bRotateField = false;
		bool bHold = false;
		int32 RotationState = 0;
	};
	
	class FTNFieldRecord
	{
	public:
		FTNFieldRecord() = default;
		~FTNFieldRecord() = default;
	
	public:
		void RecordTetrominoType(E_TNTetrominoType tetrominoType);
		void RecordPosition(FVector2D position);
		void RecordRotation(int32 rotationState);
		void RecordRotateField(bool bRotateField);
		// void RecordHold(bool bHold);
		void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	
	private:
		TArray<FTNBehavior> Behaviors;
		E_TNTetrominoType CurrentTetrominoType;
		// E_TNTetrominoType HoldTetrominoType;
		// TArray<E_TNTetrominoType> PreviewTetrominoTypes;
		TArray<TArray<FTNCellInfo>> NormalBuffer;
		TArray<TArray<FTNCellInfo>> ReversedBuffer;
	};
	
public:
	FTNRecorder();
	~FTNRecorder();
	
public:
	void Initialize();
	void RecordTetrominoType(E_TNTetrominoType tetrominoType) const;
	void RecordPosition(FVector2D Position) const;
	void RecordRotation(int32 RotationState) const;
	void RecordRotateField(bool bRotateField) const;
	// void RecordHold(bool bHold) const;
	void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	
private:
	static constexpr int32 MaxRecords = 2;
	TSharedPtr<FTNFieldRecord> CurrentFieldRecord;
	TArray<TSharedPtr<FTNFieldRecord>> FieldRecords;
};

