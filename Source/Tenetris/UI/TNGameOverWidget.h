// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/UI/TNMenuWidgetBase.h"
#include "TNGameOverWidget.generated.h"

class ATNDemoSingleGameMode;
class UButton;

// Game-over panel (Replay / Main Menu), placed inside the score HUD as GameOverPanel.
//
// Logic lives here (C++); the layout lives in a WBP subclass. The WBP must contain
// widgets named exactly like the BindWidget properties below.
UCLASS()
class TENETRIS_API UTNGameOverWidget : public UTNMenuWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual UWidget* GetDefaultFocusWidget() const override;

	// There is nothing to go back to; Back is swallowed so it does nothing.
	virtual bool HandleBack() override;

private:
	UFUNCTION()
	void onReplayClicked();

	UFUNCTION()
	void onMainMenuClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReplayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY()
	TObjectPtr<ATNDemoSingleGameMode> GameMode;
};
