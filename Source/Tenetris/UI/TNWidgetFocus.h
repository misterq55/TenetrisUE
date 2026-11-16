// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SWidget;
class UWidget;

// Focus helpers that work at the Slate level, because some UMG widgets (e.g. key selectors)
// wrap their focusable Slate widget in a non-focusable container, so UWidget::SetFocus
// would not reach it.
namespace TNWidgetFocus
{
	// First visible, enabled widget that supports keyboard focus at or below the given widget.
	TSharedPtr<SWidget> FindFocusable(const TSharedRef<SWidget>& widget);

	// Gives keyboard/gamepad focus to the first focusable widget inside the given widget.
	// If it cannot be focused yet (e.g. made visible this frame), the request is retried
	// on the next frame. Returns false if there is nothing focusable inside.
	bool Focus(const UWidget* widget);
}
