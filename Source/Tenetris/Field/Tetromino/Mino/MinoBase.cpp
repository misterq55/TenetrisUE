// Fill out your copyright notice in the Description page of Project Settings.


#include "MinoBase.h"

// Sets default values
AMinoBase::AMinoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMinoBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMinoBase::InitailizeMesh(UStaticMesh* StaticMesh)
{
	MinoMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CUBEMESH_COMPONENT"));
	MinoMeshComponent->SetupAttachment(RootComponent);
	MinoMeshComponent->SetStaticMesh(StaticMesh);
	MinoMeshComponent->SetRelativeScale3D(FVector(MinoRatio, MinoRatio, MinoRatio));

	MinoMeshComponent->SetVisibility(false);

	RootComponent = MinoMeshComponent;
}

// Called every frame
void AMinoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinoBase::SetVitibility(bool newVisibility)
{
	if (IsValid(MinoMeshComponent))
	{
		MinoMeshComponent->SetVisibility(newVisibility);
	}
}

void AMinoBase::SetTetrominoType(ETetrominoType tetrominoType)
{
	TetrominoType = tetrominoType;
}

