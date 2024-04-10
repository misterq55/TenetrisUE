// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TenetrisGameModeBase.generated.h"

/**
 * 
 */

class AFieldBase;

UCLASS(Blueprintable)
class TENETRIS_API ATenetrisGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void Initialize();
	
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void AddFields(AFieldBase* field);
	
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void SetPlayerField(AFieldBase* playerField);

	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
		AFieldBase* GetPlayerField();

private:
	TArray<AFieldBase*> Fields;
	AFieldBase* PlayerField;
};
