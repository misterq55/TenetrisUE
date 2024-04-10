// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Tenetris/TenetrisDefine.h"
#include "TenetrisBufferComponent.generated.h"

class AMinoBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TENETRIS_API UTenetrisBufferComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTenetrisBufferComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual	void Initialize();
	void SetMinoClassType(TSubclassOf<AMinoBase> minoClass);
	void SetBackgroundCubeType(int32 x, int32 y, ETetrominoType tetrominoType);
	void SetVisibilityBackgroundCube(int32 x, int32 y, bool visible);
	void SetMinoType(int32 x, int32 y, ETetrominoType tetrominoType);
	void SetVisibilityMino(int32 x, int32 y, bool visible);
	bool CheckMino(int32 x, int32 y);
	void SetBufferSize(int32 bufferHeight, int32 bufferWidth);
	int32 CalculateGuideMinoHeight(int32 x, int32 y);
	void CheckLineDelete(TArray<int32> heights);
	ETetrominoType GetValueFromCheckBuffer(int32 x, int32 y);
	void SetValueToCheckBuffer(int32 x, int32 y, ETetrominoType tetrominoType);
	bool GetLineDeleting() { return bLineDeleting; }
	void ToggleSpaceInversion();

protected:
	TArray<TArray<AMinoBase*>> BackgroundCubeBuffer;
	TArray<TArray<AMinoBase*>> MinoBuffer;
	UStaticMeshComponent* BackgroundMeshComponent;
	USceneComponent* BackGroundMinoBufferPivot;
	USceneComponent* MinoBufferPivot;
	TSubclassOf<AMinoBase> MinoClass;
	TArray<TArray<ETetrominoType>> CheckBuffer;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	TArray<int32> DeletedLines;
	float DeleteLineCheckTime = 0.f;
	bool bLineDeleting = false;
	bool bSpaceInverted = false;
};
