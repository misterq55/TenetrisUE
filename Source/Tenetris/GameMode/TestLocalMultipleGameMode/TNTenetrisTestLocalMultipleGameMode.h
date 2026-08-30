// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/TNTenetrisGameModeBase.h"
#include "TNTenetrisTestLocalMultipleGameMode.generated.h"

/**
 *
 */

UCLASS()
class TENETRIS_API ATNTenetrisTestLocalMultipleGameMode : public ATNTenetrisGameModeBase
{
	GENERATED_BODY()

public:
	ATNTenetrisTestLocalMultipleGameMode();
	virtual void StartPlay() override;
	virtual void Tick(float deltaSeconds) override;

private:
	ATNFieldBase* spawnField(const FVector& location, const FVector& scale);

public:
	TSubclassOf<ATNFieldBase> FieldClass;
	
protected:
	UPROPERTY()
	AActor* PlayerFieldPositionActor;

	UPROPERTY()
	TArray<AActor*> FieldsPositionActors;
};
