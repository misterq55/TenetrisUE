// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Input/TNInputBindingSubsystem.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"
#include "UserSettings/EnhancedInputUserSettings.h"

namespace
{
	// Menu/system keys that must stay usable, so they can never be assigned to an action.
	const FKey ReservedKeys[] =
	{
		EKeys::Escape,
		EKeys::Enter,
		EKeys::Tilde,
		EKeys::F1, EKeys::F2, EKeys::F3, EKeys::F4, EKeys::F5, EKeys::F6,
		EKeys::F7, EKeys::F8, EKeys::F9, EKeys::F10, EKeys::F11, EKeys::F12,
		EKeys::Gamepad_Special_Left,
		EKeys::Gamepad_Special_Right,
	};
}

TArray<FTNBindableAction> UTNInputBindingSubsystem::GetBindableActions() const
{
	TArray<FTNBindableAction> actions;

	const UEnhancedInputUserSettings* settings = getUserSettings();
	if (!settings)
	{
		return actions;
	}

	TSet<FName> added;

	for (const UInputMappingContext* context : settings->GetRegisteredInputMappingContexts())
	{
		if (!IsValid(context))
		{
			continue;
		}

		for (const FEnhancedActionKeyMapping& mapping : context->GetMappings())
		{
			if (!mapping.IsPlayerMappable())
			{
				continue;
			}

			const FName mappingName = mapping.GetMappingName();
			if (mappingName.IsNone() || added.Contains(mappingName))
			{
				continue;
			}

			added.Add(mappingName);

			FTNBindableAction& action = actions.AddDefaulted_GetRef();
			action.MappingName = mappingName;
			action.DisplayName = mapping.GetDisplayName().IsEmpty() ? FText::FromName(mappingName) : mapping.GetDisplayName();
		}
	}

	return actions;
}

FKey UTNInputBindingSubsystem::GetKey(const FName mappingName, const E_TNBindingDevice device) const
{
	const FPlayerKeyMapping* column = findColumnMapping(mappingName, device);
	return column ? column->GetCurrentKey() : EKeys::Invalid;
}

E_TNRebindResult UTNInputBindingSubsystem::Rebind(const FName mappingName, const E_TNBindingDevice device, const FKey& newKey)
{
	if (!newKey.IsValid() || newKey.IsMouseButton() || newKey.IsAxis1D() || newKey.IsAxis2D() || newKey.IsAxis3D()
		|| !isKeyForDevice(newKey, device))
	{
		return E_TNRebindResult::Invalid;
	}

	const FPlayerKeyMapping* column = findColumnMapping(mappingName, device);
	if (!column)
	{
		return E_TNRebindResult::Invalid;
	}

	// Copy what we need: mapping a key modifies the profile the pointer refers to.
	const FPlayerKeyMapping target = *column;
	const FKey oldKey = target.GetCurrentKey();

	if (oldKey == newKey)
	{
		return E_TNRebindResult::Unchanged;
	}

	if (isReservedKey(newKey))
	{
		return E_TNRebindResult::Reserved;
	}

	// If another action already uses the key, give it this action's old key so nothing is left unbound.
	bool bSwapped = false;
	for (const FTNBindableAction& other : GetBindableActions())
	{
		if (other.MappingName == mappingName)
		{
			continue;
		}

		const FPlayerKeyMapping* otherColumn = findColumnMapping(other.MappingName, device);
		if (otherColumn && otherColumn->GetCurrentKey() == newKey)
		{
			const FPlayerKeyMapping otherTarget = *otherColumn;
			if (!mapKey(otherTarget, oldKey))
			{
				return E_TNRebindResult::Invalid;
			}

			bSwapped = true;
			break;
		}
	}

	if (!mapKey(target, newKey))
	{
		return E_TNRebindResult::Invalid;
	}

	return bSwapped ? E_TNRebindResult::Swapped : E_TNRebindResult::Success;
}

void UTNInputBindingSubsystem::ResetToDefaults()
{
	if (UEnhancedInputUserSettings* settings = getUserSettings())
	{
		FGameplayTagContainer failureReason;
		settings->ResetKeyProfileIdToDefault(settings->GetActiveKeyProfileId(), failureReason);

		if (!failureReason.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("TNInputBindingSubsystem: resetting the key profile failed."));
		}
	}
}

void UTNInputBindingSubsystem::Save() const
{
	if (UEnhancedInputUserSettings* settings = getUserSettings())
	{
		settings->SaveSettings();
	}
}

UEnhancedInputUserSettings* UTNInputBindingSubsystem::getUserSettings() const
{
	const ULocalPlayer* localPlayer = GetLocalPlayer();
	if (!IsValid(localPlayer))
	{
		return nullptr;
	}

	const UEnhancedInputLocalPlayerSubsystem* inputSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!inputSubsystem)
	{
		return nullptr;
	}

	UEnhancedInputUserSettings* settings = inputSubsystem->GetUserSettings();
	if (!settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("TNInputBindingSubsystem: Enhanced Input user settings are disabled (Project Settings > Enhanced Input > Enable User Settings)."));
	}

	return settings;
}

const FPlayerKeyMapping* UTNInputBindingSubsystem::findColumnMapping(const FName mappingName, const E_TNBindingDevice device) const
{
	const UEnhancedInputUserSettings* settings = getUserSettings();
	const UEnhancedPlayerMappableKeyProfile* profile = settings ? settings->GetActiveKeyProfile() : nullptr;
	if (!profile)
	{
		return nullptr;
	}

	const FKeyMappingRow* row = profile->FindKeyMappingRow(mappingName);
	if (!row)
	{
		return nullptr;
	}

	// The column is decided by the default key from the mapping context, so it stays stable
	// whatever the player maps. If there are several, the lowest slot wins.
	const FPlayerKeyMapping* found = nullptr;
	for (const FPlayerKeyMapping& mapping : row->Mappings)
	{
		if (isKeyForDevice(mapping.GetDefaultKey(), device)
			&& (!found || mapping.GetSlot() < found->GetSlot()))
		{
			found = &mapping;
		}
	}

	return found;
}

bool UTNInputBindingSubsystem::isReservedKey(const FKey& key) const
{
	for (const FKey& reserved : ReservedKeys)
	{
		if (key == reserved)
		{
			return true;
		}
	}

	// Keys used by actions that cannot be rebound (e.g. Pause) are reserved as well.
	if (const UEnhancedInputUserSettings* settings = getUserSettings())
	{
		for (const UInputMappingContext* context : settings->GetRegisteredInputMappingContexts())
		{
			if (!IsValid(context))
			{
				continue;
			}

			for (const FEnhancedActionKeyMapping& mapping : context->GetMappings())
			{
				if (!mapping.IsPlayerMappable() && mapping.Key == key)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool UTNInputBindingSubsystem::mapKey(const FPlayerKeyMapping& column, const FKey& key) const
{
	UEnhancedInputUserSettings* settings = getUserSettings();
	if (!settings)
	{
		return false;
	}

	FMapPlayerKeyArgs args;
	args.MappingName = column.GetMappingName();
	args.Slot = column.GetSlot();
	args.HardwareDeviceId = column.GetHardwareDeviceId().HardwareDeviceIdentifier;
	args.NewKey = key;

	FGameplayTagContainer failureReason;
	settings->MapPlayerKey(args, failureReason);

	if (!failureReason.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("TNInputBindingSubsystem: could not map %s to %s."),
			*key.ToString(), *args.MappingName.ToString());
		return false;
	}

	return true;
}

bool UTNInputBindingSubsystem::isKeyForDevice(const FKey& key, const E_TNBindingDevice device)
{
	if (!key.IsValid() || key.IsTouch() || key.IsGesture())
	{
		return false;
	}

	return key.IsGamepadKey() == (device == E_TNBindingDevice::Gamepad);
}
