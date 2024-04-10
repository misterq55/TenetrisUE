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

	initializePreviewBuffer();
	initializePreviewTetrominos();
	initializeHoldBuffer();
	initializeHoldTetromino();

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
	registerActions();
}

void APlayerField::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	unRegisterActions();
}

// Called every frame
void APlayerField::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	tetrominoFall(deltaTime);
	setMoveState(deltaTime, LeftDirectionState, ETetrominoDirection::Left);
	setMoveState(deltaTime, RightDirectionState, ETetrominoDirection::Right);
	updateLockDown(deltaTime);
	waitForSpawn();
}

void APlayerField::Initialize()
{
	Super::Initialize();

	if (!CurrentTetromino)
	{
		CurrentTetromino = new FTetrominoBase();
	}

	if (IsValid(PreviewBufferComponent))
	{
		PreviewBufferComponent->Initialize();
	}

	if (IsValid(HoldBufferComponent))
	{
		HoldBufferComponent->Initialize();
	}

	if (TetrominoGenerator)
	{
		TetrominoGenerator->Initialize();
	}
}

bool APlayerField::MoveTetromino(ETetrominoDirection tetrominoDirection)
{
	if (CurrentTetromino)
	{
		if (!CurrentTetromino->Move(tetrominoDirection))
		{
			LockDown.CheckRemainCount(tetrominoDirection);

			return false;
		}
	}

	return true;
}

void APlayerField::RotateTetromino(ETetrominoRotation tetrominoRotation)
{
	if (CurrentTetromino)
	{
		if (!CurrentTetromino->Rotate(tetrominoRotation))
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
		doLockDown();
	}
}

void APlayerField::Hold()
{
	if (!bCanHold || !HoldTetromino || !CurrentTetromino || !HoldTetromino)
		return;

	ETetrominoType HoldTetrominoType = HoldTetromino->GetTetrominoType();
	ETetrominoType CurrentTetrominoType = CurrentTetromino->GetTetrominoType();

	CurrentTetromino->HideTetromino();
	CurrentTetromino->HideGuideTetromino();

	HoldTetromino->HideTetromino();
	HoldTetromino->SetTetrominoType(CurrentTetrominoType);
	HoldTetromino->Spawn();

	if (HoldTetrominoType != ETetrominoType::None)
	{
		CurrentTime = 0.f;
		CurrentTetromino->SetTetrominoType(HoldTetrominoType);
		CurrentTetromino->Spawn();
	}
	else
	{
		CurrentTime = 0.f;
		spawnNextTetromino();
		renewPreviewTetromino();
	}

	bCanHold = false;
}

void APlayerField::ToggleSpaceInversion()
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->ToggleSpaceInversion();
	}

	if (CurrentTetromino)
	{
		CurrentTetromino->ResetGuideTetromino();
	}
}

void APlayerField::SetMoveDirection(ETetrominoDirection tetrominoDirection, bool pressed)
{
	if (pressed)
	{
		if (tetrominoDirection == ETetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = true;
		}
		else if (tetrominoDirection == ETetrominoDirection::Right)
		{
			RightDirectionState.Pressed = true;
		}

		TetrominoMoveDirection = tetrominoDirection;
	}
	else
	{
		if (tetrominoDirection == ETetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = false;
		}
		else if (tetrominoDirection == ETetrominoDirection::Right)
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

void APlayerField::registerActions()
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
		InputComponent->BindAction("SpaceInversion", EInputEvent::IE_Pressed, this, &APlayerField::ToggleSpaceInversion);
	}
}

void APlayerField::unRegisterActions()
{
	APlayerController* PlayerController = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	DisableInput(PlayerController);
}

void APlayerField::initializePreviewBuffer()
{
	PreviewTetrominoNum = 5;

	PreviewBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void APlayerField::initializePreviewTetrominos()
{
	for (int32 i = 0; i < PreviewTetrominoNum; i++)
	{
		FTetrominoBase* previewTetromino = new FPreviewTetromino();
		PreviewTetrominos.Add(previewTetromino);
		BindTetrominoToBuffer(previewTetromino, PreviewBufferComponent);
		previewTetromino->SetStartingLocation(2, (PreviewTetrominoNum - i - 1) * 3 + 1);
	}
}

void APlayerField::initializeHoldBuffer()
{
	HoldBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("HoldBufferComponent"));
	HoldBufferComponent->SetBufferSize(4, 5);
	HoldBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoldBufferComponent->SetMobility(EComponentMobility::Movable);
	HoldBufferComponent->SetRelativeLocation(FVector(0.f, -200.f, 150.f));
	HoldBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void APlayerField::initializeHoldTetromino()
{
	HoldTetromino = new FPreviewTetromino();
	BindTetrominoToBuffer(HoldTetromino, HoldBufferComponent);
	HoldTetromino->SetStartingLocation(2, 1);
}

void APlayerField::BindTetrominoToBuffer(FTetrominoBase* tetromino, UTenetrisBufferComponent* buffer)
{
	if (!tetromino || !IsValid(buffer))
	{
		return;
	}

	tetromino->OnBackgroundCubeType.BindUObject(buffer, &UTenetrisBufferComponent::SetBackgroundCubeType);
	tetromino->OnVisibilityBackgroundCubeType.BindUObject(buffer, &UTenetrisBufferComponent::SetVisibilityBackgroundCube);
	tetromino->OnMinoType.BindUObject(buffer, &UTenetrisBufferComponent::SetMinoType);
	tetromino->OnVisibilityMinoType.BindUObject(buffer, &UTenetrisBufferComponent::SetVisibilityMino);
	tetromino->OnCheckMino.BindUObject(buffer, &UTenetrisBufferComponent::CheckMino);
	tetromino->OnCalulateGuideMino.BindUObject(buffer, &UTenetrisBufferComponent::CalculateGuideMinoHeight);
}

void APlayerField::Spawn()
{
	CurrentTime = 0.f;
	spawnNextTetromino();
	renewPreviewTetromino();
	bCanHold = true;
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

void APlayerField::spawnNextTetromino()
{
	if (CurrentTetromino && TetrominoGenerator)
	{
		CurrentTetromino->SetTetrominoType(TetrominoGenerator->GetTop());
		CurrentTetromino->Spawn();
	}
}

void APlayerField::renewPreviewTetromino()
{
	if (!TetrominoGenerator)
	{
		return;
	}

	for (int32 i = 0; i < PreviewTetrominos.Num(); i++)
	{
		FTetrominoBase* PreviewTetromino = PreviewTetrominos[i];
		PreviewTetromino->HideTetromino();
		PreviewTetromino->SetTetrominoType(TetrominoGenerator->GetAt(i));
		PreviewTetromino->Spawn();
	}
}

void APlayerField::tetrominoFall(float deltaTime)
{
	if (bWaitForSpawn)
	{
		return;
	}

	CurrentTime += deltaTime;
	if (CurrentTime >= getFallingSpeed())
	{
		if (MoveTetromino(ETetrominoDirection::Down))
		{
			LockDown.StartLockDown();
		}

		CurrentTime = 0.f;
	}
}

float APlayerField::getFallingSpeed()
{
	float Multiflier = 1.f;

	if (bSoftDrop)
	{
		Multiflier /= 20.f;
	}

	return TetrominoFallingSpeed * Multiflier;
}

void APlayerField::setMoveState(float deltaTime, FMoveDirectionState& moveState, ETetrominoDirection tetrominoDirction)
{
	if (bWaitForSpawn)
	{
		return;
	}

	if (moveState.Pressed && TetrominoMoveDirection == tetrominoDirction)
	{
		if (moveState.PressedTime == 0.f)
		{
			MoveTetromino(tetrominoDirction);
		}
		else if (moveState.PressedTime > KickInDelay)
		{
			moveState.AutoRepeatKickIn = true;
			moveState.PressedTime = KickInDelay;
		}

		moveState.PressedTime += deltaTime;
	}
	else
	{
		moveState.AutoRepeatKickIn = false;
		moveState.PressedTime = 0.f;
	}

	if (moveState.AutoRepeatKickIn)
	{
		if (moveState.PressedTime >= MoveSpeed)
		{
			MoveTetromino(tetrominoDirction);
			moveState.PressedTime = 0.f;
		}

		moveState.PressedTime += deltaTime;
	}
}

void APlayerField::updateLockDown(float deltaTime)
{
	if (bWaitForSpawn)
	{
		return;
	}

	if (LockDown.UpdateLockDown(deltaTime))
	{
		doLockDown();
	}
}

void APlayerField::doLockDown()
{
	if (CurrentTetromino)
	{
		CurrentTetromino->LockDown();
		lineDelete();
		bWaitForSpawn = true;
	}
}

void APlayerField::lineDelete()
{
	if (CurrentTetromino && IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->CheckLineDelete(CurrentTetromino->GetMinoHeights());
	}
}

void APlayerField::waitForSpawn()
{
	if (bWaitForSpawn && IsValid(TenetrisBufferComponent) && !TenetrisBufferComponent->GetLineDeleting())
	{
		Spawn();
		bWaitForSpawn = false;
	}
}
