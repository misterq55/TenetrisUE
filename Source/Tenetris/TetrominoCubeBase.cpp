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
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	CubeMeshComponent->SetupAttachment(RootComponent);
	CubeMeshComponent->SetStaticMesh(MeshObj.Object);
	CubeMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	
	FSoftObjectPath DefaultTranslucentMaterialName(" Material'/Game/StarterContent/Materials/M_Ground_Grass.M_Ground_Grass'");
	UMaterialInterface* TranslucentMaterial = Cast<UMaterialInterface>(DefaultTranslucentMaterialName.TryLoad());
	CubeMeshComponent->SetMaterial(0, TranslucentMaterial);

	CubeMeshComponent->SetVisibility(false);
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

void ATetrominoCubeBase::SetVitibility(bool InNewVisibility)
{
	if (CubeMeshComponent)
	{
		CubeMeshComponent->SetVisibility(InNewVisibility);
	}
}

