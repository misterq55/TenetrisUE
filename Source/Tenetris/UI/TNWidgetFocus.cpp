// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNWidgetFocus.h"
#include "Components/Widget.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"

TSharedPtr<SWidget> TNWidgetFocus::FindFocusable(const TSharedRef<SWidget>& widget)
{
	if (!widget->GetVisibility().IsVisible() || !widget->IsEnabled())
	{
		return nullptr;
	}

	if (widget->SupportsKeyboardFocus())
	{
		return widget;
	}

	FChildren* children = widget->GetChildren();
	for (int32 index = 0; children && index < children->Num(); ++index)
	{
		if (TSharedPtr<SWidget> found = FindFocusable(children->GetChildAt(index)))
		{
			return found;
		}
	}

	return nullptr;
}

bool TNWidgetFocus::Focus(const UWidget* widget)
{
	if (!IsValid(widget))
	{
		return false;
	}

	const TSharedPtr<SWidget> cachedWidget = widget->GetCachedWidget();
	const TSharedPtr<SWidget> target = cachedWidget.IsValid() ? FindFocusable(cachedWidget.ToSharedRef()) : nullptr;
	if (!target.IsValid())
	{
		return false;
	}

	ULocalPlayer* localPlayer = widget->GetOwningLocalPlayer();
	const TSharedPtr<FSlateUser> slateUser = localPlayer ? localPlayer->GetSlateUser() : nullptr;
	if (!slateUser.IsValid())
	{
		return false;
	}

	// Same approach as UWidget::SetUserFocus: a widget shown this frame is not in the
	// arranged hierarchy yet, so hand the request to the player's deferred Slate operations.
	FReply& delayedSlateOperations = localPlayer->GetSlateOperations();
	if (FSlateApplication::Get().SetUserFocus(slateUser->GetUserIndex(), target))
	{
		delayedSlateOperations.CancelFocusRequest();
	}
	else
	{
		delayedSlateOperations.SetUserFocus(target.ToSharedRef());
	}

	return true;
}
