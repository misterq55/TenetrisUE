// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TenetrisGameModeBase.generated.h"

/**
 * 
 */

class ATenetrisFieldBase;

UCLASS(Blueprintable)
class TENETRIS_API ATenetrisGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void Initialize();
	
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void AddTenetrisFields(ATenetrisFieldBase* InTenetrisField);
	
	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
	void SetPlayerTenetrisField(ATenetrisFieldBase* InPlayerTenetrisField);

	UFUNCTION(BlueprintCallable, Category = "TenetrisGameMode")
		ATenetrisFieldBase* GetPlayerTenetrisField();

private:
	TArray<ATenetrisFieldBase*> TenetrisFields;
	ATenetrisFieldBase* PlayerTenetrisField;
};
