// Fill out your copyright notice in the Description page of Project Settings.


#include "TNPlayerField.h"
#include "Tenetris/Field/Tetromino/TNTetrominoBase.h"
#include "Tenetris/Field/Tetromino/previewTetromino/TNPreviewTetromino.h"
#include "Tenetris/Components/TenetrisBufferComponent/TNTenetrisBufferComponent.h"
#include "Tenetris/Field/TetrominoGenerator/TNTetrominoGenerator.h"

// Sets default values
ATNPlayerField::ATNPlayerField()
	: ATNFieldBase()
	, CurrentTetromino(nullptr)
	, PreviewTetrominoNum(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	initializePreviewBuffer();
	initializePreviewTetrominos();
	initializeHoldBuffer();
	initializeHoldTetromino();

	TetrominoGenerator = new FTNTetrominoGenerator();
}

ATNPlayerField::~ATNPlayerField()
{
	if (CurrentTetromino)
	{
		delete CurrentTetromino;
	}

	while (PreviewTetrominos.Num() != 0)
	{
		FTNTetrominoBase* previewTetrominoToDelete = PreviewTetrominos.Pop();
		delete previewTetrominoToDelete;
	}

	if (TetrominoGenerator)
	{
		delete TetrominoGenerator;
	}
}

// Called when the game starts or when spawned
void ATNPlayerField::BeginPlay()
{
	Super::BeginPlay();
	registerActions();
}

void ATNPlayerField::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	unRegisterActions();
}

// Called every frame
void ATNPlayerField::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	tetrominoFall(deltaTime);
	setMoveState(deltaTime, LeftDirectionState, E_TNTetrominoDirection::Left);
	setMoveState(deltaTime, RightDirectionState, E_TNTetrominoDirection::Right);
	updateLockDown(deltaTime);
	waitForSpawn();
}

void ATNPlayerField::Initialize()
{
	Super::Initialize();

	if (!CurrentTetromino)
	{
		CurrentTetromino = new FTNTetrominoBase();
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

bool ATNPlayerField::MoveTetromino(E_TNTetrominoDirection tetrominoDirection)
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

void ATNPlayerField::RotateTetromino(E_TNTetrominoRotation tetrominoRotation)
{
	if (CurrentTetromino)
	{
		if (!CurrentTetromino->Rotate(tetrominoRotation))
		{
			LockDown.CheckRemainCount();
		}
	}
}

void ATNPlayerField::HardDrop()
{
	if (CurrentTetromino)
	{
		CurrentTetromino->HardDrop();
		doLockDown();
	}
}

void ATNPlayerField::hold()
{
	if (!bCanHold || !HoldTetromino || !CurrentTetromino || !HoldTetromino)
		return;

	E_TNTetrominoType holdTetrominoType = HoldTetromino->GetTetrominoType();
	E_TNTetrominoType currentTetrominoType = CurrentTetromino->GetTetrominoType();

	CurrentTetromino->HideTetromino();
	CurrentTetromino->HideGuideTetromino();

	HoldTetromino->HideTetromino();
	HoldTetromino->SetTetrominoType(currentTetrominoType);
	HoldTetromino->Spawn();

	if (holdTetrominoType != E_TNTetrominoType::None)
	{
		CurrentTime = 0.f;
		CurrentTetromino->SetTetrominoType(holdTetrominoType);
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

void ATNPlayerField::toggleSpaceInversion()
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

void ATNPlayerField::SetMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed)
{
	if (pressed)
	{
		if (tetrominoDirection == E_TNTetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = true;
		}
		else if (tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			RightDirectionState.Pressed = true;
		}

		TetrominoMoveDirection = tetrominoDirection;
	}
	else
	{
		if (tetrominoDirection == E_TNTetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = false;
		}
		else if (tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			RightDirectionState.Pressed = false;
		}

		if (LeftDirectionState.Pressed)
			TetrominoMoveDirection = E_TNTetrominoDirection::Left;

		if (RightDirectionState.Pressed)
			TetrominoMoveDirection = E_TNTetrominoDirection::Right;

		if (!LeftDirectionState.Pressed && !RightDirectionState.Pressed)
			TetrominoMoveDirection = E_TNTetrominoDirection::None;
	}
}

void ATNPlayerField::registerActions()
{
	APlayerController* playerController = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld());

	if (!IsValid(playerController))
	{
		return;
	}

	EnableInput(playerController);

	if (InputComponent)
	{
		InputComponent->BindAction("startMoveLeft", EInputEvent::IE_Pressed, this, &ATNPlayerField::startMoveLeft);
		InputComponent->BindAction("stopMoveLeft", EInputEvent::IE_Released, this, &ATNPlayerField::stopMoveLeft);
		InputComponent->BindAction("startMoveRight", EInputEvent::IE_Pressed, this, &ATNPlayerField::startMoveRight);
		InputComponent->BindAction("stopMoveRight", EInputEvent::IE_Released, this, &ATNPlayerField::stopMoveRight);

		InputComponent->BindAction("startSoftDrop", EInputEvent::IE_Pressed, this, &ATNPlayerField::startSoftDrop);
		InputComponent->BindAction("stopSoftDrop", EInputEvent::IE_Released, this, &ATNPlayerField::stopSoftDrop);

		InputComponent->BindAction("rotateClockWise", EInputEvent::IE_Pressed, this, &ATNPlayerField::rotateClockWise);
		InputComponent->BindAction("rotateCounterClockWise", EInputEvent::IE_Pressed, this, &ATNPlayerField::rotateCounterClockWise);
		InputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &ATNPlayerField::HardDrop);

		InputComponent->BindAction("Hold", EInputEvent::IE_Pressed, this, &ATNPlayerField::hold);
		InputComponent->BindAction("SpaceInversion", EInputEvent::IE_Pressed, this, &ATNPlayerField::toggleSpaceInversion);
	}
}

void ATNPlayerField::unRegisterActions()
{
	APlayerController* playerController = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	DisableInput(playerController);
}

void ATNPlayerField::initializePreviewBuffer()
{
	PreviewTetrominoNum = 5;

	PreviewBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void ATNPlayerField::initializePreviewTetrominos()
{
	for (int32 i = 0; i < PreviewTetrominoNum; i++)
	{
		FTNTetrominoBase* previewTetromino = new FTNPreviewTetromino();
		PreviewTetrominos.Add(previewTetromino);
		bindTetrominoToBuffer(previewTetromino, PreviewBufferComponent);
		previewTetromino->SetStartingLocation(2, (PreviewTetrominoNum - i - 1) * 3 + 1);
	}
}

void ATNPlayerField::initializeHoldBuffer()
{
	HoldBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("HoldBufferComponent"));
	HoldBufferComponent->SetBufferSize(4, 5);
	HoldBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoldBufferComponent->SetMobility(EComponentMobility::Movable);
	HoldBufferComponent->SetRelativeLocation(FVector(0.f, -200.f, 150.f));
	HoldBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void ATNPlayerField::initializeHoldTetromino()
{
	HoldTetromino = new FTNPreviewTetromino();
	bindTetrominoToBuffer(HoldTetromino, HoldBufferComponent);
	HoldTetromino->SetStartingLocation(2, 1);
}

void ATNPlayerField::bindTetrominoToBuffer(FTNTetrominoBase* tetromino, UTNTenetrisBufferComponent* buffer)
{
	if (!tetromino || !IsValid(buffer))
	{
		return;
	}

	tetromino->OnBackgroundCubeType.BindUObject(buffer, &UTNTenetrisBufferComponent::SetBackgroundCubeType);
	tetromino->OnVisibilityBackgroundCubeType.BindUObject(buffer, &UTNTenetrisBufferComponent::SetVisibilityBackgroundCube);
	tetromino->OnMinoType.BindUObject(buffer, &UTNTenetrisBufferComponent::SetMinoType);
	tetromino->OnVisibilityMinoType.BindUObject(buffer, &UTNTenetrisBufferComponent::SetVisibilityMino);
	tetromino->OnCheckMino.BindUObject(buffer, &UTNTenetrisBufferComponent::CheckMino);
	tetromino->OnCalulateGuideMino.BindUObject(buffer, &UTNTenetrisBufferComponent::CalculateGuideMinoHeight);
}

void ATNPlayerField::spawn()
{
	CurrentTime = 0.f;
	spawnNextTetromino();
	renewPreviewTetromino();
	bCanHold = true;
}

void ATNPlayerField::startMoveLeft()
{
	SetMoveDirection(E_TNTetrominoDirection::Left, true);
}

void ATNPlayerField::stopMoveLeft()
{
	SetMoveDirection(E_TNTetrominoDirection::Left, false);
}

void ATNPlayerField::startMoveRight()
{
	SetMoveDirection(E_TNTetrominoDirection::Right, true);
}

void ATNPlayerField::stopMoveRight()
{
	SetMoveDirection(E_TNTetrominoDirection::Right, false);
}

void ATNPlayerField::startSoftDrop()
{
	SetSoftDrop(true);
}

void ATNPlayerField::stopSoftDrop()
{
	SetSoftDrop(false);
}

void ATNPlayerField::rotateClockWise()
{
	RotateTetromino(E_TNTetrominoRotation::ClockWise);
}

void ATNPlayerField::rotateCounterClockWise()
{
	RotateTetromino(E_TNTetrominoRotation::CounterClockWise);
}

void ATNPlayerField::spawnNextTetromino()
{
	if (CurrentTetromino && TetrominoGenerator)
	{
		CurrentTetromino->SetTetrominoType(TetrominoGenerator->GetTop());
		CurrentTetromino->Spawn();
	}
}

void ATNPlayerField::renewPreviewTetromino()
{
	if (!TetrominoGenerator)
	{
		return;
	}

	for (int32 i = 0; i < PreviewTetrominos.Num(); i++)
	{
		FTNTetrominoBase* previewTetromino = PreviewTetrominos[i];
		previewTetromino->HideTetromino();
		previewTetromino->SetTetrominoType(TetrominoGenerator->GetAt(i));
		previewTetromino->Spawn();
	}
}

void ATNPlayerField::tetrominoFall(float deltaTime)
{
	if (bWaitForSpawn)
	{
		return;
	}

	CurrentTime += deltaTime;
	if (CurrentTime >= getFallingSpeed())
	{
		if (MoveTetromino(E_TNTetrominoDirection::Down))
		{
			LockDown.StartLockDown();
		}

		CurrentTime = 0.f;
	}
}

float ATNPlayerField::getFallingSpeed()
{
	float multiflier = 1.f;

	if (bSoftDrop)
	{
		multiflier /= 20.f;
	}

	return TetrominoFallingSpeed * multiflier;
}

void ATNPlayerField::setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirction)
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

void ATNPlayerField::updateLockDown(float deltaTime)
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

void ATNPlayerField::doLockDown()
{
	if (CurrentTetromino)
	{
		CurrentTetromino->LockDown();
		lineDelete();
		bWaitForSpawn = true;
	}
}

void ATNPlayerField::lineDelete()
{
	if (CurrentTetromino && IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->CheckLineDelete(CurrentTetromino->GetMinoHeights());
	}
}

void ATNPlayerField::waitForSpawn()
{
	if (bWaitForSpawn && IsValid(TenetrisBufferComponent) && !TenetrisBufferComponent->GetLineDeleting())
	{
		spawn();
		bWaitForSpawn = false;
	}
}
