// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerField.h"
#include "Tenetris/PlayerController/TenetrisPlayerController.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/PreviewTetromino/PreviewTetromino.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"

// Sets default values
APlayerField::APlayerField()
	: AFieldBase()
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
		FTetrominoBase* PreviewTetromino = new FPreviewTetromino();
		PreviewTetrominos.Add(PreviewTetromino);
		BindTetrominoToBuffer(PreviewTetromino, PreviewBufferComponent);
		PreviewTetromino->SetStartingLocation(0, i * 3);
	}
}

APlayerField::~APlayerField()
{
	if (!CurrentTetromino)
	{
		delete CurrentTetromino;
	}
}

// Called when the game starts or when spawned
void APlayerField::BeginPlay()
{
	Super::BeginPlay();
	RegisterActions();
}

void APlayerField::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegisterActions();
}

// Called every frame
void APlayerField::Tick(float DeltaTime)
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

void APlayerField::Initialize()
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

void APlayerField::MoveTetromino(ETetrominoDirection InTetrominoDirection)
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

void APlayerField::RotateTetromino(ETetrominoRotation InTetrominoRotation)
{
	if (CurrentTetromino)
		CurrentTetromino->Rotate(InTetrominoRotation);
}

void APlayerField::HardDrop()
{

}

void APlayerField::RegisterActions()
{
	ATenetrisPlayerController* PlayerController = Cast<ATenetrisPlayerController>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController)
	{
		PlayerController->OnTetrominoMove.BindUObject(this, &APlayerField::MoveTetromino);
		PlayerController->OnTetrominoRotate.BindUObject(this, &APlayerField::RotateTetromino);
		PlayerController->OnTetrominoHardDrop.BindUObject(this, &APlayerField::HardDrop);
	}
}

void APlayerField::UnRegisterActions()
{
	ATenetrisPlayerController* PlayerController = Cast<ATenetrisPlayerController>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController)
	{
		PlayerController->OnTetrominoMove.Unbind();
		PlayerController->OnTetrominoRotate.Unbind();
		PlayerController->OnTetrominoHardDrop.Unbind();
	}
}

void APlayerField::BindTetrominoToBuffer(FTetrominoBase* InTetromino, UTenetrisBufferComponent* InBuffer)
{
	InTetromino->OnBackgroundCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetBackgroundCubeType);
	InTetromino->OnVisibilityBackgroundCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetVisibilityBackgroundCube);
	InTetromino->OnMinoType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetMinoType);
	InTetromino->OnVisibilityMinoType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetVisibilityMino);
	InTetromino->OnCheckMino.BindUObject(InBuffer, &UTenetrisBufferComponent::CheckMino);
}
