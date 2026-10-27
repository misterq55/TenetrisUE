// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNScoreHUDWidget.generated.h"

class ATNDemoSingleGameMode;
class UTextBlock;
class UButton;
class UWidget;

// Demo score HUD.
//
// Data/binding logic lives here (C++); the visual layout lives in a WBP subclass.
// The WBP must contain widgets named exactly like the BindWidget properties below.
UCLASS()
class TENETRIS_API UTNScoreHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void bindGameMode();
	void refreshFromGameMode();

	UFUNCTION()
	void handleScoreChanged(int32 Score, int32 Level, int32 TotalLines);

	UFUNCTION()
	void handleGameOver();

	UFUNCTION()
	void onReplayClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LinesText;

	// Optional panel shown on game over (BindWidgetOptional so the WBP can omit it).
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> GameOverPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ReplayButton;

private:
	UPROPERTY()
	TObjectPtr<ATNDemoSingleGameMode> GameMode;
};
