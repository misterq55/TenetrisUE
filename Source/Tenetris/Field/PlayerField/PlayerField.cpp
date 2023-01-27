// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerField.h"
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

	InitializePreviewBuffer();
	InitializePreviewTetrominos();
	InitializeHoldBuffer();
	InitializeHoldTetromino();

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
	UpdateLockDown(DeltaTime);
	WaitForSpawn();
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

bool APlayerField::MoveTetromino(ETetrominoDirection InTetrominoDirection)
{
	if (CurrentTetromino)
	{
		if (!CurrentTetromino->Move(InTetrominoDirection))
		{
			LockDown.CheckRemainCount(InTetrominoDirection);

			return false;
		}
	}

	return true;
}

void APlayerField::RotateTetromino(ETetrominoRotation InTetrominoRotation)
{
	if (CurrentTetromino)
	{
		if (!CurrentTetromino->Rotate(InTetrominoRotation))
		{
			LockDown.CheckRemainCount();
		}
	}
}

void APlayerField::HardDrop()
{
	if (CurrentTetromino)
	{
		CurrentTetromino->HardDrop();
		DoLockDown();
	}
}

void APlayerField::Hold()
{
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
	APlayerController* PlayerController = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	EnableInput(PlayerController);

	if (InputComponent)
	{
		InputComponent->BindAction("StartMoveLeft", EInputEvent::IE_Pressed, this, &APlayerField::StartMoveLeft);
		InputComponent->BindAction("StopMoveLeft", EInputEvent::IE_Released, this, &APlayerField::StopMoveLeft);
		InputComponent->BindAction("StartMoveRight", EInputEvent::IE_Pressed, this, &APlayerField::StartMoveRight);
		InputComponent->BindAction("StopMoveRight", EInputEvent::IE_Released, this, &APlayerField::StopMoveRight);

		InputComponent->BindAction("StartSoftDrop", EInputEvent::IE_Pressed, this, &APlayerField::StartSoftDrop);
		InputComponent->BindAction("StopSoftDrop", EInputEvent::IE_Released, this, &APlayerField::StopSoftDrop);

		InputComponent->BindAction("RotateClockWise", EInputEvent::IE_Pressed, this, &APlayerField::RotateClockWise);
		InputComponent->BindAction("RotateCounterClockWise", EInputEvent::IE_Pressed, this, &APlayerField::RotateCounterClockWise);
		InputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &APlayerField::HardDrop);

		InputComponent->BindAction("Hold", EInputEvent::IE_Pressed, this, &APlayerField::Hold);
	}
}

void APlayerField::UnRegisterActions()
{
	APlayerController* PlayerController = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	DisableInput(PlayerController);
}

void APlayerField::InitializePreviewBuffer()
{
	PreviewTetrominoNum = 5;

	PreviewBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void APlayerField::InitializePreviewTetrominos()
{
	for (int32 i = 0; i < PreviewTetrominoNum; i++)
	{
		FTetrominoBase* PreviewTetromino = new FPreviewTetromino();
		PreviewTetrominos.Add(PreviewTetromino);
		BindTetrominoToBuffer(PreviewTetromino, PreviewBufferComponent);
		PreviewTetromino->SetStartingLocation(2, (PreviewTetrominoNum - i - 1) * 3 + 1);
	}
}

void APlayerField::InitializeHoldBuffer()
{
	HoldBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("HoldBufferComponent"));
	HoldBufferComponent->SetBufferSize(4, 5);
	HoldBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoldBufferComponent->SetMobility(EComponentMobility::Movable);
	HoldBufferComponent->SetRelativeLocation(FVector(0.f, -200.f, 150.f));
	HoldBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void APlayerField::InitializeHoldTetromino()
{
	HoldTetromino = new FPreviewTetromino();
	BindTetrominoToBuffer(HoldTetromino, HoldBufferComponent);
	HoldTetromino->SetStartingLocation(2, 1);
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

void APlayerField::StartMoveLeft()
{
	SetMoveDirection(ETetrominoDirection::Left, true);
}

void APlayerField::StopMoveLeft()
{
	SetMoveDirection(ETetrominoDirection::Left, false);
}

void APlayerField::StartMoveRight()
{
	SetMoveDirection(ETetrominoDirection::Right, true);
}

void APlayerField::StopMoveRight()
{
	SetMoveDirection(ETetrominoDirection::Right, false);
}

void APlayerField::StartSoftDrop()
{
	SetSoftDrop(true);
}

void APlayerField::StopSoftDrop()
{
	SetSoftDrop(false);
}

void APlayerField::RotateClockWise()
{
	RotateTetromino(ETetrominoRotation::ClockWise);
}

void APlayerField::RotateCounterClockWise()
{
	RotateTetromino(ETetrominoRotation::CounterClockWise);
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
	if (bWaitForSpawn)
		return;

	CurrentTime += DeltaTime;
	if (CurrentTime >= GetFallingSpeed())
	{
		if (MoveTetromino(ETetrominoDirection::Down))
		{
			LockDown.StartLockDown();
		}

		CurrentTime = 0.f;
	}
}

float APlayerField::GetFallingSpeed()
{
	float Multiflier = 1.f;

	if (bSoftDrop)
		Multiflier /= 20.f;

	return TetrominoFallingSpeed * Multiflier;
}

void APlayerField::SetMoveState(float DeltaTime, FMoveDirectionState& InMoveState, ETetrominoDirection InTetrominoDirction)
{
	if (bWaitForSpawn)
		return;

	if (InMoveState.Pressed && TetrominoMoveDirection == InTetrominoDirction)
	{
		if (InMoveState.PressedTime == 0.f)
		{
			MoveTetromino(InTetrominoDirction);
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
			MoveTetromino(InTetrominoDirction);
			InMoveState.PressedTime = 0.f;
		}

		InMoveState.PressedTime += DeltaTime;
	}
}

void APlayerField::UpdateLockDown(float DeltaTime)
{
	if (bWaitForSpawn)
		return;

	if (LockDown.UpdateLockDown(DeltaTime))
	{
		DoLockDown();
	}
}

void APlayerField::DoLockDown()
{
	if (CurrentTetromino)
	{
		CurrentTetromino->LockDown();
		LineDelete();
		bWaitForSpawn = true;
	}
}

void APlayerField::LineDelete()
{
	if (CurrentTetromino && TenetrisBufferComponent)
	{
		TenetrisBufferComponent->CheckLineDelete(CurrentTetromino->GetMinoHeights());
	}
}

void APlayerField::WaitForSpawn()
{
	if (bWaitForSpawn && !TenetrisBufferComponent->GetLineDeleting())
	{
		Spawn();
		bWaitForSpawn = false;
	}
}
