// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNTitleMenuWidget.generated.h"

class UButton;
class UWidget;

// Title/main-menu widget.
//
// Data/logic lives here (C++); the visual layout lives in a WBP subclass.
// The WBP must contain buttons named exactly like the BindWidget properties below.
UCLASS()
class TENETRIS_API UTNTitleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void onPlayClicked();

	UFUNCTION()
	void onHowToPlayClicked();

	UFUNCTION()
	void onCloseHowToPlayClicked();

	UFUNCTION()
	void onQuitClicked();

protected:
	// Level opened when Play is pressed (set in the WBP; defaults to the demo level).
	UPROPERTY(EditAnywhere, Category = "Title")
	FName PlayLevelName = FName("DemoSingle");

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HowToPlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	// Main menu buttons container; hidden while How To Play is shown.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> MainMenuPanel;

	// Shown when How To Play is pressed (hidden by default).
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> HowToPlayPanel;

	// Closes the How To Play panel.
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseHowToPlayButton;
};
