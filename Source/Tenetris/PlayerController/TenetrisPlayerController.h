// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tenetris/TenetrisDefine.h"
#include "TenetrisPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class TENETRIS_API ATenetrisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;
};
