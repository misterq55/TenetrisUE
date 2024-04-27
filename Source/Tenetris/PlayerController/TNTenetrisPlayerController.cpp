// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Controller/TNController.h"

void ATNTenetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("startMoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("stopMoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("startMoveRight", EKeys::Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("stopMoveRight", EKeys::Right));
	
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("startSoftDrop", EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("stopSoftDrop", EKeys::Down));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("rotateClockWise", EKeys::Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("rotateCounterClockWise", EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HardDrop", EKeys::SpaceBar));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Hold", EKeys::LeftShift));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SpaceInversion", EKeys::Z));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("startMoveLeft", EKeys::Gamepad_DPad_Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("stopMoveLeft", EKeys::Gamepad_DPad_Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("startMoveRight", EKeys::Gamepad_DPad_Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("stopMoveRight", EKeys::Gamepad_DPad_Right));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("startSoftDrop", EKeys::Gamepad_DPad_Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("stopSoftDrop", EKeys::Gamepad_DPad_Down));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("rotateClockWise", EKeys::Gamepad_FaceButton_Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("rotateCounterClockWise", EKeys::Gamepad_FaceButton_Bottom));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HardDrop", EKeys::Gamepad_DPad_Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Hold", EKeys::Gamepad_LeftShoulder));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SpaceInversion", EKeys::Gamepad_FaceButton_Top));

	if (IsValid(InputComponent))
	{
		InputComponent->BindAction("startMoveLeft", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::startMoveLeft);
		InputComponent->BindAction("stopMoveLeft", EInputEvent::IE_Released, this, &ATNTenetrisPlayerController::stopMoveLeft);
		InputComponent->BindAction("startMoveRight", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::startMoveRight);
		InputComponent->BindAction("stopMoveRight", EInputEvent::IE_Released, this, &ATNTenetrisPlayerController::stopMoveRight);

		InputComponent->BindAction("startSoftDrop", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::startSoftDrop);
		InputComponent->BindAction("stopSoftDrop", EInputEvent::IE_Released, this, &ATNTenetrisPlayerController::stopSoftDrop);

		InputComponent->BindAction("rotateClockWise", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::rotateClockWise);
		InputComponent->BindAction("rotateCounterClockWise", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::rotateCounterClockWise);
		InputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::HardDrop);

		InputComponent->BindAction("Hold", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::hold);
		InputComponent->BindAction("SpaceInversion", EInputEvent::IE_Pressed, this, &ATNTenetrisPlayerController::toggleSpaceInversion);
	}
}

void ATNTenetrisPlayerController::startMoveLeft()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->StartMoveLeft();
	}
}

void ATNTenetrisPlayerController::stopMoveLeft()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->StopMoveLeft();
	}
}

void ATNTenetrisPlayerController::startMoveRight()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->StartMoveRight();
	}
}

void ATNTenetrisPlayerController::stopMoveRight()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->StopMoveRight();
	}
}

void ATNTenetrisPlayerController::startSoftDrop()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->StartSoftDrop();
	}
}

void ATNTenetrisPlayerController::stopSoftDrop()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->StopSoftDrop();
	}
}

void ATNTenetrisPlayerController::rotateClockWise()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->RotateClockWise();
	}
}

void ATNTenetrisPlayerController::rotateCounterClockWise()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->RotateCounterClockWise();
	}
}

void ATNTenetrisPlayerController::hold()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->Hold();
	}
}

void ATNTenetrisPlayerController::toggleSpaceInversion()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->ToggleSpaceInversion();
	}
}

void ATNTenetrisPlayerController::HardDrop()
{
	TSharedPtr<ITNController> tnController = FTNMVCHolder::GetInstance().GetController();
	if (tnController.IsValid())
	{
		tnController->HardDrop();
	}
}
