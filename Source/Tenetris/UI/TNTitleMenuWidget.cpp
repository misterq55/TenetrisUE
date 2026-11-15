// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/UI/TNTitleMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GeneralProjectSettings.h"
#include "Tenetris/UI/TNOptionsWidget.h"
#include "Tenetris/Input/TNInputBindingSubsystem.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTNTitleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(PlayButton))
	{
		PlayButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onPlayClicked);
	}

	if (IsValid(HowToPlayButton))
	{
		HowToPlayButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onHowToPlayClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onQuitClicked);
	}

	if (IsValid(CloseHowToPlayButton))
	{
		CloseHowToPlayButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onCloseHowToPlayClicked);
	}

	if (IsValid(HowToPlayPanel))
	{
		HowToPlayPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.AddDynamic(this, &UTNTitleMenuWidget::onOptionsClicked);
	}

	if (IsValid(OptionsPanel))
	{
		OptionsPanel->OnCloseRequested.AddDynamic(this, &UTNTitleMenuWidget::onOptionsCloseRequested);
		OptionsPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(VersionText))
	{
		const FString& version = GetDefault<UGeneralProjectSettings>()->ProjectVersion;
		VersionText->SetText(FText::FromString(FString::Printf(TEXT("Build %s"), *version)));
	}
}

void UTNTitleMenuWidget::NativeDestruct()
{
	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onOptionsClicked);
	}

	if (IsValid(OptionsPanel))
	{
		OptionsPanel->OnCloseRequested.RemoveDynamic(this, &UTNTitleMenuWidget::onOptionsCloseRequested);
	}

	if (IsValid(PlayButton))
	{
		PlayButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onPlayClicked);
	}

	if (IsValid(HowToPlayButton))
	{
		HowToPlayButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onHowToPlayClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onQuitClicked);
	}

	if (IsValid(CloseHowToPlayButton))
	{
		CloseHowToPlayButton->OnClicked.RemoveDynamic(this, &UTNTitleMenuWidget::onCloseHowToPlayClicked);
	}

	Super::NativeDestruct();
}

void UTNTitleMenuWidget::onPlayClicked()
{
	if (PlayLevelName.IsNone())
	{
		return;
	}

	UGameplayStatics::OpenLevel(this, PlayLevelName);
}

void UTNTitleMenuWidget::onHowToPlayClicked()
{
	if (IsValid(MainMenuPanel))
	{
		MainMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(HowToPlayPanel))
	{
		refreshActionKeyTexts();
		HowToPlayPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTNTitleMenuWidget::refreshActionKeyTexts()
{
	if (!IsValid(HowToPlayPanel))
	{
		return;
	}

	// Keys may have been rebound in the options panel since the last time the guide was shown.
	UWidgetTree::ForWidgetAndChildren(HowToPlayPanel, [this](UWidget* widget)
	{
		UTextBlock* textBlock = Cast<UTextBlock>(widget);
		if (!IsValid(textBlock))
		{
			return;
		}

		const FText* storedTemplate = KeyTextTemplates.Find(textBlock);
		const FText templateText = storedTemplate ? *storedTemplate : textBlock->GetText();

		const FString& source = templateText.ToString();
		if (!source.Contains(TEXT("{")))
		{
			return;
		}

		if (!storedTemplate)
		{
			KeyTextTemplates.Add(textBlock, templateText);
		}

		textBlock->SetText(FText::FromString(resolveKeyTokens(source)));
	});
}

FString UTNTitleMenuWidget::resolveKeyTokens(const FString& source) const
{
	const ULocalPlayer* localPlayer = GetOwningLocalPlayer();
	const UTNInputBindingSubsystem* bindingSubsystem = IsValid(localPlayer) ? localPlayer->GetSubsystem<UTNInputBindingSubsystem>() : nullptr;

	FString result;
	result.Reserve(source.Len());

	int32 index = 0;
	while (index < source.Len())
	{
		const int32 open = source.Find(TEXT("{"), ESearchCase::CaseSensitive, ESearchDir::FromStart, index);
		if (open == INDEX_NONE)
		{
			result.Append(source.Mid(index));
			break;
		}

		const int32 close = source.Find(TEXT("}"), ESearchCase::CaseSensitive, ESearchDir::FromStart, open + 1);
		if (close == INDEX_NONE)
		{
			result.Append(source.Mid(index));
			break;
		}

		result.Append(source.Mid(index, open - index));

		const FString token = source.Mid(open + 1, close - open - 1);
		const FString rawToken = source.Mid(open, close - open + 1);

		// Token format: Name or Name:Gamepad (keyboard is the default device).
		FString nameString = token;
		FString deviceString;
		token.Split(TEXT(":"), &nameString, &deviceString);

		const E_TNBindingDevice device = deviceString.Equals(TEXT("Gamepad"), ESearchCase::IgnoreCase)
			? E_TNBindingDevice::Gamepad
			: E_TNBindingDevice::Keyboard;

		const FKey key = bindingSubsystem ? bindingSubsystem->GetKey(FName(*nameString.TrimStartAndEnd()), device) : EKeys::Invalid;

		// Leave unresolved tokens visible so typos in the WBP are easy to spot.
		result.Append(key.IsValid() ? UTNInputBindingSubsystem::GetKeyShortName(key).ToString() : rawToken);

		index = close + 1;
	}

	return result;
}

void UTNTitleMenuWidget::onCloseHowToPlayClicked()
{
	if (IsValid(HowToPlayPanel))
	{
		HowToPlayPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(MainMenuPanel))
	{
		MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTNTitleMenuWidget::onOptionsClicked()
{
	if (IsValid(MainMenuPanel))
	{
		MainMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(OptionsPanel))
	{
		OptionsPanel->RefreshFromSettings();
		OptionsPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTNTitleMenuWidget::onOptionsCloseRequested()
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

void UTNTitleMenuWidget::onQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}
