// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TNTitleGameMode.generated.h"

// Title/main-menu game mode: shows the menu widget and switches input to UI only.
UCLASS()
class TENETRIS_API ATNTitleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	// Title menu widget (e.g. WBP_TitleMenu). Assigned in the BP.
	UPROPERTY(EditDefaultsOnly, Category = "Title")
	TSubclassOf<UUserWidget> TitleMenuWidgetClass;
};
