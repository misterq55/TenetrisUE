// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNAudioSettingsWidget.h"
#include "Components/Slider.h"
#include "Tenetris/Audio/TNAudioSubsystem.h"
#include "Tenetris/Audio/TNAudioData.h"

namespace
{
	// Volume change per gamepad / keyboard step.
	constexpr float SliderStepSize = 0.05f;
}

void UTNAudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(MasterSlider))
	{
		setupSlider(MasterSlider);
		MasterSlider->OnValueChanged.AddDynamic(this, &UTNAudioSettingsWidget::onMasterChanged);
	}

	if (IsValid(BGMSlider))
	{
		setupSlider(BGMSlider);
		BGMSlider->OnValueChanged.AddDynamic(this, &UTNAudioSettingsWidget::onBgmChanged);
	}

	if (IsValid(SFXSlider))
	{
		setupSlider(SFXSlider);
		SFXSlider->OnValueChanged.AddDynamic(this, &UTNAudioSettingsWidget::onSfxChanged);
	}

	RefreshFromSettings();
}

void UTNAudioSettingsWidget::NativeDestruct()
{
	if (IsValid(MasterSlider))
	{
		MasterSlider->OnValueChanged.RemoveDynamic(this, &UTNAudioSettingsWidget::onMasterChanged);
	}

	if (IsValid(BGMSlider))
	{
		BGMSlider->OnValueChanged.RemoveDynamic(this, &UTNAudioSettingsWidget::onBgmChanged);
	}

	if (IsValid(SFXSlider))
	{
		SFXSlider->OnValueChanged.RemoveDynamic(this, &UTNAudioSettingsWidget::onSfxChanged);
	}

	// The menu can close (e.g. resume from pause) before the countdown ends; keep the change.
	if (SettleCountdown >= 0.f)
	{
		SettleCountdown = -1.f;
		SaveSettings();
	}

	Super::NativeDestruct();
}

void UTNAudioSettingsWidget::NativeTick(const FGeometry& myGeometry, const float deltaTime)
{
	Super::NativeTick(myGeometry, deltaTime);

	if (SettleCountdown < 0.f)
	{
		return;
	}

	SettleCountdown -= deltaTime;
	if (SettleCountdown < 0.f)
	{
		onVolumeSettled();
	}
}

void UTNAudioSettingsWidget::setupSlider(USlider* slider)
{
	slider->RequiresControllerLock = false;
	slider->SetStepSize(SliderStepSize);

	// RequiresControllerLock has no setter; push it to the Slate slider.
	slider->SynchronizeProperties();
}

UWidget* UTNAudioSettingsWidget::GetFirstFocusWidget() const
{
	return MasterSlider;
}

UTNAudioSubsystem* UTNAudioSettingsWidget::getAudioSubsystem() const
{
	if (const UGameInstance* gameInstance = GetGameInstance())
	{
		return gameInstance->GetSubsystem<UTNAudioSubsystem>();
	}

	return nullptr;
}

void UTNAudioSettingsWidget::RefreshFromSettings()
{
	const UTNAudioSubsystem* audio = getAudioSubsystem();
	if (!IsValid(audio))
	{
		return;
	}

	// SetValue does not broadcast OnValueChanged, so this will not re-apply the volumes.
	if (IsValid(MasterSlider))
	{
		MasterSlider->SetValue(audio->GetMasterVolume());
	}

	if (IsValid(BGMSlider))
	{
		BGMSlider->SetValue(audio->GetBGMVolume());
	}

	if (IsValid(SFXSlider))
	{
		SFXSlider->SetValue(audio->GetSFXVolume());
	}
}

void UTNAudioSettingsWidget::onMasterChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetMasterVolume(value);
	}

	scheduleSettle();
}

void UTNAudioSettingsWidget::onBgmChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetBGMVolume(value);
	}

	scheduleSettle();
}

void UTNAudioSettingsWidget::onSfxChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetSFXVolume(value);
	}

	scheduleSettle();
}

void UTNAudioSettingsWidget::scheduleSettle()
{
	SettleCountdown = SettleDelay;
}

void UTNAudioSettingsWidget::onVolumeSettled()
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SaveVolumes();
		audio->PlaySFX(TNSoundKeys::MenuClick);
	}
}

void UTNAudioSettingsWidget::SaveSettings()
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SaveVolumes();
	}
}
