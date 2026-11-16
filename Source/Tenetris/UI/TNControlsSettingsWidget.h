// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TNControlsSettingsWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;
class UWidget;
class UTNFocusRow;
class UTNKeyBindingSelector;
class UTNInputBindingSubsystem;
enum class E_TNBindingDevice : uint8;
enum class E_TNInputDevice : uint8;

// Key binding settings panel, hosted by UTNOptionsWidget.
//
// Rows (action label + keyboard selector + gamepad selector) are created in C++ inside
// the BindingList box. The WBP subclass must contain widgets named exactly like the
// BindWidget properties below.
//
// While the player uses a gamepad, only the gamepad column can be focused; the keyboard
// column becomes editable again as soon as the keyboard or mouse is used.
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

	// Widget that receives focus when the panel is opened with a gamepad or keyboard.
	UWidget* GetFirstFocusWidget() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Wrapper that shows a focus indicator next to each key cell. Optional; without it the
	// selectors are added to the rows directly.
	UPROPERTY(EditAnywhere, Category = "Options")
	TSubclassOf<UTNFocusRow> FocusRowClass;

private:
	void buildRows();
	void refreshKeys();
	void showMessage(const FString& message);
	UTNInputBindingSubsystem* getBindingSubsystem() const;
	UTNKeyBindingSelector* createSelector(const FName actionName, const E_TNBindingDevice device);
	UWidget* wrapInFocusRow(UWidget* content);

	// Makes the keyboard column unreachable while a gamepad is in use.
	void applyDeviceColumnMode();
	E_TNBindingDevice getPreferredDevice() const;
	UTNKeyBindingSelector* findSelector(const FName actionName, const E_TNBindingDevice device) const;

	void handleBindingKeySelected(UTNKeyBindingSelector* selector, const FKey& key);
	void handleInputDeviceChanged(const E_TNInputDevice device);

	UFUNCTION()
	void onResetClicked();

	// Container the rows are added to.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> BindingList;

	// Restores the default keys.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResetButton;

	// Short feedback line (reserved key, swapped keys).
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UTNKeyBindingSelector>> Selectors;
};
