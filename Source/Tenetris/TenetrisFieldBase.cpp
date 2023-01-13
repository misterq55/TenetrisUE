// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisFieldBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "TetrominoCubeBase.h"

// Sets default values
ATenetrisFieldBase::ATenetrisFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	StaticMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	StaticMeshComponent->SetRelativeScale3D(FVector(5.f, 3.f, 0.f));
}

// Called when the game starts or when spawned
void ATenetrisFieldBase::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			AActor* TestActor = GetWorld()->SpawnActor(ATetrominoCubeBase::StaticClass());
			TestActor->SetActorLocation(FVector(90.f, j * 10.f, i * 10.f));
		}
	}
}

// Called every frame
void ATenetrisFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

