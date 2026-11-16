// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNHowToPlayRowWidget.generated.h"

class UTextBlock;

// One line of the How To Play guide: action name + current keyboard key + current gamepad key.
//
// Place instances in the How To Play panel and set MappingName per instance; the keys are
// read from UTNInputBindingSubsystem, so the guide follows the player's rebinding.
// The WBP subclass must contain text blocks named exactly like the BindWidget properties below.
UCLASS()
class TENETRIS_API UTNHowToPlayRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Re-reads the action's current keys (call when the guide is shown).
	UFUNCTION(BlueprintCallable, Category = "How To Play")
	void Refresh();

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// Player mappable key name of the action (Player Mappable Key Settings > Name, e.g. MoveLeft).
	UPROPERTY(EditAnywhere, Category = "How To Play")
	FName MappingName;

	// Label shown for the action; empty uses the action's DisplayName from the input settings.
	UPROPERTY(EditAnywhere, Category = "How To Play")
	FText DisplayName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyboardKeyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GamepadKeyText;
};
