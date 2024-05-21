// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TNTenetrisGameModeBase.h"
#include "TNTenetrisGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TENETRIS_API ATNTenetrisGameMode : public ATNTenetrisGameModeBase
{
	GENERATED_BODY()	

public:
	ATNTenetrisGameMode();

	virtual void StartPlay() override;
	virtual void Tick(float deltaSeconds) override;
};
