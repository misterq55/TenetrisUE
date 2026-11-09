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
#include "Tenetris/Input/TNKeyBindingSelector.h"

void UTNControlsSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ResetButton))
	{
		ResetButton->OnClicked.AddDynamic(this, &UTNControlsSettingsWidget::onResetClicked);
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

	Super::NativeDestruct();
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
			UHorizontalBoxSlot* selectorSlot = row->AddChildToHorizontalBox(createSelector(action.MappingName, device));
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
}

void UTNControlsSettingsWidget::handleBindingKeySelected(UTNKeyBindingSelector* selector, const FKey& key)
{
	UTNInputBindingSubsystem* bindings = getBindingSubsystem();
	if (!bindings || !IsValid(selector))
	{
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
