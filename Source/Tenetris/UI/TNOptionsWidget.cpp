// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNOptionsWidget.h"
#include "Components/Button.h"
#include "Tenetris/UI/TNAudioSettingsWidget.h"
#include "Tenetris/UI/TNControlsSettingsWidget.h"

void UTNOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(AudioTabButton))
	{
		AudioTabButton->OnClicked.AddDynamic(this, &UTNOptionsWidget::onAudioTabClicked);
	}

	if (IsValid(ControlsTabButton))
	{
		ControlsTabButton->OnClicked.AddDynamic(this, &UTNOptionsWidget::onControlsTabClicked);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.AddDynamic(this, &UTNOptionsWidget::onCloseClicked);
	}

	RefreshFromSettings();
}

void UTNOptionsWidget::NativeDestruct()
{
	if (IsValid(AudioTabButton))
	{
		AudioTabButton->OnClicked.RemoveDynamic(this, &UTNOptionsWidget::onAudioTabClicked);
	}

	if (IsValid(ControlsTabButton))
	{
		ControlsTabButton->OnClicked.RemoveDynamic(this, &UTNOptionsWidget::onControlsTabClicked);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveDynamic(this, &UTNOptionsWidget::onCloseClicked);
	}

	Super::NativeDestruct();
}

void UTNOptionsWidget::RefreshFromSettings()
{
	if (IsValid(AudioSettings))
	{
		AudioSettings->RefreshFromSettings();
	}

	if (IsValid(ControlsSettings))
	{
		ControlsSettings->RefreshFromSettings();
	}

	ShowAudioSection();
}

void UTNOptionsWidget::ShowAudioSection()
{
	if (IsValid(AudioSettings))
	{
		AudioSettings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(ControlsSettings))
	{
		ControlsSettings->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTNOptionsWidget::ShowControlsSection()
{
	if (IsValid(AudioSettings))
	{
		AudioSettings->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(ControlsSettings))
	{
		ControlsSettings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UTNOptionsWidget::onAudioTabClicked()
{
	ShowAudioSection();
}

void UTNOptionsWidget::onControlsTabClicked()
{
	ShowControlsSection();
}

void UTNOptionsWidget::onCloseClicked()
{
	if (IsValid(AudioSettings))
	{
		AudioSettings->SaveSettings();
	}

	if (IsValid(ControlsSettings))
	{
		ControlsSettings->SaveSettings();
	}

	OnCloseRequested.Broadcast();
}
