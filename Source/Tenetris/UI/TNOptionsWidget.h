// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNOptionsWidget.generated.h"

class UButton;
class UTNAudioSettingsWidget;
class UTNControlsSettingsWidget;

// Fired when the user presses the close/back button.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTNOptionsCloseRequested);

// Options container with Audio / Controls sections.
//
// Reusable from the title menu and the in-game pause menu. The visual layout lives in a
// WBP subclass whose widgets must be named exactly like the BindWidget properties below.
// Only one section is visible at a time; the tab buttons switch between them.
UCLASS()
class TENETRIS_API UTNOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Refreshes both sections and shows the Audio section (call when the panel is shown).
	UFUNCTION(BlueprintCallable, Category = "Options")
	void RefreshFromSettings();

	UFUNCTION(BlueprintCallable, Category = "Options")
	void ShowAudioSection();

	UFUNCTION(BlueprintCallable, Category = "Options")
	void ShowControlsSection();

public:
	UPROPERTY(BlueprintAssignable, Category = "Options")
	FTNOptionsCloseRequested OnCloseRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void onAudioTabClicked();

	UFUNCTION()
	void onControlsTabClicked();

	UFUNCTION()
	void onCloseClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTNAudioSettingsWidget> AudioSettings;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTNControlsSettingsWidget> ControlsSettings;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AudioTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ControlsTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
};
