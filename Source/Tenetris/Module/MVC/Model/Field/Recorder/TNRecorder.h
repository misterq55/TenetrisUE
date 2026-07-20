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
		int32 RotationState = 0;
	};
	
	class FTNFieldRecord
	{
	public:
		FTNFieldRecord();
		~FTNFieldRecord();
	
	public:
		void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	
	private:
		TArray<FTNBehavior> Behaviors;
		E_TNTetrominoType HoldTetrominoType;
		TArray<E_TNTetrominoType> PreviewTetrominoTypes;
		TArray<TArray<FTNCellInfo>> NormalBuffer;
		TArray<TArray<FTNCellInfo>> ReversedBuffer;
	};
	
public:
	FTNRecorder();
	~FTNRecorder();
	
public:
	void Initialize();
	void RecordBuffers(const TArray<TArray<FTNCellInfo>>& normalBuffer, const TArray<TArray<FTNCellInfo>>& reversedBuffer);
	
private:
	static constexpr int32 MaxRecords = 2;
	TSharedPtr<FTNFieldRecord> CurrentFieldRecord;
	TArray<TSharedPtr<FTNFieldRecord>> FieldRecords;
};

