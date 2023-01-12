// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisFieldBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ATenetrisFieldBase::ATenetrisFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
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

