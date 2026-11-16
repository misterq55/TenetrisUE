// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNOptionsWidget.h"
#include "Components/Button.h"
#include "Tenetris/Input/TNInputDeviceSubsystem.h"
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

	if (UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this))
	{
		inputDevice->OnInputDeviceChanged.AddUObject(this, &UTNOptionsWidget::handleInputDeviceChanged);
	}

	updateTabHints();
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

	if (UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this))
	{
		inputDevice->OnInputDeviceChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

UWidget* UTNOptionsWidget::GetDefaultFocusWidget() const
{
	if (isAudioSectionShown())
	{
		return AudioSettings->GetFirstFocusWidget();
	}

	return IsValid(ControlsSettings) ? ControlsSettings->GetFirstFocusWidget() : nullptr;
}

bool UTNOptionsWidget::HandleBack()
{
	onCloseClicked();
	return true;
}

bool UTNOptionsWidget::HandleMenuKey(const FKey& key)
{
	if (key == EKeys::Gamepad_LeftShoulder)
	{
		if (!isAudioSectionShown())
		{
			ShowAudioSection();
			FocusMenu();
		}

		return true;
	}

	if (key == EKeys::Gamepad_RightShoulder)
	{
		if (isAudioSectionShown())
		{
			ShowControlsSection();
			FocusMenu();
		}

		return true;
	}

	return false;
}

bool UTNOptionsWidget::isAudioSectionShown() const
{
	return IsValid(AudioSettings) && AudioSettings->IsVisible();
}

void UTNOptionsWidget::handleInputDeviceChanged(const E_TNInputDevice device)
{
	updateTabHints();
}

void UTNOptionsWidget::updateTabHints()
{
	const UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this);
	const ESlateVisibility hintVisibility = inputDevice && inputDevice->IsUsingGamepad()
		? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Hidden;

	if (IsValid(PrevTabHint))
	{
		PrevTabHint->SetVisibility(hintVisibility);
	}

	if (IsValid(NextTabHint))
	{
		NextTabHint->SetVisibility(hintVisibility);
	}
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
