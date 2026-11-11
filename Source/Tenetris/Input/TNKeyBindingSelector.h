// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"
#include "Tenetris/Input/TNInputBindingSubsystem.h"
#include "TNKeyBindingSelector.generated.h"

class UTNKeyBindingSelector;
class STextBlock;

DECLARE_MULTICAST_DELEGATE_TwoParams(FTNOnBindingKeySelected, UTNKeyBindingSelector*, const FKey&);

// Key selector that remembers which action/device column it edits, so one handler
// can serve every row of the controls settings. It shows a compact key name on top of
// the engine's text, which uses long names such as "Gamepad Face Button Bottom".
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
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION()
	void handleKeySelected(FInputChord chord);

	UFUNCTION()
	void handleIsSelectingKeyChanged();

private:
	FText getShortKeyText() const;
	EVisibility getShortKeyVisibility() const;

	// The engine text is only shown while waiting for a key, so its prompt stays visible.
	void updateEngineTextVisibility();

	FName ActionName;
	E_TNBindingDevice Device = E_TNBindingDevice::Keyboard;

	TSharedPtr<STextBlock> ShortKeyText;
};
