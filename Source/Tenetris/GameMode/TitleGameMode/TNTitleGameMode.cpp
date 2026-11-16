// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/GameMode/TitleGameMode/TNTitleGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Tenetris/Input/TNInputDeviceSubsystem.h"

void ATNTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	if (IsValid(playerController))
	{
		FInputModeUIOnly inputMode;
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		playerController->SetInputMode(inputMode);
	}

	if (UTNInputDeviceSubsystem* inputDevice = UTNInputDeviceSubsystem::Get(this))
	{
		inputDevice->SetCursorWanted(true);
	}

	// Added after the input mode is applied: applying UI-only input focuses the viewport,
	// while the menu focuses its default button when it is constructed.
	if (TitleMenuWidgetClass && IsValid(playerController))
	{
		if (UUserWidget* menuWidget = CreateWidget<UUserWidget>(playerController, TitleMenuWidgetClass))
		{
			menuWidget->AddToViewport();
		}
	}
}
