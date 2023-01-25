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

	InputComponent->BindAction("StartMoveLeft", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::StartMoveLeft);
	InputComponent->BindAction("StopMoveLeft", EInputEvent::IE_Released, this, &ATenetrisPlayerController::StopMoveLeft);
	InputComponent->BindAction("StartMoveRight", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::StartMoveRight);
	InputComponent->BindAction("StopMoveRight", EInputEvent::IE_Released, this, &ATenetrisPlayerController::StopMoveRight);
	
	InputComponent->BindAction("StartSoftDrop", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::StartSoftDrop);
	InputComponent->BindAction("StopSoftDrop", EInputEvent::IE_Released, this, &ATenetrisPlayerController::StopSoftDrop);
	
	InputComponent->BindAction("RotateClockWise", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::RotateClockWise);
	InputComponent->BindAction("RotateCounterClockWise", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::RotateCounterClockWise);
	InputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::HardDrop);
}

void ATenetrisPlayerController::StartMoveLeft()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Left, true);
}

void ATenetrisPlayerController::StopMoveLeft()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Left, false);
}

void ATenetrisPlayerController::StartMoveRight()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Right, true);
}

void ATenetrisPlayerController::StopMoveRight()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Right, false);
}

void ATenetrisPlayerController::StartSoftDrop()
{
	OnTetrominoSoftDrop.ExecuteIfBound(true);
}

void ATenetrisPlayerController::StopSoftDrop()
{
	OnTetrominoSoftDrop.ExecuteIfBound(false);
}

void ATenetrisPlayerController::RotateClockWise()
{
	OnTetrominoRotate.ExecuteIfBound(ETetrominoRotation::ClockWise);
}

void ATenetrisPlayerController::RotateCounterClockWise()
{
	OnTetrominoRotate.ExecuteIfBound(ETetrominoRotation::CounterClockWise);
}

void ATenetrisPlayerController::HardDrop()
{
	OnTetrominoHardDrop.ExecuteIfBound();
}
