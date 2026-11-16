// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/UI/TNMenuWidgetBase.h"
#include "TNPauseMenuWidget.generated.h"

class ATNDemoSingleGameMode;
class UButton;
class UWidget;
class UTNOptionsWidget;

// In-game pause menu (Resume / Options / Quit to Title).
//
// Created and shown by ATNDemoSingleGameMode while the match is paused. Layout lives in a
// WBP subclass whose widgets must be named exactly like the properties below.
UCLASS()
class TENETRIS_API UTNPauseMenuWidget : public UTNMenuWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual UWidget* GetDefaultFocusWidget() const override;

	// Back on the main panel resumes the match (the options panel handles Back itself).
	virtual bool HandleBack() override;

private:
	UFUNCTION()
	void onResumeClicked();

	UFUNCTION()
	void onOptionsClicked();

	UFUNCTION()
	void onOptionsCloseRequested();

	UFUNCTION()
	void onQuitToTitleClicked();

	void showMainPanel();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitToTitleButton;

	// Container for the buttons above; hidden while the options panel is shown.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> MainMenuPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTNOptionsWidget> OptionsPanel;

private:
	UPROPERTY()
	TObjectPtr<ATNDemoSingleGameMode> GameMode;
};
