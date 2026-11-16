// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNFocusRow.h"
#include "Tenetris/Input/TNInputDeviceSubsystem.h"
#include "Tenetris/UI/TNWidgetFocus.h"

const FName UTNFocusRow::ContentSlotName(TEXT("Content"));

void UTNFocusRow::NativeConstruct()
{
	Super::NativeConstruct();

	setIndicatorVisible(HasFocusedDescendants());
}

void UTNFocusRow::NativeOnAddedToFocusPath(const FFocusEvent& inFocusEvent)
{
	Super::NativeOnAddedToFocusPath(inFocusEvent);

	setIndicatorVisible(true);
}

void UTNFocusRow::NativeOnRemovedFromFocusPath(const FFocusEvent& inFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(inFocusEvent);

	setIndicatorVisible(false);
}

void UTNFocusRow::NativeOnMouseEnter(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent)
{
	Super::NativeOnMouseEnter(inGeometry, inMouseEvent);

	// Layout changes under a resting (hidden) cursor also raise mouse-enter; they must not
	// steal focus from the gamepad.
	const UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this);
	if (inputDevice && inputDevice->IsUsingGamepad())
	{
		return;
	}

	if (HasFocusedDescendants())
	{
		return;
	}

	TNWidgetFocus::Focus(GetContentForSlot(ContentSlotName));
}

void UTNFocusRow::setIndicatorVisible(const bool bVisible)
{
	if (IsValid(FocusIndicator))
	{
		// Hidden rather than collapsed, so the row does not shift when focus moves.
		FocusIndicator->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}
