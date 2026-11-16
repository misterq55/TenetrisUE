// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TNInputDeviceSubsystem.generated.h"

class FTNInputDeviceProcessor;

UENUM(BlueprintType)
enum class E_TNInputDevice : uint8
{
	KeyboardMouse,
	Gamepad,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FTNOnInputDeviceChanged, E_TNInputDevice);

// Tracks whether the player is currently using keyboard/mouse or a gamepad, and owns the
// mouse cursor visibility: the cursor is shown only when the current screen wants it
// (SetCursorWanted) and the last input did not come from a gamepad.
UCLASS()
class TENETRIS_API UTNInputDeviceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UTNInputDeviceSubsystem* Get(const UObject* worldContextObject);

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "Input")
	E_TNInputDevice GetCurrentDevice() const { return CurrentDevice; }

	UFUNCTION(BlueprintPure, Category = "Input")
	bool IsUsingGamepad() const { return CurrentDevice == E_TNInputDevice::Gamepad; }

	// Each screen declares on entry whether it wants a mouse cursor (menus yes, gameplay no).
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetCursorWanted(const bool bWanted);

	// Fired only when the device actually changes.
	FTNOnInputDeviceChanged OnInputDeviceChanged;

private:
	void handleInputDeviceDetected(const E_TNInputDevice device);
	void applyCursorVisibility() const;

	TSharedPtr<FTNInputDeviceProcessor> Processor;

	E_TNInputDevice CurrentDevice = E_TNInputDevice::KeyboardMouse;

	bool bCursorWanted = true;
};
