// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Tenetris/TenetrisDefine.h"
#include "TenetrisBufferComponent.generated.h"

class ATetrominoCubeBase;

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
	void SetTetrominoCubeClassType(TSubclassOf<ATetrominoCubeBase> InTetrominoCubeClass);
	void SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType);
	void SetVisibilityBackgroundCube(int32 X, int32 Y, bool InVisible);
	void SetTetrominoCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType);
	void SetVisibilityTetrominoCube(int32 X, int32 Y, bool InVisible);
	bool CheckTetrominoCube(int32 X, int32 Y);
	void SetBufferSize(int32 InBufferHeight, int32 InBufferWidth);

protected:
	TArray<TArray<ATetrominoCubeBase*>> BackgroundCubeBuffer;
	TArray<TArray<ATetrominoCubeBase*>> TetrominoCubeBuffer;
	UStaticMeshComponent* BackgroundMeshComponent;
	USceneComponent* BackGroundCubeBufferPivot;
	USceneComponent* TetrominoCubeBufferPivot;
	TSubclassOf<ATetrominoCubeBase> TetrominoCubeClass;
	TArray<TArray<int32>> CheckBuffer;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
};
