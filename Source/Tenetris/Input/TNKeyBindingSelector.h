// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"
#include "Tenetris/Input/TNInputBindingSubsystem.h"
#include "TNKeyBindingSelector.generated.h"

class UTNKeyBindingSelector;

DECLARE_MULTICAST_DELEGATE_TwoParams(FTNOnBindingKeySelected, UTNKeyBindingSelector*, const FKey&);

// Key selector that remembers which action/device column it edits, so one handler
// can serve every row of the controls settings.
UCLASS()
class TENETRIS_API UTNKeyBindingSelector : public UInputKeySelector
{
	GENERATED_BODY()

public:
	void Setup(const FName actionName, const E_TNBindingDevice device);

	FName GetActionName() const { return ActionName; }
	E_TNBindingDevice GetDevice() const { return Device; }

	FTNOnBindingKeySelected OnBindingKeySelected;

protected:
	UFUNCTION()
	void handleKeySelected(FInputChord chord);

private:
	FName ActionName;
	E_TNBindingDevice Device = E_TNBindingDevice::Keyboard;
};
