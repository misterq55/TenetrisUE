// Fill out your copyright notice in the Description page of Project Settings.


#include "TNMinoBase.h"

// Sets default values
ATNMinoBase::ATNMinoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATNMinoBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATNMinoBase::InitailizeMesh(UStaticMesh* StaticMesh)
{
	MinoMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CUBEMESH_COMPONENT"));
	MinoMeshComponent->SetupAttachment(RootComponent);
	MinoMeshComponent->SetStaticMesh(StaticMesh);
	MinoMeshComponent->SetRelativeScale3D(FVector(MinoRatio, MinoRatio, MinoRatio));

	MinoMeshComponent->SetVisibility(false);

	RootComponent = MinoMeshComponent;
}

// Called every frame
void ATNMinoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATNMinoBase::SetVitibility(bool newVisibility)
{
	if (IsValid(MinoMeshComponent))
	{
		MinoMeshComponent->SetVisibility(newVisibility);
	}
}

void ATNMinoBase::SetTetrominoType(E_TNTetrominoType tetrominoType)
{
	TetrominoType = tetrominoType;
}

