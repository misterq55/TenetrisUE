// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisBufferComponent.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Mino/TNMinoBase.h"

// Sets default values for this component's properties
UTNTenetrisBufferComponent::UTNTenetrisBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	BackgroundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BACKGROUNDMESH_COMPONENT"));
	BackgroundMeshComponent->SetStaticMesh(MeshObj.Object);
	BackgroundMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMeshComponent->SetMobility(EComponentMobility::Movable);
	BackgroundMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	BackGroundMinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BackGroundMinoBufferPivot"));
	MinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MinoBufferPivot"));

	BackGroundMinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundMinoBufferPivot->SetMobility(EComponentMobility::Movable);

	MinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	MinoBufferPivot->SetMobility(EComponentMobility::Movable);
}


// Called when the game starts
void UTNTenetrisBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTNTenetrisBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTNTenetrisBufferComponent::buildMinoLayer(USceneComponent* pivot, TArray<TArray<ATNMinoBase*>>& targetBuffer)
{
	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		for (int32 j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* childComponent = NewObject<UChildActorComponent>(this);

			if (!IsValid(childComponent))
			{
				continue;
			}

			childComponent->SetChildActorClass(MinoClass);
			childComponent->AttachToComponent(pivot, FAttachmentTransformRules::KeepRelativeTransform);
			childComponent->CreateChildActor();

			ATNMinoBase* minoBase = Cast<ATNMinoBase>(childComponent->GetChildActor());

			if (!IsValid(minoBase))
			{
				continue;
			}

			minoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));
			targetBuffer[i].Add(minoBase);
		}
	}
}

void UTNTenetrisBufferComponent::Initialize()
{
	buildMinoLayer(BackGroundMinoBufferPivot, BackgroundCubeBuffer);
	buildMinoLayer(MinoBufferPivot, MinoBuffer);
}

void UTNTenetrisBufferComponent::SetBufferSize(const int32 bufferHeight, const int32 bufferWidth)
{
	BufferHeight = bufferHeight;
	BufferWidth = bufferWidth;

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATNMinoBase*> buffer;
		buffer.Reserve(BufferWidth);

		BackgroundCubeBuffer.Add(buffer);
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATNMinoBase*> buffer;
		buffer.Reserve(BufferWidth);

		MinoBuffer.Add(buffer);
	}

	if (IsValid(BackgroundMeshComponent))
	{
		BackgroundMeshComponent->SetRelativeScale3D(FVector(MinoRatio * BufferHeight, MinoRatio * BufferWidth, 1));
	}
}

void UTNTenetrisBufferComponent::SetMinoClassType(const TSubclassOf<ATNMinoBase> minoClass)
{
	MinoClass = minoClass;
}

void UTNTenetrisBufferComponent::SetBackgroundCubeType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	ATNMinoBase* const mino = BackgroundCubeBuffer[y][x];

	if (IsValid(mino))
	{
		mino->SetVitibility(E_TNTetrominoType::None != tetrominoType);
		mino->ShowTetrominoType(tetrominoType);
	}
}

void UTNTenetrisBufferComponent::SetVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible)
{
	ATNMinoBase* const mino = BackgroundCubeBuffer[y][x];

	if (IsValid(mino))
	{
		mino->SetVitibility(visible);
	}
}

void UTNTenetrisBufferComponent::SetMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	ATNMinoBase* const mino = MinoBuffer[y][x];

	if (IsValid(mino))
	{
		mino->SetVitibility(true);
		mino->ShowTetrominoType(tetrominoType);
	}
}

void UTNTenetrisBufferComponent::SetVisibilityMino(const int32 x, const int32 y, const bool visible)
{
	ATNMinoBase* const mino = MinoBuffer[y][x];

	if (IsValid(mino))
	{
		mino->SetVitibility(visible);
	}
}

void UTNTenetrisBufferComponent::RotateField(const float alpha, const bool bSpaceInverted) const
{
	if (IsValid(BackGroundMinoBufferPivot))
	{
		const float clampedAlpha = FMath::Clamp(alpha, 0.f, 1.f);
		const float startYaw = bSpaceInverted ? 0.f : 180.f;
		const float targetYaw = bSpaceInverted ? 180.f : 0.f;

		FRotator newRotation = BackGroundMinoBufferPivot->GetRelativeRotation();
		newRotation.Yaw = FMath::Lerp(startYaw, targetYaw, clampedAlpha);
		BackGroundMinoBufferPivot->SetRelativeRotation(newRotation);
	}
}

