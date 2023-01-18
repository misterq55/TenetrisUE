// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/TenetrisDefine.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "TenetrisFieldBase.generated.h"

class FTetrominoBase;
class ATetrominoCubeBase;
class UTenetrisBufferComponent;

UCLASS()
class TENETRIS_API ATenetrisFieldBase : public AActor
{
	GENERATED_BODY()

		friend class FTetrominoBase;

public:	
	// Sets default values for this actor's properties
	ATenetrisFieldBase();
	virtual ~ATenetrisFieldBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "TenetrisField")
		virtual	void Initialize();

	void SetTetrominoCubeClassType(TSubclassOf<ATetrominoCubeBase> InTetrominoCubeClass);

protected:
	void SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType);

protected:
	TSubclassOf<ATetrominoCubeBase> TetrominoCubeClass;
	UTenetrisBufferComponent* TenetrisBufferComponent;
};
