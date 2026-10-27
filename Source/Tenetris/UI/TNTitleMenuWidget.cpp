// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNTitleMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTNTitleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(PlayButton))
	{
		PlayButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onPlayClicked);
	}

	if (IsValid(HowToPlayButton))
	{
		HowToPlayButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onHowToPlayClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onQuitClicked);
	}

	if (IsValid(CloseHowToPlayButton))
	{
		CloseHowToPlayButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onCloseHowToPlayClicked);
	}

	if (IsValid(HowToPlayPanel))
	{
		HowToPlayPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTNTitleMenuWidget::NativeDestruct()
{
	if (IsValid(PlayButton))
	{
		PlayButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onPlayClicked);
	}

	if (IsValid(HowToPlayButton))
	{
		HowToPlayButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onHowToPlayClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onQuitClicked);
	}

	if (IsValid(CloseHowToPlayButton))
	{
		CloseHowToPlayButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onCloseHowToPlayClicked);
	}

	Super::NativeDestruct();
}

void UTNTitleMenuWidget::onPlayClicked()
{
	if (PlayLevelName.IsNone())
	{
		return;
	}

	UGameplayStatics::OpenLevel(this, PlayLevelName);
}

void UTNTitleMenuWidget::onHowToPlayClicked()
{
	if (IsValid(HowToPlayPanel))
	{
		HowToPlayPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTNTitleMenuWidget::onCloseHowToPlayClicked()
{
	if (IsValid(HowToPlayPanel))
	{
		HowToPlayPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTNTitleMenuWidget::onQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}
