// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/Interface/ITNController.h"
#include "Tenetris/Audio/TNAudioSubsystem.h"
#include "Tenetris/Audio/TNAudioData.h"
#include "Tenetris/GameMode/DemoSingleGameMode/TNDemoSingleGameMode.h"
#include "Kismet/GameplayStatics.h"

void ATNTenetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Each screen sets its own input state on entry; reset to game input so that
	// a UI-only mode left over from a previous level (e.g. the title menu) does not
	// swallow gameplay input. GameAndUI keeps HUD buttons clickable.
	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(inputMode);
}

void ATNTenetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::startMoveLeft);
	EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATNTenetrisPlayerController::stopMoveLeft);
	EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Canceled, this, &ATNTenetrisPlayerController::stopMoveLeft);

	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::startMoveRight);
	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATNTenetrisPlayerController::stopMoveRight);
	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Canceled, this, &ATNTenetrisPlayerController::stopMoveRight);

	EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::startSoftDrop);
	EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATNTenetrisPlayerController::stopSoftDrop);
	EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Canceled, this, &ATNTenetrisPlayerController::stopSoftDrop);

	EnhancedInputComponent->BindAction(RotateClockWiseAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::rotateClockWise);
	EnhancedInputComponent->BindAction(RotateCounterClockWiseAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::rotateCounterClockWise);
	EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::hardDrop);
	EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::hold);
	EnhancedInputComponent->BindAction(SpaceInversionAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::toggleSpaceInversion);
	EnhancedInputComponent->BindAction(TimeInversionAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::toggleTimeInversion);

	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATNTenetrisPlayerController::togglePause);
}

void ATNTenetrisPlayerController::togglePause()
{
	if (ATNDemoSingleGameMode* gameMode = Cast<ATNDemoSingleGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		gameMode->TogglePause();
	}
}

void ATNTenetrisPlayerController::SetGameplayInputBlocked(const bool bBlocked)
{
	if (bGameplayInputBlocked == bBlocked)
	{
		return;
	}

	if (bBlocked)
	{
		// Release anything held before ignoring input so the piece does not keep moving after resume.
		stopMoveLeft();
		stopMoveRight();
		stopSoftDrop();
	}

	bGameplayInputBlocked = bBlocked;
}

void ATNTenetrisPlayerController::startMoveLeft()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StartMoveLeft);
	}
}

void ATNTenetrisPlayerController::stopMoveLeft()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StopMoveLeft);
	}
}

void ATNTenetrisPlayerController::startMoveRight()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StartMoveRight);
	}
}

void ATNTenetrisPlayerController::stopMoveRight()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StopMoveRight);
	}
}

void ATNTenetrisPlayerController::startSoftDrop()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StartSoftDrop);
	}
}

void ATNTenetrisPlayerController::stopSoftDrop()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StopSoftDrop);
	}
}

void ATNTenetrisPlayerController::rotateClockWise()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::RotateClockWise);
	}
}

void ATNTenetrisPlayerController::rotateCounterClockWise()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::RotateCounterClockWise);
	}
}

void ATNTenetrisPlayerController::hold()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::Hold);
	}
}

void ATNTenetrisPlayerController::toggleSpaceInversion()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::RotateField);
	}

	if (UGameInstance* gameInstance = GetGameInstance())
	{
		if (UTNAudioSubsystem* audio = gameInstance->GetSubsystem<UTNAudioSubsystem>())
		{
			audio->PlaySFX(TNSoundKeys::SpaceInversion);
		}
	}
}

void ATNTenetrisPlayerController::toggleTimeInversion()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::Rewind);
	}
}

void ATNTenetrisPlayerController::hardDrop()
{
	if (bGameplayInputBlocked)
	{
		return;
	}

	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::HardDrop);
	}

	if (UGameInstance* gameInstance = GetGameInstance())
	{
		if (UTNAudioSubsystem* audio = gameInstance->GetSubsystem<UTNAudioSubsystem>())
		{
			audio->PlaySFX(TNSoundKeys::HardDrop);
		}
	}
}
