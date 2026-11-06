// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNOptionsWidget.generated.h"

class USlider;
class UButton;
class UTNAudioSubsystem;

// Fired when the user presses the close/back button.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTNOptionsCloseRequested);

// Audio options panel (Master / BGM / SFX volume).
//
// Reusable from the title menu and the in-game pause menu. It only talks to
// UTNAudioSubsystem; the visual layout lives in a WBP subclass whose widgets must be
// named exactly like the BindWidget properties below.
UCLASS()
class TENETRIS_API UTNOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Re-reads the current volumes into the sliders (call when the panel is shown).
	UFUNCTION(BlueprintCallable, Category = "Options")
	void RefreshFromSettings();

public:
	UPROPERTY(BlueprintAssignable, Category = "Options")
	FTNOptionsCloseRequested OnCloseRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UTNAudioSubsystem* getAudioSubsystem() const;

	UFUNCTION()
	void onMasterChanged(float value);

	UFUNCTION()
	void onBgmChanged(float value);

	UFUNCTION()
	void onSfxChanged(float value);

	// Slider released: persist the volumes and play a sample so the SFX level can be heard.
	UFUNCTION()
	void onSliderReleased();

	UFUNCTION()
	void onCloseClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MasterSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> BGMSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SFXSlider;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton;
};
