// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TNTenetrisGameModeBase.generated.h"

/**
 * 
 */

class ATNFieldBase;

UCLASS(Blueprintable)
class TENETRIS_API ATNTenetrisGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	virtual void AddFields(ATNFieldBase* field);
	
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void SetPlayerField(ATNFieldBase* playerField);

protected:
	TArray<ATNFieldBase*> Fields;
};
