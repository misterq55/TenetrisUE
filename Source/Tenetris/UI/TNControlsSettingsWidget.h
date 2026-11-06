// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNControlsSettingsWidget.generated.h"

// Key binding settings panel, hosted by UTNOptionsWidget.
//
// Skeleton only: the in-game rebinding (Enhanced Input user settings) is added later.
UCLASS()
class TENETRIS_API UTNControlsSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Re-reads the current bindings into the UI (call when the panel is shown).
	UFUNCTION(BlueprintCallable, Category = "Options")
	void RefreshFromSettings();

	// Persists the current bindings.
	UFUNCTION(BlueprintCallable, Category = "Options")
	void SaveSettings();
};
