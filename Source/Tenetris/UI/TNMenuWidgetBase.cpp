// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNMenuWidgetBase.h"
#include "Tenetris/UI/TNWidgetFocus.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"
#include "Widgets/SViewport.h"

void UTNMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	bIsRootMenu = GetTypedOuter<UTNMenuWidgetBase>() == nullptr;
	LastFocusedWidget.Reset();
}

void UTNMenuWidgetBase::FocusMenu()
{
	UWidget* target = GetDefaultFocusWidget();

	if (UTNMenuWidgetBase* subMenu = Cast<UTNMenuWidgetBase>(target))
	{
		subMenu->FocusMenu();
		return;
	}

	// Retries on the next frame by itself if the target was only just made visible.
	TNWidgetFocus::Focus(target);
}

void UTNMenuWidgetBase::NativeTick(const FGeometry& myGeometry, const float deltaTime)
{
	Super::NativeTick(myGeometry, deltaTime);

	if (bIsRootMenu && IsVisible())
	{
		restoreFocus();
	}
}

void UTNMenuWidgetBase::restoreFocus()
{
	const ULocalPlayer* localPlayer = GetOwningLocalPlayer();
	const TSharedPtr<const FSlateUser> slateUser = localPlayer ? localPlayer->GetSlateUser() : nullptr;
	if (!slateUser.IsValid())
	{
		return;
	}

	if (HasFocusedDescendants())
	{
		LastFocusedWidget = slateUser->GetFocusedWidget();
		return;
	}

	// Only take focus back from the game viewport (or from nowhere), never from another
	// window such as the editor while playing in PIE.
	if (slateUser->GetFocusedWidget().IsValid())
	{
		const UGameViewportClient* viewportClient = GetWorld() ? GetWorld()->GetGameViewport() : nullptr;
		const TSharedPtr<SViewport> viewportWidget = viewportClient ? viewportClient->GetGameViewportWidget() : nullptr;
		if (!viewportWidget.IsValid() || !slateUser->IsWidgetInFocusPath(viewportWidget))
		{
			return;
		}
	}

	// Fails when the widget is no longer visible (e.g. its panel was closed).
	if (const TSharedPtr<SWidget> lastFocused = LastFocusedWidget.Pin())
	{
		if (FSlateApplication::Get().SetUserFocus(slateUser->GetUserIndex(), lastFocused))
		{
			return;
		}
	}

	FocusMenu();
}

FReply UTNMenuWidgetBase::NativeOnKeyDown(const FGeometry& inGeometry, const FKeyEvent& inKeyEvent)
{
	if (!inKeyEvent.IsRepeat())
	{
		if (FSlateApplication::Get().GetNavigationActionFromKey(inKeyEvent) == EUINavigationAction::Back && HandleBack())
		{
			return FReply::Handled();
		}

		if (HandleMenuKey(inKeyEvent.GetKey()))
		{
			return FReply::Handled();
		}
	}

	return Super::NativeOnKeyDown(inGeometry, inKeyEvent);
}
