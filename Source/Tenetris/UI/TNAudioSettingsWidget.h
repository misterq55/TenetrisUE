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

	// Item focused when the section is entered with keyboard/gamepad.
	UWidget* GetFirstFocusWidget() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& myGeometry, float deltaTime) override;

private:
	UTNAudioSubsystem* getAudioSubsystem() const;

	// Lets a gamepad / keyboard change the slider without first "locking" it with Accept.
	void setupSlider(USlider* slider);

	UFUNCTION()
	void onMasterChanged(float value);

	UFUNCTION()
	void onBgmChanged(float value);

	UFUNCTION()
	void onSfxChanged(float value);

	// Restarts the settle countdown; see SettleDelay.
	void scheduleSettle();

	// Volume stopped changing: persist it and play a sample so the SFX level can be heard.
	void onVolumeSettled();

	// Seconds a volume must stay unchanged before it is saved and the sample plays, so a
	// held stick or a mouse drag produces one sample instead of one per step.
	static constexpr float SettleDelay = 0.3f;

	// Remaining settle time; negative while nothing is pending.
	float SettleCountdown = -1.f;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MasterSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> BGMSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SFXSlider;
};
