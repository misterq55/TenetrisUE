// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerField.h"
#include "Tenetris/PlayerController/TenetrisPlayerController.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/PreviewTetromino/PreviewTetromino.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"
#include "Tenetris/Field/TetrominoGenerator/TetrominoGenerator.h"

// Sets default values
APlayerField::APlayerField()
	: AFieldBase()
	, CurrentTetromino(nullptr)
	, PreviewTetrominoNum(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PreviewTetrominoNum = 5;

	PreviewBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	InitializePreviewTetrominos();

	TetrominoGenerator = new FTetrominoGenerator();
}

APlayerField::~APlayerField()
{
	if (CurrentTetromino)
	{
		delete CurrentTetromino;
	}

	while (PreviewTetrominos.Num() != 0)
	{
		FTetrominoBase* PreviewTetrominoToDelete = PreviewTetrominos.Pop();
		delete PreviewTetrominoToDelete;
	}

	if (TetrominoGenerator)
	{
		delete TetrominoGenerator;
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
			Spawn();
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

	TetrominoGenerator->Initialize();
}

void APlayerField::MoveTetromino(ETetrominoDirection InTetrominoDirection)
{
	if (CurrentTetromino)
	{
		if (CurrentTetromino->Move(InTetrominoDirection) && InTetrominoDirection == ETetrominoDirection::Down)
		{
			CurrentTetromino->LockDown();
			Spawn();
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
	if (CurrentTetromino)
	{
		CurrentTetromino->HardDrop();
		CurrentTetromino->LockDown();
		Spawn();
	}
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

void APlayerField::InitializePreviewTetrominos()
{
	for (int32 i = 0; i < PreviewTetrominoNum; i++)
	{
		FTetrominoBase* PreviewTetromino = new FPreviewTetromino();
		PreviewTetrominos.Add(PreviewTetromino);
		BindTetrominoToBuffer(PreviewTetromino, PreviewBufferComponent);
		PreviewTetromino->SetTetrominoType(ETetrominoType(i));
		PreviewTetromino->SetStartingLocation(2, (PreviewTetrominoNum - i - 1) * 3 + 1);
	}
}

void APlayerField::BindTetrominoToBuffer(FTetrominoBase* InTetromino, UTenetrisBufferComponent* InBuffer)
{
	InTetromino->OnBackgroundCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetBackgroundCubeType);
	InTetromino->OnVisibilityBackgroundCubeType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetVisibilityBackgroundCube);
	InTetromino->OnMinoType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetMinoType);
	InTetromino->OnVisibilityMinoType.BindUObject(InBuffer, &UTenetrisBufferComponent::SetVisibilityMino);
	InTetromino->OnCheckMino.BindUObject(InBuffer, &UTenetrisBufferComponent::CheckMino);
	InTetromino->OnCalulateGuideMino.BindUObject(InBuffer, &UTenetrisBufferComponent::CalculateGuideMinoHeight);
}

void APlayerField::Spawn()
{
	SpawnNextTetromino();
	RenewPreviewTetromino();
}

void APlayerField::SpawnNextTetromino()
{
	if (CurrentTetromino)
	{
		CurrentTetromino->SetTetrominoType(TetrominoGenerator->GetTop());
		CurrentTetromino->Spawn();
	}
}

void APlayerField::RenewPreviewTetromino()
{
	for (int32 i = 0; i < PreviewTetrominos.Num(); i++)
	{
		FTetrominoBase* PreviewTetromino = PreviewTetrominos[i];
		PreviewTetromino->HideTetromino();
		PreviewTetromino->SetTetrominoType(TetrominoGenerator->GetAt(i));
		PreviewTetromino->Spawn();
	}
}
