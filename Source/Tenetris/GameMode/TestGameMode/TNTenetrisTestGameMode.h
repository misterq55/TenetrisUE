// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/GameMode/TNTenetrisGameModeBase.h"
#include "TNTenetrisTestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TENETRIS_API ATNTenetrisTestGameMode : public ATNTenetrisGameModeBase
{
	GENERATED_BODY()	

public:
	ATNTenetrisTestGameMode();

	virtual void StartPlay() override;
	virtual void Tick(float deltaSeconds) override;
};
