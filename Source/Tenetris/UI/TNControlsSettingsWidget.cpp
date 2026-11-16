// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNControlsSettingsWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Tenetris/Input/TNInputBindingSubsystem.h"
#include "Tenetris/Input/TNInputDeviceSubsystem.h"
#include "Tenetris/Input/TNKeyBindingSelector.h"
#include "Tenetris/UI/TNFocusRow.h"
#include "Tenetris/UI/TNWidgetFocus.h"

void UTNControlsSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ResetButton))
	{
		ResetButton->OnClicked.AddDynamic(this, &UTNControlsSettingsWidget::onResetClicked);
	}

	if (UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this))
	{
		inputDevice->OnInputDeviceChanged.AddUObject(this, &UTNControlsSettingsWidget::handleInputDeviceChanged);
	}

	buildRows();
	RefreshFromSettings();
}

void UTNControlsSettingsWidget::NativeDestruct()
{
	if (IsValid(ResetButton))
	{
		ResetButton->OnClicked.RemoveDynamic(this, &UTNControlsSettingsWidget::onResetClicked);
	}

	if (UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this))
	{
		inputDevice->OnInputDeviceChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

UWidget* UTNControlsSettingsWidget::GetFirstFocusWidget() const
{
	const E_TNBindingDevice preferredDevice = getPreferredDevice();
	for (UTNKeyBindingSelector* selector : Selectors)
	{
		if (IsValid(selector) && selector->GetDevice() == preferredDevice && selector->GetIsEnabled())
		{
			return selector;
		}
	}

	return ResetButton;
}

void UTNControlsSettingsWidget::RefreshFromSettings()
{
	refreshKeys();
	showMessage(FString());
}

void UTNControlsSettingsWidget::SaveSettings()
{
	if (const UTNInputBindingSubsystem* bindings = getBindingSubsystem())
	{
		bindings->Save();
	}
}

void UTNControlsSettingsWidget::buildRows()
{
	Selectors.Reset();

	if (!IsValid(BindingList))
	{
		UE_LOG(LogTemp, Warning, TEXT("TNControlsSettingsWidget: no BindingList VerticalBox in the widget blueprint; key bindings will not be shown."));
		return;
	}

	const UTNInputBindingSubsystem* bindings = getBindingSubsystem();
	if (!bindings)
	{
		return;
	}

	BindingList->ClearChildren();

	for (const FTNBindableAction& action : bindings->GetBindableActions())
	{
		UHorizontalBox* row = WidgetTree->ConstructWidget<UHorizontalBox>();

		UTextBlock* label = WidgetTree->ConstructWidget<UTextBlock>();
		label->SetText(action.DisplayName);

		UHorizontalBoxSlot* labelSlot = row->AddChildToHorizontalBox(label);
		labelSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		labelSlot->SetVerticalAlignment(VAlign_Center);

		for (const E_TNBindingDevice device : { E_TNBindingDevice::Keyboard, E_TNBindingDevice::Gamepad })
		{
			UHorizontalBoxSlot* selectorSlot = row->AddChildToHorizontalBox(wrapInFocusRow(createSelector(action.MappingName, device)));
			selectorSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			selectorSlot->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));
		}

		UVerticalBoxSlot* rowSlot = BindingList->AddChildToVerticalBox(row);
		rowSlot->SetPadding(FMargin(0.f, 6.f));
	}
}

UTNKeyBindingSelector* UTNControlsSettingsWidget::createSelector(const FName actionName, const E_TNBindingDevice device)
{
	UTNKeyBindingSelector* selector = WidgetTree->ConstructWidget<UTNKeyBindingSelector>();
	selector->Setup(actionName, device);
	selector->OnBindingKeySelected.AddUObject(this, &UTNControlsSettingsWidget::handleBindingKeySelected);

	Selectors.Add(selector);

	return selector;
}

UWidget* UTNControlsSettingsWidget::wrapInFocusRow(UWidget* content)
{
	if (!FocusRowClass)
	{
		return content;
	}

	UTNFocusRow* focusRow = WidgetTree->ConstructWidget<UTNFocusRow>(FocusRowClass);
	focusRow->SetContentForSlot(UTNFocusRow::ContentSlotName, content);
	return focusRow;
}

E_TNBindingDevice UTNControlsSettingsWidget::getPreferredDevice() const
{
	const UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this);
	return inputDevice && inputDevice->IsUsingGamepad() ? E_TNBindingDevice::Gamepad : E_TNBindingDevice::Keyboard;
}

UTNKeyBindingSelector* UTNControlsSettingsWidget::findSelector(const FName actionName, const E_TNBindingDevice device) const
{
	for (UTNKeyBindingSelector* selector : Selectors)
	{
		if (IsValid(selector) && selector->GetActionName() == actionName && selector->GetDevice() == device)
		{
			return selector;
		}
	}

	return nullptr;
}

void UTNControlsSettingsWidget::applyDeviceColumnMode()
{
	const bool bGamepad = getPreferredDevice() == E_TNBindingDevice::Gamepad;

	for (UTNKeyBindingSelector* selector : Selectors)
	{
		if (!IsValid(selector) || selector->GetDevice() != E_TNBindingDevice::Keyboard)
		{
			continue;
		}

		// Still drawn, but out of the hit-test grid, which also removes it from gamepad navigation.
		selector->SetVisibility(bGamepad ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);

		if (bGamepad && (selector->HasAnyUserFocus() || selector->HasFocusedDescendants()))
		{
			UTNKeyBindingSelector* gamepadSelector = findSelector(selector->GetActionName(), E_TNBindingDevice::Gamepad);
			if (!IsValid(gamepadSelector) || !gamepadSelector->GetIsEnabled() || !TNWidgetFocus::Focus(gamepadSelector))
			{
				TNWidgetFocus::Focus(GetFirstFocusWidget());
			}
		}
	}
}

void UTNControlsSettingsWidget::handleInputDeviceChanged(const E_TNInputDevice device)
{
	applyDeviceColumnMode();
}

void UTNControlsSettingsWidget::refreshKeys()
{
	const UTNInputBindingSubsystem* bindings = getBindingSubsystem();
	if (!bindings)
	{
		return;
	}

	for (UTNKeyBindingSelector* selector : Selectors)
	{
		if (IsValid(selector))
		{
			// An action without a mapping for this device in the mapping context has no column to edit.
			const FKey key = bindings->GetKey(selector->GetActionName(), selector->GetDevice());
			selector->SetSelectedKey(FInputChord(key));
			selector->SetIsEnabled(key.IsValid());
		}
	}

	applyDeviceColumnMode();
}

void UTNControlsSettingsWidget::handleBindingKeySelected(UTNKeyBindingSelector* selector, const FKey& key)
{
	UTNInputBindingSubsystem* bindings = getBindingSubsystem();
	if (!bindings || !IsValid(selector))
	{
		return;
	}

	// The engine selector clears its key on the gamepad X button; unbinding is not supported,
	// so just show the current key again.
	if (!key.IsValid())
	{
		refreshKeys();
		return;
	}

	switch (bindings->Rebind(selector->GetActionName(), selector->GetDevice(), key))
	{
	case E_TNRebindResult::Success:
	case E_TNRebindResult::Unchanged:
		showMessage(FString());
		break;

	case E_TNRebindResult::Swapped:
		showMessage(TEXT("That key was in use; the two actions swapped keys."));
		break;

	case E_TNRebindResult::Reserved:
		showMessage(TEXT("That key is reserved and cannot be assigned."));
		break;

	default:
		showMessage(TEXT("That key cannot be used here."));
		break;
	}

	// Refresh every row: a swap changes another action, and a rejected key must revert the display.
	refreshKeys();
}

void UTNControlsSettingsWidget::onResetClicked()
{
	if (UTNInputBindingSubsystem* bindings = getBindingSubsystem())
	{
		bindings->ResetToDefaults();
		refreshKeys();
		showMessage(FString());
	}
}

void UTNControlsSettingsWidget::showMessage(const FString& message)
{
	if (IsValid(MessageText))
	{
		MessageText->SetText(FText::FromString(message));
	}
}

UTNInputBindingSubsystem* UTNControlsSettingsWidget::getBindingSubsystem() const
{
	const ULocalPlayer* localPlayer = GetOwningLocalPlayer();
	return IsValid(localPlayer) ? localPlayer->GetSubsystem<UTNInputBindingSubsystem>() : nullptr;
}
