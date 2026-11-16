// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNMenuWidgetBase.generated.h"

// Base for menus that can be driven by keyboard and gamepad as well as the mouse.
//
// - FocusMenu() moves focus to GetDefaultFocusWidget(); if that is another menu, it recurses.
// - Back (gamepad B / Esc) goes to HandleBack(), other keys to HandleMenuKey(). Keys bubble
//   from the focused item up through its parent menus, so a nested menu handles them first.
//   Keys neither handler consumes fall through (e.g. Start still reaches the player controller).
// - A root menu (one not nested in another menu) takes focus back whenever the game viewport
//   holds it, e.g. on the first frame or after a mouse click on an empty area.
UCLASS(Abstract)
class TENETRIS_API UTNMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void FocusMenu();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& myGeometry, float deltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent) override;

	// Item focused when the menu is entered; may depend on which panel is currently shown.
	virtual UWidget* GetDefaultFocusWidget() const { return nullptr; }

	// Back was pressed while focus was inside this menu. Return true to consume it.
	virtual bool HandleBack() { return false; }

	// Any other key pressed while focus was inside this menu. Return true to consume it.
	virtual bool HandleMenuKey(const FKey& key) { return false; }

private:
	void restoreFocus();

	bool bIsRootMenu = false;

	// Restored first when focus is lost, so a stray click does not reset the selection.
	TWeakPtr<SWidget> LastFocusedWidget;
};
