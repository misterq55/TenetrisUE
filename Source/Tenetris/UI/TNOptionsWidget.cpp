// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNOptionsWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Tenetris/Audio/TNAudioSubsystem.h"
#include "Tenetris/Audio/TNAudioData.h"

void UTNOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(MasterSlider))
	{
		MasterSlider->OnValueChanged.AddDynamic(this, &UTNOptionsWidget::onMasterChanged);
		MasterSlider->OnMouseCaptureEnd.AddDynamic(this, &UTNOptionsWidget::onSliderReleased);
	}

	if (IsValid(BGMSlider))
	{
		BGMSlider->OnValueChanged.AddDynamic(this, &UTNOptionsWidget::onBgmChanged);
		BGMSlider->OnMouseCaptureEnd.AddDynamic(this, &UTNOptionsWidget::onSliderReleased);
	}

	if (IsValid(SFXSlider))
	{
		SFXSlider->OnValueChanged.AddDynamic(this, &UTNOptionsWidget::onSfxChanged);
		SFXSlider->OnMouseCaptureEnd.AddDynamic(this, &UTNOptionsWidget::onSliderReleased);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.AddDynamic(this, &UTNOptionsWidget::onCloseClicked);
	}

	RefreshFromSettings();
}

void UTNOptionsWidget::NativeDestruct()
{
	if (IsValid(MasterSlider))
	{
		MasterSlider->OnValueChanged.RemoveDynamic(this, &UTNOptionsWidget::onMasterChanged);
		MasterSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UTNOptionsWidget::onSliderReleased);
	}

	if (IsValid(BGMSlider))
	{
		BGMSlider->OnValueChanged.RemoveDynamic(this, &UTNOptionsWidget::onBgmChanged);
		BGMSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UTNOptionsWidget::onSliderReleased);
	}

	if (IsValid(SFXSlider))
	{
		SFXSlider->OnValueChanged.RemoveDynamic(this, &UTNOptionsWidget::onSfxChanged);
		SFXSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UTNOptionsWidget::onSliderReleased);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveDynamic(this, &UTNOptionsWidget::onCloseClicked);
	}

	Super::NativeDestruct();
}

UTNAudioSubsystem* UTNOptionsWidget::getAudioSubsystem() const
{
	if (const UGameInstance* gameInstance = GetGameInstance())
	{
		return gameInstance->GetSubsystem<UTNAudioSubsystem>();
	}

	return nullptr;
}

void UTNOptionsWidget::RefreshFromSettings()
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

void UTNOptionsWidget::onMasterChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetMasterVolume(value);
	}
}

void UTNOptionsWidget::onBgmChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetBGMVolume(value);
	}
}

void UTNOptionsWidget::onSfxChanged(const float value)
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SetSFXVolume(value);
	}
}

void UTNOptionsWidget::onSliderReleased()
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SaveVolumes();
		audio->PlaySFX(TNSoundKeys::MenuClick);
	}
}

void UTNOptionsWidget::onCloseClicked()
{
	if (UTNAudioSubsystem* audio = getAudioSubsystem())
	{
		audio->SaveVolumes();
	}

	OnCloseRequested.Broadcast();
}
