// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNAudioSettingsWidget.generated.h"

class USlider;
class UTNAudioSubsystem;

// Audio settings panel (Master / BGM / SFX volume), hosted by UTNOptionsWidget.
//
// It only talks to UTNAudioSubsystem; the visual layout lives in a WBP subclass whose
// widgets must be named exactly like the BindWidget properties below.
UCLASS()
class TENETRIS_API UTNAudioSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Re-reads the current volumes into the sliders (call when the panel is shown).
	UFUNCTION(BlueprintCallable, Category = "Options")
	void RefreshFromSettings();

	// Persists the current volumes.
	UFUNCTION(BlueprintCallable, Category = "Options")
	void SaveSettings();

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

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MasterSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> BGMSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SFXSlider;
};
