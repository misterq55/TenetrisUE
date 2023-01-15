// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisPlayerController.h"
#include "GameFramework/PlayerInput.h"

void ATenetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveRight", EKeys::Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveDown", EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateClockWise", EKeys::Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateCounterClockWise", EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HardDrop", EKeys::SpaceBar));

	InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::MoveLeft);
	InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::MoveRight);
	InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::MoveDown);
	InputComponent->BindAction("RotateClockWise", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::RotateClockWise);
	InputComponent->BindAction("RotateCounterClockWise", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::RotateCounterClockWise);
	InputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::HardDrop);
}

void ATenetrisPlayerController::MoveLeft()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Left);
}

void ATenetrisPlayerController::MoveRight()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Right);
}

void ATenetrisPlayerController::MoveDown()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Down);
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
