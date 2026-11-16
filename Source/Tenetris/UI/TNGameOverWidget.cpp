// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Tenetris/GameMode/DemoSingleGameMode/TNDemoSingleGameMode.h"

void UTNGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<ATNDemoSingleGameMode>(UGameplayStatics::GetGameMode(this));

	if (IsValid(ReplayButton))
	{
		ReplayButton->OnClicked.AddDynamic(this, &UTNGameOverWidget::onReplayClicked);
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UTNGameOverWidget::onMainMenuClicked);
	}
}

void UTNGameOverWidget::NativeDestruct()
{
	if (IsValid(ReplayButton))
	{
		ReplayButton->OnClicked.RemoveDynamic(this, &UTNGameOverWidget::onReplayClicked);
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->OnClicked.RemoveDynamic(this, &UTNGameOverWidget::onMainMenuClicked);
	}

	Super::NativeDestruct();
}

UWidget* UTNGameOverWidget::GetDefaultFocusWidget() const
{
	return ReplayButton;
}

bool UTNGameOverWidget::HandleBack()
{
	return true;
}

void UTNGameOverWidget::onReplayClicked()
{
	if (IsValid(GameMode))
	{
		GameMode->RestartMatch();
	}
}

void UTNGameOverWidget::onMainMenuClicked()
{
	if (IsValid(GameMode))
	{
		GameMode->ReturnToTitle();
	}
}
