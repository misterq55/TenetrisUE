// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTenetrisField.h"
#include "Tenetris/PlayerController/TenetrisPlayerController.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/PreviewTetromino/PreviewTetromino.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"

// Sets default values
APlayerTenetrisField::APlayerTenetrisField()
	: ATenetrisFieldBase()
	, CurrentTetromino(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PreviewBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(12, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 0.f));

	for (int32 i = 0; i < 5; i++)
	{
		FTetrominoBase* PreviewTetromino = new FPreviewTetromino(this);
		PreviewTetrominos.Add(PreviewTetromino);
		BindTetrominoToBuffer(PreviewTetromino, PreviewBufferComponent);
		PreviewTetromino->SetStartingLocation(0, i * 3);
	}
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

	if (CurrentTetromino)
	{
		if (CurrentTetromino->Move(ETetrominoDirection::Down))
		{
			CurrentTetromino->LockDown();
			CurrentTetromino->SetTetrominoType(ETetrominoType::Z);
			CurrentTetromino->Spawn();
		}
	}
}

void APlayerTenetrisField::Initialize()
{
	Super::Initialize();

	if (!CurrentTetromino)
	{
		CurrentTetromino = new FTetrominoBase();
	}

	PreviewBufferComponent->Initialize();

	for (FTetrominoBase* PreviewTetromino : PreviewTetrominos)
	{
		PreviewTetromino->SetTetrominoType(ETetrominoType::J);
		PreviewTetromino->Spawn();
	}
}

void APlayerTenetrisField::MoveTetromino(ETetrominoDirection InTetrominoDirection)
{
	if (CurrentTetromino)
	{
		if (CurrentTetromino->Move(InTetrominoDirection) && InTetrominoDirection == ETetrominoDirection::Down)
		{
			CurrentTetromino->LockDown();
			CurrentTetromino->Spawn();
		}
	}
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

void APlayerTenetrisField::BindTetrominoToBuffer(FTetrominoBase* InTetromino, UTenetrisBufferComponent* InBuffer)
{
	InTetromino->OnBackgroundCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetBackgroundCubeType);
	InTetromino->OnVisibilityBackgroundCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetVisibilityBackgroundCube);
	InTetromino->OnTetrominoCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetTetrominoCubeType);
	InTetromino->OnVisibilityTetrominoCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetVisibilityTetrominoCube);
	InTetromino->OnCheckTetrominoCube.BindUObject(InBuffer, &UTenetrisBufferComponent::CheckTetrominoCube);
}
