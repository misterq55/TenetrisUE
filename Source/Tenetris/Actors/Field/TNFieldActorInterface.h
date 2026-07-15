// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNFieldActorInterface.generated.h"

UINTERFACE(BlueprintType)
class TENETRIS_API UTNFieldActorInterface : public UInterface
{
	GENERATED_BODY()
};

class TENETRIS_API ITNFieldActorInterface
{
	GENERATED_BODY()

public:
	virtual void Initialize() = 0;
	virtual void HandleFieldState(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) = 0;
	virtual void ApplyPreviewVisible(bool bVisible) = 0;
	virtual void ApplyHoldVisible(bool bVisible) = 0;
};
