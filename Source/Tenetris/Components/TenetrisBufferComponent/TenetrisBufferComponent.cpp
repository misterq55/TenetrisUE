// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/TetrominoCube/TetrominoCubeBase.h"

// Sets default values for this component's properties
UTenetrisBufferComponent::UTenetrisBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	BackGroundCubeBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BackGroundCubeBufferPivot"));
	TetrominoCubeBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("TetrominoCubeBufferPivot"));

	BackGroundCubeBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundCubeBufferPivot->SetMobility(EComponentMobility::Movable);

	TetrominoCubeBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	TetrominoCubeBufferPivot->SetMobility(EComponentMobility::Movable);

	for (int32 i = 0; i < RowMax; i++)
	{
		TArray<ATetrominoCubeBase*> Buffer;
		Buffer.Reserve(ColumnMax);

		BackgroundCubeBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < RowMax; i++)
	{
		TArray<ATetrominoCubeBase*> Buffer;
		Buffer.Reserve(ColumnMax);

		TetrominoCubeBuffer.Add(Buffer);
	}
}


// Called when the game starts
void UTenetrisBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTenetrisBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTenetrisBufferComponent::Initialize()
{
	for (int i = 0; i < RowMax; i++)
	{
		for (int j = 0; j < ColumnMax; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			ChildComponent->SetChildActorClass(TetrominoCubeClass);
			ChildComponent->AttachToComponent(BackGroundCubeBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			ATetrominoCubeBase* TetrominoCubeBase = Cast<ATetrominoCubeBase>(ChildComponent->GetChildActor());
			TetrominoCubeBase->SetTetrominoCubePosition(j, i);

			BackgroundCubeBuffer[i].Add(TetrominoCubeBase);
		}
	}

	for (int i = 0; i < RowMax; i++)
	{
		for (int j = 0; j < ColumnMax; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			ChildComponent->SetChildActorClass(TetrominoCubeClass);
			ChildComponent->AttachToComponent(TetrominoCubeBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			ATetrominoCubeBase* TetrominoCubeBase = Cast<ATetrominoCubeBase>(ChildComponent->GetChildActor());
			TetrominoCubeBase->SetTetrominoCubePosition(j, i);

			TetrominoCubeBuffer[i].Add(TetrominoCubeBase);
		}
	}
}

void UTenetrisBufferComponent::SetTetrominoCubeClassType(TSubclassOf<ATetrominoCubeBase> InTetrominoCubeClass)
{
	TetrominoCubeClass = InTetrominoCubeClass;
}

void UTenetrisBufferComponent::SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	ATetrominoCubeBase* TetrominoCube = BackgroundCubeBuffer[Y][X];

	if (TetrominoCube)
	{
		TetrominoCube->SetVitibility(true);
		TetrominoCube->SetTetrominoType(InTetrominoType);
	}
}

void UTenetrisBufferComponent::SetVisibilityBackgroundCube(int32 X, int32 Y, bool InVisible)
{
	ATetrominoCubeBase* TetrominoCube = BackgroundCubeBuffer[Y][X];

	if (TetrominoCube)
	{
		TetrominoCube->SetVitibility(InVisible);
	}
}

void UTenetrisBufferComponent::SetTetrominoCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	ATetrominoCubeBase* TetrominoCube = TetrominoCubeBuffer[Y][X];

	if (TetrominoCube)
	{
		TetrominoCube->SetVitibility(true);
		TetrominoCube->SetTetrominoType(InTetrominoType);
	}
}

void UTenetrisBufferComponent::SetVisibilityTetrominoCube(int32 X, int32 Y, bool InVisible)
{
	ATetrominoCubeBase* TetrominoCube = TetrominoCubeBuffer[Y][X];

	if (TetrominoCube)
	{
		TetrominoCube->SetVitibility(InVisible);
	}
}

