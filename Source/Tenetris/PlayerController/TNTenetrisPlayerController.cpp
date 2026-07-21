// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/Interface/ITNController.h"

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
}

void ATNTenetrisPlayerController::startMoveLeft()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StartMoveLeft);
	}
}

void ATNTenetrisPlayerController::stopMoveLeft()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StopMoveLeft);
	}
}

void ATNTenetrisPlayerController::startMoveRight()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StartMoveRight);
	}
}

void ATNTenetrisPlayerController::stopMoveRight()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StopMoveRight);
	}
}

void ATNTenetrisPlayerController::startSoftDrop()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StartSoftDrop);
	}
}

void ATNTenetrisPlayerController::stopSoftDrop()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::StopSoftDrop);
	}
}

void ATNTenetrisPlayerController::rotateClockWise()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::RotateClockWise);
	}
}

void ATNTenetrisPlayerController::rotateCounterClockWise()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::RotateCounterClockWise);
	}
}

void ATNTenetrisPlayerController::hold()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::Hold);
	}
}

void ATNTenetrisPlayerController::toggleSpaceInversion()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::RotateField);
	}
}

void ATNTenetrisPlayerController::toggleTimeInversion()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::Rewind);
	}
}

void ATNTenetrisPlayerController::hardDrop()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HandleControlInput(E_TNControlType::HardDrop);
	}
}
