// Fill out your copyright notice in the Description page of Project Settings.


#include "MinoBase.h"

// Sets default values
AMinoBase::AMinoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CUBEMESH_COMPONENT"));
	CubeMeshComponent->SetupAttachment(RootComponent);
	CubeMeshComponent->SetStaticMesh(MeshObj.Object);
	CubeMeshComponent->SetRelativeScale3D(FVector(MinoRatio, MinoRatio, MinoRatio));

	CubeMeshComponent->SetVisibility(false);

	RootComponent = CubeMeshComponent;
}

// Called when the game starts or when spawned
void AMinoBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMinoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinoBase::SetVitibility(bool InNewVisibility)
{
	if (CubeMeshComponent)
	{
		CubeMeshComponent->SetVisibility(InNewVisibility);
	}
}

void AMinoBase::SetTetrominoType(ETetrominoType InTetrominoType)
{
	TetrominoType = InTetrominoType;
}

