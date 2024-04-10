// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisPlayerController.h"
#include "GameFramework/PlayerInput.h"

void ATNTenetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartMoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopMoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartMoveRight", EKeys::Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopMoveRight", EKeys::Right));
	
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartSoftDrop", EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopSoftDrop", EKeys::Down));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateClockWise", EKeys::Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateCounterClockWise", EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HardDrop", EKeys::SpaceBar));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Hold", EKeys::LeftShift));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SpaceInversion", EKeys::Z));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartMoveLeft", EKeys::Gamepad_DPad_Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopMoveLeft", EKeys::Gamepad_DPad_Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartMoveRight", EKeys::Gamepad_DPad_Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopMoveRight", EKeys::Gamepad_DPad_Right));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartSoftDrop", EKeys::Gamepad_DPad_Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopSoftDrop", EKeys::Gamepad_DPad_Down));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateClockWise", EKeys::Gamepad_FaceButton_Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateCounterClockWise", EKeys::Gamepad_FaceButton_Bottom));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HardDrop", EKeys::Gamepad_DPad_Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Hold", EKeys::Gamepad_LeftShoulder));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SpaceInversion", EKeys::Gamepad_FaceButton_Top));
}
