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
	
	TetrominoFall(DeltaTime);
	SetMoveState(DeltaTime, LeftDirectionState, ETetrominoDirection::Left);
	SetMoveState(DeltaTime, RightDirectionState, ETetrominoDirection::Right);
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

void APlayerField::SetMoveDirection(ETetrominoDirection InTetrominoDirection, bool InPressed)
{
	if (InPressed)
	{
		if (InTetrominoDirection == ETetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = true;
		}
		else if (InTetrominoDirection == ETetrominoDirection::Right)
		{
			RightDirectionState.Pressed = true;
		}

		TetrominoMoveDirection = InTetrominoDirection;
	}
	else
	{
		if (InTetrominoDirection == ETetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = false;
		}
		else if (InTetrominoDirection == ETetrominoDirection::Right)
		{
			RightDirectionState.Pressed = false;
		}

		if (LeftDirectionState.Pressed)
			TetrominoMoveDirection = ETetrominoDirection::Left;

		if (RightDirectionState.Pressed)
			TetrominoMoveDirection = ETetrominoDirection::Right;

		if (!LeftDirectionState.Pressed && !RightDirectionState.Pressed)
			TetrominoMoveDirection = ETetrominoDirection::None;
	}
}

void APlayerField::RegisterActions()
{
	ATenetrisPlayerController* PlayerController = Cast<ATenetrisPlayerController>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController)
	{
		PlayerController->OnTetrominoMove.BindUObject(this, &APlayerField::SetMoveDirection);
		PlayerController->OnTetrominoRotate.BindUObject(this, &APlayerField::RotateTetromino);
		PlayerController->OnTetrominoHardDrop.BindUObject(this, &APlayerField::HardDrop);
		PlayerController->OnTetrominoSoftDrop.BindUObject(this, &APlayerField::SetSoftDrop);
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
		PlayerController->OnTetrominoSoftDrop.Unbind();
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
	CurrentTime = 0.f;
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

void APlayerField::TetrominoFall(float DeltaTime)
{
	CurrentTime += DeltaTime;
	if (CurrentTime >= GetFallingSpeed())
	{
		if (CurrentTetromino)
		{
			if (CurrentTetromino->Move(ETetrominoDirection::Down))
			{
				CurrentTetromino->LockDown();
				Spawn();
			}
		}

		CurrentTime = 0.f;
	}
}

float APlayerField::GetFallingSpeed()
{
	float Multiflier = 1.f;

	if (SoftDrop)
		Multiflier /= 20.f;

	return TetrominoFallingSpeed * Multiflier;
}

void APlayerField::SetMoveState(float DeltaTime, FMoveDirectionState& InMoveState, ETetrominoDirection InTetrominoDirction)
{
	if (InMoveState.Pressed && TetrominoMoveDirection == InTetrominoDirction)
	{
		if (InMoveState.PressedTime == 0.f)
		{
			if (CurrentTetromino)
				CurrentTetromino->Move(InTetrominoDirction);
		}
		else if (InMoveState.PressedTime > KickInDelay)
		{
			InMoveState.AutoRepeatKickIn = true;
			InMoveState.PressedTime = KickInDelay;
		}

		InMoveState.PressedTime += DeltaTime;
	}
	else
	{
		InMoveState.AutoRepeatKickIn = false;
		InMoveState.PressedTime = 0.f;
	}

	if (InMoveState.AutoRepeatKickIn)
	{
		if (InMoveState.PressedTime >= MoveSpeed)
		{
			if (CurrentTetromino)
				CurrentTetromino->Move(InTetrominoDirction);
			InMoveState.PressedTime = 0.f;
		}

		InMoveState.PressedTime += DeltaTime;
	}
}
