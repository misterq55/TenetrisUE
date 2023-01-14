// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTenetrisField.h"
#include "GameFramework/PlayerInput.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"

// Sets default values
APlayerTenetrisField::APlayerTenetrisField()
	: CurrentTetromino(nullptr)
	, PrevTetromino(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

APlayerTenetrisField::~APlayerTenetrisField()
{
	if (CurrentTetromino)
		delete CurrentTetromino;

	if (PrevTetromino)
		delete PrevTetromino;
}

// Called when the game starts or when spawned
void APlayerTenetrisField::BeginPlay()
{
	Super::BeginPlay();

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveLeft", EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveRight", EKeys::Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MoveDown", EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateClockWise", EKeys::Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RotateCounterClockWise", EKeys::LeftControl));

	APlayerController* PlayerController = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	PlayerController->InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &APlayerTenetrisField::MoveLeft);
	PlayerController->InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &APlayerTenetrisField::MoveRight);
	PlayerController->InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &APlayerTenetrisField::MoveDown);
	PlayerController->InputComponent->BindAction("RotateClockWise", EInputEvent::IE_Pressed, this, &APlayerTenetrisField::RotateClockWise);
	PlayerController->InputComponent->BindAction("RotateCounterClockWise", EInputEvent::IE_Pressed, this, &APlayerTenetrisField::RotateCounterClockWise);

}

// Called every frame
void APlayerTenetrisField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerTenetrisField::Initialize()
{
	
}

void APlayerTenetrisField::MoveLeft()
{
	if (CurrentTetromino)
		CurrentTetromino->Move(ETetrominoDirection::Left);
}

void APlayerTenetrisField::MoveRight()
{
	if (CurrentTetromino)
		CurrentTetromino->Move(ETetrominoDirection::Right);
}

void APlayerTenetrisField::MoveDown()
{
	if (CurrentTetromino)
		CurrentTetromino->Move(ETetrominoDirection::Down);
}

void APlayerTenetrisField::RotateClockWise()
{
	if (CurrentTetromino)
		CurrentTetromino->Rotate(ETetrominoRotation::ClockWise);
}

void APlayerTenetrisField::RotateCounterClockWise()
{
	if (CurrentTetromino)
		CurrentTetromino->Rotate(ETetrominoRotation::CounterClockWise);
}