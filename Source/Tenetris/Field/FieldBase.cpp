// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldBase.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/Mino/MinoBase.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"

// Sets default values
AFieldBase::AFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT"));

	TenetrisBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("BufferComponent"));
	TenetrisBufferComponent->SetBufferSize(RowMax, ColumnMax);
	TenetrisBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TenetrisBufferComponent->SetMobility(EComponentMobility::Movable);

	// SetActorTickInterval(1.f);
	MinoClass = AMinoBase::StaticClass();
}

AFieldBase::~AFieldBase()
{

}

// Called when the game starts or when spawned
void AFieldBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFieldBase::Initialize()
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->Initialize();
}

void AFieldBase::SetMinoClassType(TSubclassOf<AMinoBase> InMinoClass)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetMinoClassType(InMinoClass);
}

void AFieldBase::SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetBackgroundCubeType(X, Y, InTetrominoType);
}

