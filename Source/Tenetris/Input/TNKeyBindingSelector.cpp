// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Input/TNKeyBindingSelector.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

void UTNKeyBindingSelector::Setup(const FName actionName, const E_TNBindingDevice device)
{
	ActionName = actionName;
	Device = device;

	SetAllowModifierKeys(false);
	SetAllowGamepadKeys(device == E_TNBindingDevice::Gamepad);

	OnKeySelected.RemoveDynamic(this, &UTNKeyBindingSelector::handleKeySelected);
	OnKeySelected.AddDynamic(this, &UTNKeyBindingSelector::handleKeySelected);

	OnIsSelectingKeyChanged.RemoveDynamic(this, &UTNKeyBindingSelector::handleIsSelectingKeyChanged);
	OnIsSelectingKeyChanged.AddDynamic(this, &UTNKeyBindingSelector::handleIsSelectingKeyChanged);
}

TSharedRef<SWidget> UTNKeyBindingSelector::RebuildWidget()
{
	const TSharedRef<SWidget> selectorWidget = Super::RebuildWidget();

	updateEngineTextVisibility();

	return SNew(SOverlay)
		+ SOverlay::Slot()
		[
			selectorWidget
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(ShortKeyText, STextBlock)
			.TextStyle(&GetTextStyle())
			// Outside the button, so it would not inherit the button's foreground color otherwise.
			.ColorAndOpacity(GetButtonStyle().NormalForeground)
			.Text_UObject(this, &UTNKeyBindingSelector::getShortKeyText)
			.Visibility_UObject(this, &UTNKeyBindingSelector::getShortKeyVisibility)
		];
}

void UTNKeyBindingSelector::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	ShortKeyText.Reset();
}

void UTNKeyBindingSelector::handleKeySelected(FInputChord chord)
{
	OnBindingKeySelected.Broadcast(this, chord.Key);
}

void UTNKeyBindingSelector::handleIsSelectingKeyChanged()
{
	updateEngineTextVisibility();
}

FText UTNKeyBindingSelector::getShortKeyText() const
{
	const FKey key = GetSelectedKey().Key;
	return key.IsValid() ? UTNInputBindingSubsystem::GetKeyShortName(key) : GetNoKeySpecifiedText();
}

EVisibility UTNKeyBindingSelector::getShortKeyVisibility() const
{
	// Never blocks clicks, so the selector underneath still receives them.
	return GetIsSelectingKey() ? EVisibility::Collapsed : EVisibility::HitTestInvisible;
}

void UTNKeyBindingSelector::updateEngineTextVisibility()
{
	// Hidden rather than collapsed, so the button keeps its height.
	SetTextBlockVisibility(GetIsSelectingKey() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}
