// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNScoreHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Tenetris/GameMode/DemoSingleGameMode/TNDemoSingleGameMode.h"

void UTNScoreHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bindGameMode();

	if (IsValid(ReplayButton))
	{
		ReplayButton->OnClicked.AddDynamic(this, &UTNScoreHUDWidget::onReplayClicked);
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UTNScoreHUDWidget::onMainMenuClicked);
	}

	if (IsValid(GameOverPanel))
	{
		GameOverPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	refreshFromGameMode();
}

void UTNScoreHUDWidget::NativeDestruct()
{
	if (IsValid(GameMode))
	{
		GameMode->OnScoreChanged.RemoveDynamic(this, &UTNScoreHUDWidget::handleScoreChanged);
		GameMode->OnGameOverUI.RemoveDynamic(this, &UTNScoreHUDWidget::handleGameOver);
	}

	if (IsValid(ReplayButton))
	{
		ReplayButton->OnClicked.RemoveDynamic(this, &UTNScoreHUDWidget::onReplayClicked);
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->OnClicked.RemoveDynamic(this, &UTNScoreHUDWidget::onMainMenuClicked);
	}

	Super::NativeDestruct();
}

void UTNScoreHUDWidget::bindGameMode()
{
	GameMode = Cast<ATNDemoSingleGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(GameMode))
	{
		return;
	}

	GameMode->OnScoreChanged.AddDynamic(this, &UTNScoreHUDWidget::handleScoreChanged);
	GameMode->OnGameOverUI.AddDynamic(this, &UTNScoreHUDWidget::handleGameOver);
}

void UTNScoreHUDWidget::refreshFromGameMode()
{
	if (!IsValid(GameMode))
	{
		return;
	}

	handleScoreChanged(GameMode->GetScore(), GameMode->GetMatchLevel(), GameMode->GetTotalLines());
}

void UTNScoreHUDWidget::handleScoreChanged(const int32 Score, const int32 Level, const int32 TotalLines)
{
	if (IsValid(ScoreText))
	{
		ScoreText->SetText(FText::Format(NSLOCTEXT("TNScoreHUD", "ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
	}

	if (IsValid(LevelText))
	{
		LevelText->SetText(FText::Format(NSLOCTEXT("TNScoreHUD", "LevelFmt", "Level: {0}"), FText::AsNumber(Level)));
	}

	if (IsValid(LinesText))
	{
		LinesText->SetText(FText::Format(NSLOCTEXT("TNScoreHUD", "LinesFmt", "Lines: {0}"), FText::AsNumber(TotalLines)));
	}
}

void UTNScoreHUDWidget::handleGameOver()
{
	if (IsValid(GameOverPanel))
	{
		GameOverPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTNScoreHUDWidget::onReplayClicked()
{
	if (IsValid(GameMode))
	{
		GameMode->RestartMatch();
	}
}

void UTNScoreHUDWidget::onMainMenuClicked()
{
	if (IsValid(GameMode))
	{
		GameMode->ReturnToTitle();
	}
}
