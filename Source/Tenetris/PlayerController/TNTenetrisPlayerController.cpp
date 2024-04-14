// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisPlayerController.h"
#include "GameFramework/PlayerInput.h"

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
}
