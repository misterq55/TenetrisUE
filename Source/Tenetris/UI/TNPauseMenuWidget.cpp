// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNPauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Tenetris/UI/TNOptionsWidget.h"
#include "Tenetris/GameMode/DemoSingleGameMode/TNDemoSingleGameMode.h"

void UTNPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<ATNDemoSingleGameMode>(UGameplayStatics::GetGameMode(this));

	if (IsValid(ResumeButton))
	{
		ResumeButton->OnClicked.AddDynamic(this, &UTNPauseMenuWidget::onResumeClicked);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.AddDynamic(this, &UTNPauseMenuWidget::onOptionsClicked);
	}

	if (IsValid(QuitToTitleButton))
	{
		QuitToTitleButton->OnClicked.AddDynamic(this, &UTNPauseMenuWidget::onQuitToTitleClicked);
	}

	if (IsValid(OptionsPanel))
	{
		OptionsPanel->OnCloseRequested.AddDynamic(this, &UTNPauseMenuWidget::onOptionsCloseRequested);
	}

	showMainPanel();
}

void UTNPauseMenuWidget::NativeDestruct()
{
	if (IsValid(ResumeButton))
	{
		ResumeButton->OnClicked.RemoveDynamic(this, &UTNPauseMenuWidget::onResumeClicked);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.RemoveDynamic(this, &UTNPauseMenuWidget::onOptionsClicked);
	}

	if (IsValid(QuitToTitleButton))
	{
		QuitToTitleButton->OnClicked.RemoveDynamic(this, &UTNPauseMenuWidget::onQuitToTitleClicked);
	}

	if (IsValid(OptionsPanel))
	{
		OptionsPanel->OnCloseRequested.RemoveDynamic(this, &UTNPauseMenuWidget::onOptionsCloseRequested);
	}

	Super::NativeDestruct();
}

UWidget* UTNPauseMenuWidget::GetDefaultFocusWidget() const
{
	if (IsValid(OptionsPanel) && OptionsPanel->IsVisible())
	{
		return OptionsPanel;
	}

	return ResumeButton;
}

bool UTNPauseMenuWidget::HandleBack()
{
	onResumeClicked();
	return true;
}

void UTNPauseMenuWidget::showMainPanel()
{
	if (IsValid(OptionsPanel))
	{
		OptionsPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(MainMenuPanel))
	{
		MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTNPauseMenuWidget::onResumeClicked()
{
	if (IsValid(GameMode))
	{
		GameMode->SetGamePaused(false);
	}
}

void UTNPauseMenuWidget::onOptionsClicked()
{
	if (IsValid(MainMenuPanel))
	{
		MainMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(OptionsPanel))
	{
		OptionsPanel->RefreshFromSettings();
		OptionsPanel->SetVisibility(ESlateVisibility::Visible);
		OptionsPanel->FocusMenu();
	}
}

void UTNPauseMenuWidget::onOptionsCloseRequested()
{
	showMainPanel();

	// Return to the button that opened the options.
	if (IsValid(OptionsButton))
	{
		OptionsButton->SetFocus();
	}
}

void UTNPauseMenuWidget::onQuitToTitleClicked()
{
	if (IsValid(GameMode))
	{
		GameMode->ReturnToTitle();
	}
}
