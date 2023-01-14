// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrominoCubeBase.h"

// Sets default values
ATetrominoCubeBase::ATetrominoCubeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATetrominoCubeBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATetrominoCubeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATetrominoCubeBase::SetVitibility(bool InNewVisibility)
{
	if (CubeMeshComponent)
	{
		CubeMeshComponent->SetVisibility(InNewVisibility);
	}
}

void ATetrominoCubeBase::SetTetrominoType(ETetrominoType InTetrominoType)
{
	TetrominoType = InTetrominoType;
}

void ATetrominoCubeBase::SetTetrominoCubePosition(int32 X, int32 Y)
{
}

