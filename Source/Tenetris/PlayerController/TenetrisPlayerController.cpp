// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisPlayerController.h"
#include "GameFramework/PlayerInput.h"

void ATenetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveRight", EKeys::Right));
	// UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveDown", EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StartSoftDrop", EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("StopSoftDrop", EKeys::Down));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateClockWise", EKeys::Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateCounterClockWise", EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HardDrop", EKeys::SpaceBar));

	InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::MoveLeft);
	InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::MoveRight);
	// InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::MoveDown);
	InputComponent->BindAction("StartSoftDrop", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::StartSoftDrop);
	InputComponent->BindAction("StopSoftDrop", EInputEvent::IE_Released, this, &ATenetrisPlayerController::StopSoftDrop);
	
	InputComponent->BindAction("RotateClockWise", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::RotateClockWise);
	InputComponent->BindAction("RotateCounterClockWise", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::RotateCounterClockWise);
	InputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &ATenetrisPlayerController::HardDrop);
}

//void ATenetrisPlayerController::PlayerTick(float DeltaTime)
//{
//	Super::PlayerTick(DeltaTime);
//}

void ATenetrisPlayerController::MoveLeft()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Left);
}

void ATenetrisPlayerController::MoveRight()
{
	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Right);
}

//void ATenetrisPlayerController::MoveDown()
//{
//	OnTetrominoMove.ExecuteIfBound(ETetrominoDirection::Down);
//}

void ATenetrisPlayerController::StartSoftDrop()
{
	OnToggleSoftDrop.ExecuteIfBound(true);
}

void ATenetrisPlayerController::StopSoftDrop()
{
	OnToggleSoftDrop.ExecuteIfBound(false);
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
