// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/Actors/Field/TNFieldActorInterface.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNFieldBase.generated.h"

UCLASS(Abstract)
class TENETRIS_API ATNFieldBase : public AActor, public ITNFieldActorInterface
{
	GENERATED_BODY()

public:
	ATNFieldBase();
	virtual ~ATNFieldBase();

	virtual void Initialize() PURE_VIRTUAL(ATNFieldBase::Initialize, );
	virtual void HandleFieldState(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) PURE_VIRTUAL(ATNFieldBase::HandleFieldState, );
	virtual void ApplyPreviewVisible(bool bVisible) PURE_VIRTUAL(ATNFieldBase::ApplyPreviewVisible, );
	virtual void ApplyHoldVisible(bool bVisible) PURE_VIRTUAL(ATNFieldBase::ApplyHoldVisible, );
};
