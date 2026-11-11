// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "TNInputBindingSubsystem.generated.h"

class UEnhancedInputUserSettings;
struct FPlayerKeyMapping;

// Which column of the controls settings a key belongs to.
UENUM(BlueprintType)
enum class E_TNBindingDevice : uint8
{
	Keyboard,
	Gamepad,
};

UENUM(BlueprintType)
enum class E_TNRebindResult : uint8
{
	Success,
	// The key was already used by another action; the two actions exchanged keys.
	Swapped,
	// The action already had this key.
	Unchanged,
	// Reserved key (menu/system keys or keys of non-rebindable actions).
	Reserved,
	// Wrong device for the column, mouse/axis input, or no such mapping.
	Invalid,
};

// One rebindable action, as shown in the controls settings.
USTRUCT(BlueprintType)
struct FTNBindableAction
{
	GENERATED_BODY()

	// Player mappable key name (Player Mappable Key Settings > Name on the input action).
	UPROPERTY(BlueprintReadOnly)
	FName MappingName;

	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;
};

// Game policy layer on top of the Enhanced Input user settings.
//
// The engine owns the key data: rebindable mappings come from the input mapping contexts
// registered with the user settings (Project Settings > Enhanced Input > Default Mapping
// Contexts, "Register With User Settings"), the player's keys are saved to a SaveGame slot,
// and changes are applied to the player's input immediately by the engine.
//
// This subsystem only adds the Tenetris rules: one keyboard and one gamepad key per action,
// reserved keys, swapping on conflict, and no unbound actions.
UCLASS()
class TENETRIS_API UTNInputBindingSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	// Rebindable actions in input mapping context order.
	TArray<FTNBindableAction> GetBindableActions() const;

	// Current key of the column, or an invalid key if the action has no mapping for that device.
	FKey GetKey(const FName mappingName, const E_TNBindingDevice device) const;

	E_TNRebindResult Rebind(const FName mappingName, const E_TNBindingDevice device, const FKey& newKey);

	void ResetToDefaults();

	// Writes the player's keys to the user settings save slot.
	void Save() const;

	// Compact label for the controls settings ("A", "LB", ...) instead of the engine's long gamepad names.
	static FText GetKeyShortName(const FKey& key);

private:
	UEnhancedInputUserSettings* getUserSettings() const;

	// The player mapping that represents the given column; null if it does not exist.
	const FPlayerKeyMapping* findColumnMapping(const FName mappingName, const E_TNBindingDevice device) const;

	bool isReservedKey(const FKey& key) const;
	bool mapKey(const FPlayerKeyMapping& column, const FKey& key) const;

	static bool isKeyForDevice(const FKey& key, const E_TNBindingDevice device);
};
