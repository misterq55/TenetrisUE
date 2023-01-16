// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisFieldBase.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/TetrominoCube/TetrominoCubeBase.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"

// Sets default values
ATenetrisFieldBase::ATenetrisFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT"));

	TenetrisBufferComponent = CreateDefaultSubobject<UTenetrisBufferComponent>(TEXT("BufferComponent"));

	TenetrisBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TenetrisBufferComponent->SetMobility(EComponentMobility::Movable);

	SetActorTickInterval(1.f);
	TetrominoCubeClass = ATetrominoCubeBase::StaticClass();
}

ATenetrisFieldBase::~ATenetrisFieldBase()
{

}

// Called when the game starts or when spawned
void ATenetrisFieldBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATenetrisFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATenetrisFieldBase::Initialize()
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->Initialize();
}

void ATenetrisFieldBase::SetTetrominoCubeClassType(TSubclassOf<ATetrominoCubeBase> InTetrominoCubeClass)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetTetrominoCubeClassType(InTetrominoCubeClass);
}

void ATenetrisFieldBase::SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetBackgroundCubeType(X, Y, InTetrominoType);
}

void ATenetrisFieldBase::SetVisibilityBackgroundCube(int32 X, int32 Y, bool InVisible)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetVisibilityBackgroundCube(X, Y, InVisible);
}

void ATenetrisFieldBase::SetTetrominoCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetTetrominoCubeType(X, Y, InTetrominoType);
}

void ATenetrisFieldBase::SetVisibilityTetrominoCube(int32 X, int32 Y, bool InVisible)
{
	if (TenetrisBufferComponent)
		TenetrisBufferComponent->SetVisibilityTetrominoCube(X, Y, InVisible);
}
