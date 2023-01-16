// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTenetrisField.h"
#include "Tenetris/PlayerController/TenetrisPlayerController.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"

// Sets default values
APlayerTenetrisField::APlayerTenetrisField()
	: ATenetrisFieldBase()
	, CurrentTetromino(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

APlayerTenetrisField::~APlayerTenetrisField()
{
	if (!CurrentTetromino)
	{
		delete CurrentTetromino;
	}
}

// Called when the game starts or when spawned
void APlayerTenetrisField::BeginPlay()
{
	Super::BeginPlay();
	RegisterActions();
}

void APlayerTenetrisField::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegisterActions();
}

// Called every frame
void APlayerTenetrisField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerTenetrisField::Initialize()
{
	Super::Initialize();

	if (!CurrentTetromino)
	{
		CurrentTetromino = new FTetrominoBase();
	}
}

void APlayerTenetrisField::MoveTetromino(ETetrominoDirection InTetrominoDirection)
{
	if (CurrentTetromino)
		CurrentTetromino->Move(InTetrominoDirection);
}

void APlayerTenetrisField::RotateTetromino(ETetrominoRotation InTetrominoRotation)
{
	if (CurrentTetromino)
		CurrentTetromino->Rotate(InTetrominoRotation);
}

void APlayerTenetrisField::HardDrop()
{

}

void APlayerTenetrisField::RegisterActions()
{
	ATenetrisPlayerController* PlayerController = Cast<ATenetrisPlayerController>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController)
	{
		PlayerController->OnTetrominoMove.BindUObject(this, &APlayerTenetrisField::MoveTetromino);
		PlayerController->OnTetrominoRotate.BindUObject(this, &APlayerTenetrisField::RotateTetromino);
		PlayerController->OnTetrominoHardDrop.BindUObject(this, &APlayerTenetrisField::HardDrop);
	}
}

void APlayerTenetrisField::UnRegisterActions()
{
	ATenetrisPlayerController* PlayerController = Cast<ATenetrisPlayerController>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController)
	{
		PlayerController->OnTetrominoMove.Unbind();
		PlayerController->OnTetrominoRotate.Unbind();
		PlayerController->OnTetrominoHardDrop.Unbind();
	}
}
