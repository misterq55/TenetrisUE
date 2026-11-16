// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNFocusRow.generated.h"

// Wrapper that marks the focused menu item.
//
// The WBP subclass holds a Named Slot called "Content" for the item (button, slider, key
// selector) and an optional FocusIndicator widget (e.g. a ">" text) that is shown while the
// item, or anything inside it, has keyboard/gamepad focus. Hovering with the mouse moves focus
// to the item as well, so the indicator follows whichever device is in use.
UCLASS()
class TENETRIS_API UTNFocusRow : public UUserWidget
{
	GENERATED_BODY()

public:
	static const FName ContentSlotName;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& inFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& inFocusEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;

private:
	void setIndicatorVisible(const bool bVisible);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> FocusIndicator;
};
