// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/TenetrisDefine.h"
#include "Tenetris/Field/Tetromino/TNTetrominoBase.h"
#include "TNFieldBase.generated.h"

class ATNMinoBase;
class UTNTenetrisBufferComponent;

UCLASS()
class TENETRIS_API ATNFieldBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATNFieldBase();
	virtual ~ATNFieldBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Field")
		virtual	void Initialize();

	void SetMinoClassType(TSubclassOf<ATNMinoBase> minoClass);
	void SetBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType);

protected:
	void setBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType);

protected:
	TSubclassOf<ATNMinoBase> MinoClass;
	UTNTenetrisBufferComponent* TenetrisBufferComponent;
};
