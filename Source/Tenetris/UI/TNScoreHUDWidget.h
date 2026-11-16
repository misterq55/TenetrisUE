// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNScoreHUDWidget.generated.h"

class ATNDemoSingleGameMode;
class UTextBlock;
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

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LinesText;

	// Panel shown on game over (hidden by default). A UTNGameOverWidget instance gets
	// keyboard/gamepad focus when shown.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> GameOverPanel;

private:
	UPROPERTY()
	TObjectPtr<ATNDemoSingleGameMode> GameMode;
};
