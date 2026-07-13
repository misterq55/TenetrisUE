// Fill out your copyright notice in the Description page of Project Settings.


#include "TNFieldBase.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Mino/TNMinoBase.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Components/TenetrisBufferComponent/TNTenetrisBufferComponent.h"

// Sets default values
ATNFieldBase::ATNFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT"));

	TenetrisBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("BufferComponent"));
	TenetrisBufferComponent->SetBufferSize(RowMax, ColumnMax);
	TenetrisBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TenetrisBufferComponent->SetMobility(EComponentMobility::Movable);

	// SetActorTickInterval(1.f);
	MinoClass = ATNMinoBase::StaticClass();
}

ATNFieldBase::~ATNFieldBase()
{

}

// Called when the game starts or when spawned
void ATNFieldBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATNFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotationRemainingTime > 0.f && IsValid(TenetrisBufferComponent))
	{
		RotationRemainingTime -= DeltaTime;
		if (RotationRemainingTime <= 0.f)
		{
			RotationRemainingTime = 0.f;
		}
		
		const float elapsed = RotationDuration - RotationRemainingTime;
		TenetrisBufferComponent->RotateField(FMath::Clamp(elapsed / RotationDuration, 0.f, 1.f), bCachedSpaceInverted);
	}
}

void ATNFieldBase::Initialize()
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->Initialize();
	}
}

void ATNFieldBase::RotateField(const FTNFieldContext& fieldContext)
{
	RotationRemainingTime = RotationDuration;
	bCachedSpaceInverted = fieldContext.bSpaceInverted;
}

void ATNFieldBase::HideTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setVisibilityMino(coord.X + tetrominoInfo->CurrentPosition.X,
						  coord.Y + tetrominoInfo->CurrentPosition.Y, false);
	}
}

void ATNFieldBase::SetTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y,
					tetrominoInfo->CurrentType);
	}
}

void ATNFieldBase::LockDown(const FTNFieldContext& fieldContext)
{
	const int32 bufferHeight = fieldContext.BufferHeight;
	const int32 bufferWidth = fieldContext.BufferWidth;

	for (int32 i = 0; i < bufferHeight; i++)
	{
		for (int32 j = 0; j < bufferWidth; j++)
		{
			const E_TNTetrominoType tetrominoType = fieldContext.CheckBuffer[i + 1][j + 1];
			setBackgroundCubeType(j, i, tetrominoType);
		}
	}
}

void ATNFieldBase::setMinoClassType(TSubclassOf<ATNMinoBase> minoClass)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetMinoClassType(minoClass);
	}
}

void ATNFieldBase::setMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetMinoType(x, y, tetrominoType);
	}
}

void ATNFieldBase::setVisibilityMino(const int32 x, const int32 y, const bool visible)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetVisibilityMino(x, y, visible);
	}
}

void ATNFieldBase::setBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetBackgroundCubeType(x, y, tetrominoType);
	}
}

