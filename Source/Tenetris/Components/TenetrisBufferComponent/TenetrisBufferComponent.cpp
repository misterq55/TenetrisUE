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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	BackgroundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	BackgroundMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMeshComponent->SetStaticMesh(MeshObj.Object);
	BackgroundMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	BackGroundCubeBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BackGroundCubeBufferPivot"));
	TetrominoCubeBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("TetrominoCubeBufferPivot"));

	BackGroundCubeBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundCubeBufferPivot->SetMobility(EComponentMobility::Movable);

	TetrominoCubeBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	TetrominoCubeBufferPivot->SetMobility(EComponentMobility::Movable);
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
	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
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

	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
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

	CheckBuffer[Y + 1][X + 1] = 1;
}

void UTenetrisBufferComponent::SetVisibilityBackgroundCube(int32 X, int32 Y, bool InVisible)
{
	ATetrominoCubeBase* TetrominoCube = BackgroundCubeBuffer[Y][X];

	if (TetrominoCube)
	{
		TetrominoCube->SetVitibility(InVisible);
	}

	CheckBuffer[Y + 1][X + 1] = 0;
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

bool UTenetrisBufferComponent::CheckTetrominoCube(int32 X, int32 Y)
{
	return CheckBuffer[Y + 1][X + 1] == 1;
}

void UTenetrisBufferComponent::SetBufferSize(int32 InBufferHeight, int32 InBufferWidth)
{
	BufferHeight = InBufferHeight;
	BufferWidth = InBufferWidth;

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATetrominoCubeBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		BackgroundCubeBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATetrominoCubeBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		TetrominoCubeBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
	{
		TArray<int32> Buffer;
		for (int32 j = 0; j < BufferWidth + 2; j++)
		{
			if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				Buffer.Add(1);
			else
				Buffer.Add(0);
		}

		CheckBuffer.Add(Buffer);
	}

	BackgroundMeshComponent->SetRelativeScale3D(FVector(TetrominoCubeRatio * BufferHeight, TetrominoCubeRatio * BufferWidth, 0.f));
}

