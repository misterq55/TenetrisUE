// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNAudioSettingsWidget.h"
#include "Components/Slider.h"
#include "Tenetris/Audio/TNAudioSubsystem.h"
#include "Tenetris/Audio/TNAudioData.h"

void UTNAudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(MasterSlider))
	{
		MasterSlider->OnValueChanged.AddDynamic(this, &UTNAudioSettingsWidget::onMasterChanged);
		MasterSlider->OnMouseCaptureEnd.AddDynamic(this, &UTNAudioSettingsWidget::onSliderReleased);
	}

	if (IsValid(BGMSlider))
	{
		BGMSlider->OnValueChanged.AddDynamic(this, &UTNAudioSettingsWidget::onBgmChanged);
		BGMSlider->OnMouseCaptureEnd.AddDynamic(this, &UTNAudioSettingsWidget::onSliderReleased);
	}

	if (IsValid(SFXSlider))
	{
		SFXSlider->OnValueChanged.AddDynamic(this, &UTNAudioSettingsWidget::onSfxChanged);
		SFXSlider->OnMouseCaptureEnd.AddDynamic(this, &UTNAudioSettingsWidget::onSliderReleased);
	}

	RefreshFromSettings();
}

void UTNAudioSettingsWidget::NativeDestruct()
{
	if (IsValid(MasterSlider))
	{
		MasterSlider->OnValueChanged.RemoveDynamic(this, &UTNAudioSettingsWidget::onMasterChanged);
		MasterSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UTNAudioSettingsWidget::onSliderReleased);
	}

	if (IsValid(BGMSlider))
	{
		BGMSlider->OnValueChanged.RemoveDynamic(this, &UTNAudioSettingsWidget::onBgmChanged);
		BGMSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UTNAudioSettingsWidget::onSliderReleased);
	}

	if (IsValid(SFXSlider))
	{
		SFXSlider->OnValueChanged.RemoveDynamic(this, &UTNAudioSettingsWidget::onSfxChanged);
		SFXSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UTNAudioSettingsWidget::onSliderReleased);
	}

	Super::NativeDestruct();
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
}

void UTNAudioSettingsWidget::onBgmChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetBGMVolume(value);
	}
}

void UTNAudioSettingsWidget::onSfxChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetSFXVolume(value);
	}
}

void UTNAudioSettingsWidget::onSliderReleased()
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
