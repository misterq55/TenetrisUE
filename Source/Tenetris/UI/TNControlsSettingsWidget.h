// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNControlsSettingsWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;
class UTNKeyBindingSelector;
class UTNInputBindingSubsystem;
enum class E_TNBindingDevice : uint8;

// Key binding settings panel, hosted by UTNOptionsWidget.
//
// Rows (action label + keyboard selector + gamepad selector) are created in C++ inside
// the BindingList box. The WBP subclass only needs a VerticalBox named exactly
// "BindingList"; the other widgets are optional.
UCLASS()
class TENETRIS_API UTNControlsSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Re-reads the current bindings into the UI (call when the panel is shown).
	UFUNCTION(BlueprintCallable, Category = "Options")
	void RefreshFromSettings();

	// Persists the current bindings.
	UFUNCTION(BlueprintCallable, Category = "Options")
	void SaveSettings();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void buildRows();
	void refreshKeys();
	void showMessage(const FString& message);
	UTNInputBindingSubsystem* getBindingSubsystem() const;
	UTNKeyBindingSelector* createSelector(const FName actionName, const E_TNBindingDevice device);

	void handleBindingKeySelected(UTNKeyBindingSelector* selector, const FKey& key);

	UFUNCTION()
	void onResetClicked();

	// Container the rows are added to.
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> BindingList;

	// Restores the default keys.
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ResetButton;

	// Short feedback line (reserved key, swapped keys).
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> MessageText;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UTNKeyBindingSelector>> Selectors;
};
