// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Input/TNInputDeviceSubsystem.h"
#include "Tenetris/Input/TNInputDeviceProcessor.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"
#include "GameFramework/PlayerController.h"

UTNInputDeviceSubsystem* UTNInputDeviceSubsystem::Get(const UObject* worldContextObject)
{
	const UWorld* world = GEngine ? GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::ReturnNull) : nullptr;
	const UGameInstance* gameInstance = world ? world->GetGameInstance() : nullptr;
	return gameInstance ? gameInstance->GetSubsystem<UTNInputDeviceSubsystem>() : nullptr;
}

void UTNInputDeviceSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	Processor = MakeShared<FTNInputDeviceProcessor>();
	Processor->OnInputDeviceDetected.BindUObject(this, &UTNInputDeviceSubsystem::handleInputDeviceDetected);
	FSlateApplication::Get().RegisterInputPreProcessor(Processor);
}

void UTNInputDeviceSubsystem::Deinitialize()
{
	if (Processor.IsValid())
	{
		Processor->OnInputDeviceDetected.Unbind();

		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(Processor);
		}

		Processor.Reset();
	}

	// The Slate cursor is shared with the editor in PIE; never leave it hidden behind.
	if (CurrentDevice == E_TNInputDevice::Gamepad && FSlateApplication::IsInitialized())
	{
		if (const TSharedPtr<FSlateUser> slateUser = FSlateApplication::Get().GetCursorUser())
		{
			slateUser->SetCursorVisibility(true);
		}
	}

	Super::Deinitialize();
}

void UTNInputDeviceSubsystem::SetCursorWanted(const bool bWanted)
{
	bCursorWanted = bWanted;
	applyCursorVisibility();
}

void UTNInputDeviceSubsystem::handleInputDeviceDetected(const E_TNInputDevice device)
{
	if (CurrentDevice == device)
	{
		return;
	}

	CurrentDevice = device;
	applyCursorVisibility();

	OnInputDeviceChanged.Broadcast(CurrentDevice);
}

void UTNInputDeviceSubsystem::applyCursorVisibility() const
{
	const bool bUsingGamepad = CurrentDevice == E_TNInputDevice::Gamepad;

	if (APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController())
	{
		playerController->SetShowMouseCursor(bCursorWanted && !bUsingGamepad);
	}

	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	FSlateApplication& slateApp = FSlateApplication::Get();

	// Hiding the Slate cursor makes a gamepad switch take effect without waiting for a mouse move.
	// It is restored as soon as the mouse is used again, so the editor cursor in PIE comes back too.
	if (const TSharedPtr<FSlateUser> slateUser = slateApp.GetCursorUser())
	{
		slateUser->SetCursorVisibility(!bUsingGamepad);
	}

	// The viewport only re-evaluates its cursor on mouse events otherwise.
	slateApp.QueryCursor();
}
