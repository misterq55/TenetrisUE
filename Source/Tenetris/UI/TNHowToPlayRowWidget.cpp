// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNHowToPlayRowWidget.h"
#include "Components/TextBlock.h"
#include "Engine/LocalPlayer.h"
#include "Tenetris/Input/TNInputBindingSubsystem.h"

namespace
{
	// Shown when the action has no key for a device.
	const FText NoKeyText = INVTEXT("-");
}

void UTNHowToPlayRowWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Designer preview: there is no local player, so only the label can be shown.
	if (IsDesignTime() && IsValid(ActionText))
	{
		ActionText->SetText(DisplayName.IsEmpty() ? FText::FromName(MappingName) : DisplayName);
	}
}

void UTNHowToPlayRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Refresh();
}

void UTNHowToPlayRowWidget::Refresh()
{
	const ULocalPlayer* localPlayer = GetOwningLocalPlayer();
	const UTNInputBindingSubsystem* bindings = IsValid(localPlayer) ? localPlayer->GetSubsystem<UTNInputBindingSubsystem>() : nullptr;

	if (IsValid(ActionText))
	{
		FText label = DisplayName;

		if (label.IsEmpty() && bindings)
		{
			for (const FTNBindableAction& action : bindings->GetBindableActions())
			{
				if (action.MappingName == MappingName)
				{
					label = action.DisplayName;
					break;
				}
			}
		}

		ActionText->SetText(label.IsEmpty() ? FText::FromName(MappingName) : label);
	}

	// Returns whether the action has a key for the device.
	const auto setKeyText = [this, bindings](UTextBlock* textBlock, const E_TNBindingDevice device)
	{
		const FKey key = bindings ? bindings->GetKey(MappingName, device) : EKeys::Invalid;

		if (IsValid(textBlock))
		{
			textBlock->SetText(key.IsValid() ? UTNInputBindingSubsystem::GetKeyShortName(key) : NoKeyText);
		}

		return key.IsValid();
	};

	const bool bHasKeyboardKey = setKeyText(KeyboardKeyText, E_TNBindingDevice::Keyboard);
	const bool bHasGamepadKey = setKeyText(GamepadKeyText, E_TNBindingDevice::Gamepad);

	if (bindings && !bHasKeyboardKey && !bHasGamepadKey)
	{
		UE_LOG(LogTemp, Warning, TEXT("TNHowToPlayRowWidget %s: no key found for mapping '%s'; check MappingName."),
			*GetName(), *MappingName.ToString());
	}
}
