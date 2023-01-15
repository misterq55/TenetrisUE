// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/TenetrisDefine.h"
#include "TenetrisFieldBase.generated.h"

class FTetrominoBase;
class ATetrominoCubeBase;

UCLASS()
class TENETRIS_API ATenetrisFieldBase : public AActor
{
	GENERATED_BODY()
	
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

protected:
	const int32 RowMax = 20;
	const int32 ColumnMax = 10;

	TArray<TArray<ATetrominoCubeBase*>> CubeBuffer;

	TSubclassOf<ATetrominoCubeBase> TetrominoCubeClass;
};
