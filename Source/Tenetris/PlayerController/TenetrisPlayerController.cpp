// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisPlayerController.h"
#include "GameFramework/PlayerInput.h"

void ATenetrisPlayerController::SetupInputComponent()
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
}
