// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrominoCubeBase.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ATetrominoCubeBase::ATetrominoCubeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	StaticMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	
	FSoftObjectPath DefaultTranslucentMaterialName(" Material'/Game/StarterContent/Materials/M_Ground_Grass.M_Ground_Grass'");
	UMaterialInterface* TranslucentMaterial = Cast<UMaterialInterface>(DefaultTranslucentMaterialName.TryLoad());
	StaticMeshComponent->SetMaterial(0, TranslucentMaterial);
}

// Called when the game starts or when spawned
void ATetrominoCubeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector::ZeroVector);
}

// Called every frame
void ATetrominoCubeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

