// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/TenetrisDefine.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "FieldBase.generated.h"

class FTetrominoBase;
class AMinoBase;
class UTenetrisBufferComponent;

UCLASS()
class TENETRIS_API AFieldBase : public AActor
{
	GENERATED_BODY()

		friend class FTetrominoBase;

public:	
	// Sets default values for this actor's properties
	AFieldBase();
	virtual ~AFieldBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Field")
		virtual	void Initialize();

	void SetMinoClassType(TSubclassOf<AMinoBase> minoClass);

protected:
	void setBackgroundCubeType(int32 x, int32 y, ETetrominoType tetrominoType);

protected:
	TSubclassOf<AMinoBase> MinoClass;
	UTenetrisBufferComponent* TenetrisBufferComponent;
};
