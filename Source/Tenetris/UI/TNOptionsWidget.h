// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/UI/TNMenuWidgetBase.h"
#include "TNOptionsWidget.generated.h"

class UButton;
class UTNAudioSettingsWidget;
class UTNControlsSettingsWidget;
enum class E_TNInputDevice : uint8;

// Fired when the user presses the close/back button.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTNOptionsCloseRequested);

// Options container with Audio / Controls sections.
//
// Reusable from the title menu and the in-game pause menu. The visual layout lives in a
// WBP subclass whose widgets must be named exactly like the BindWidget properties below.
// Only one section is visible at a time; the tab buttons (or LB / RB) switch between them,
// and Back (gamepad B / Esc) closes the panel like the close button.
UCLASS()
class TENETRIS_API UTNOptionsWidget : public UTNMenuWidgetBase
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

	virtual UWidget* GetDefaultFocusWidget() const override;
	virtual bool HandleBack() override;
	virtual bool HandleMenuKey(const FKey& key) override;

private:
	UFUNCTION()
	void onAudioTabClicked();

	UFUNCTION()
	void onControlsTabClicked();

	UFUNCTION()
	void onCloseClicked();

	void handleInputDeviceChanged(const E_TNInputDevice device);
	void updateTabHints();

	bool isAudioSectionShown() const;

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

	// Optional "LB" / "RB" labels next to the tabs; shown only while a gamepad is in use.
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> PrevTabHint;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> NextTabHint;
};
