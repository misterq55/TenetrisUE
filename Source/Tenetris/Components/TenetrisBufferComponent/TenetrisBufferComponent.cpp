// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/Mino/MinoBase.h"

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
	MinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MinoBufferPivot"));

	BackGroundCubeBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundCubeBufferPivot->SetMobility(EComponentMobility::Movable);

	MinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	MinoBufferPivot->SetMobility(EComponentMobility::Movable);
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
			ChildComponent->SetChildActorClass(MinoClass);
			ChildComponent->AttachToComponent(BackGroundCubeBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			AMinoBase* MinoBase = Cast<AMinoBase>(ChildComponent->GetChildActor());
			MinoBase->SetMinoPosition(j, i);

			BackgroundCubeBuffer[i].Add(MinoBase);
		}
	}

	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			ChildComponent->SetChildActorClass(MinoClass);
			ChildComponent->AttachToComponent(MinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			AMinoBase* MinoBase = Cast<AMinoBase>(ChildComponent->GetChildActor());
			MinoBase->SetMinoPosition(j, i);

			MinoBuffer[i].Add(MinoBase);
		}
	}
}

void UTenetrisBufferComponent::SetMinoClassType(TSubclassOf<AMinoBase> InMinoClass)
{
	MinoClass = InMinoClass;
}

void UTenetrisBufferComponent::SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	AMinoBase* Mino = BackgroundCubeBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(true);
		Mino->SetTetrominoType(InTetrominoType);
	}

	CheckBuffer[Y + 1][X + 1] = 1;
}

void UTenetrisBufferComponent::SetVisibilityBackgroundCube(int32 X, int32 Y, bool InVisible)
{
	AMinoBase* Mino = BackgroundCubeBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(InVisible);
	}

	CheckBuffer[Y + 1][X + 1] = 0;
}

void UTenetrisBufferComponent::SetMinoType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	AMinoBase* Mino = MinoBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(true);
		Mino->SetTetrominoType(InTetrominoType);
	}
}

void UTenetrisBufferComponent::SetVisibilityMino(int32 X, int32 Y, bool InVisible)
{
	AMinoBase* Mino = MinoBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(InVisible);
	}
}

bool UTenetrisBufferComponent::CheckMino(int32 X, int32 Y)
{
	return CheckBuffer[Y + 1][X + 1] == 1;
}

void UTenetrisBufferComponent::SetBufferSize(int32 InBufferHeight, int32 InBufferWidth)
{
	BufferHeight = InBufferHeight;
	BufferWidth = InBufferWidth;

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<AMinoBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		BackgroundCubeBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<AMinoBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		MinoBuffer.Add(Buffer);
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

	BackgroundMeshComponent->SetRelativeScale3D(FVector(MinoRatio * BufferHeight, MinoRatio * BufferWidth, 0.f));
}

