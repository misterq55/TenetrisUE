// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNTenetrisBufferComponent.generated.h"

class ATNMinoBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TENETRIS_API UTNTenetrisBufferComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTNTenetrisBufferComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual	void Initialize();
	void SetBufferSize(const int32 bufferHeight, const int32 bufferWidth);
	void SetMinoClassType(const TSubclassOf<ATNMinoBase> minoClass);
	void SetBackgroundCubeType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible);
	void SetMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityMino(const int32 x, const int32 y, const bool visible);

private:
	TArray<TArray<ATNMinoBase*>> BackgroundCubeBuffer;
	TArray<TArray<ATNMinoBase*>> MinoBuffer;
	UStaticMeshComponent* BackgroundMeshComponent;
	USceneComponent* BackGroundMinoBufferPivot;
	USceneComponent* MinoBufferPivot;
	TSubclassOf<ATNMinoBase> MinoClass;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	bool bSpaceInverted = false;
};
