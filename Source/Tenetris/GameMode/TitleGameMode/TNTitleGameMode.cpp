// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/GameMode/TitleGameMode/TNTitleGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ATNTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	if (TitleMenuWidgetClass && IsValid(playerController))
	{
		if (UUserWidget* menuWidget = CreateWidget<UUserWidget>(playerController, TitleMenuWidgetClass))
		{
			menuWidget->AddToViewport();
		}
	}

	if (IsValid(playerController))
	{
		playerController->bShowMouseCursor = true;

		FInputModeUIOnly inputMode;
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		playerController->SetInputMode(inputMode);
	}
}
