// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/TenetrisFieldBase.h"
#include "Tenetris/Field/Tetromino/TetrominoCube/TetrominoCubeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "TenetrisTest.generated.h"

UCLASS()
class TENETRIS_API ATestTetrominoCube : public ATetrominoCubeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestTetrominoCube()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
		CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
		CubeMeshComponent->SetupAttachment(RootComponent);
		CubeMeshComponent->SetStaticMesh(MeshObj.Object);
		CubeMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

		/*FSoftObjectPath DefaultTranslucentMaterialName("Material'/Game/TetrominoResources/Test/ObstacleMino.ObstacleMino'");
		UMaterialInterface* TranslucentMaterial = Cast<UMaterialInterface>(DefaultTranslucentMaterialName.TryLoad());
		CubeMeshComponent->SetMaterial(0, TranslucentMaterial);*/

		CubeMeshComponent->SetVisibility(false);
	}

	void SetTetrominoType(ETetrominoType InTetrominoType)
	{
		Super::SetTetrominoType(InTetrominoType);

		if (CubeMeshComponent)
		{
			FSoftObjectPath DefaultTranslucentMaterialName;
			switch (InTetrominoType)
			{
			case ETetrominoType::I:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/IMino.IMino'");
				break;

			case ETetrominoType::J:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/JMino.JMino'");
				break;

			case ETetrominoType::L:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/LMino.LMino'");
				break;

			case ETetrominoType::O:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/OMino.OMino'");
				break;

			case ETetrominoType::S:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/SMino.SMino'");
				break;

			case ETetrominoType::T:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/TMino.TMino'");
				break;

			case ETetrominoType::Z:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/ZMino.ZMino'");
				break;

			case ETetrominoType::Obstacle:
				DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/ObstacleMino.ObstacleMino'");
				break;

			default:
				break;
			}

			UMaterialInterface* TranslucentMaterial = Cast<UMaterialInterface>(DefaultTranslucentMaterialName.TryLoad());
			CubeMeshComponent->SetMaterial(0, TranslucentMaterial);
		}
	}
};

UCLASS()
class TENETRIS_API ATestTenetrisField : public ATenetrisFieldBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestTenetrisField()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
		UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
		StaticMeshComponent->SetupAttachment(RootComponent);
		StaticMeshComponent->SetStaticMesh(MeshObj.Object);
		StaticMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() {}

public:

	virtual void Initialize()
	{
		FVector FieldLocation = GetActorLocation();
		FieldLocation.Y -= 112.5f;
		FieldLocation.Z -= 250.f;

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
				ChildComponent->SetChildActorClass(TSubclassOf< AActor >(ATestTetrominoCube::StaticClass()));
				ChildComponent->SetupAttachment(RootComponent);
				ChildComponent->CreateChildActor();
				// ATestTetrominoCube* TestActor = Cast<ATestTetrominoCube>(GetWorld()->SpawnActor(ATestTetrominoCube::StaticClass()));
				ATestTetrominoCube* TestActor = Cast<ATestTetrominoCube>(ChildComponent->GetChildActor());
				TestActor->SetActorLocation(FVector(90.f, j * 25.f + FieldLocation.Y, i * 25.f + FieldLocation.Z));
				if (i == 0 || i == 19 ||
					j == 0 || j == 9)
					TestActor->SetVitibility(true);

				if (j == 0)
					TestActor->SetTetrominoType(ETetrominoType::J);

				if (j == 9)
					TestActor->SetTetrominoType(ETetrominoType::Z);

				if (i == 0)
					TestActor->SetTetrominoType(ETetrominoType::I);

				if (i == 19)
					TestActor->SetTetrominoType(ETetrominoType::O);

				//UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>();
				//// ChildComponent->
				//FActorParentComponentSetter::Set(TestActor, RootComponent);
				//ChildComponent->SetupAttachment(RootComponent);

				// TestActor->GetRootComponent()->SetupAttachment(RootComponent);;
			}
		}
	}
};