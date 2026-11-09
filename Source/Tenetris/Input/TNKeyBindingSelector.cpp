// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Input/TNKeyBindingSelector.h"

void UTNKeyBindingSelector::Setup(const FName actionName, const E_TNBindingDevice device)
{
	ActionName = actionName;
	Device = device;

	SetAllowModifierKeys(false);
	SetAllowGamepadKeys(device == E_TNBindingDevice::Gamepad);

	OnKeySelected.RemoveDynamic(this, &UTNKeyBindingSelector::handleKeySelected);
	OnKeySelected.AddDynamic(this, &UTNKeyBindingSelector::handleKeySelected);
}

void UTNKeyBindingSelector::handleKeySelected(FInputChord chord)
{
	OnBindingKeySelected.Broadcast(this, chord.Key);
}
