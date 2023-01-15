// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Tenetris/Field/TenetrisFieldBase.h"
#include "Tenetris/Field/PlayerTenetrisField/PlayerTenetrisField.h"
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
class TENETRIS_API ATestTenetrisField : public APlayerTenetrisField
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
		// StaticMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

		TetrominoCubeClass = ATestTetrominoCube::StaticClass();
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() 
	{
		Super::BeginPlay();
	}

	virtual void Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

		static int32 TestValue = 0;
		if (TestValue % 2 == 0)
		{
			for (int i = 0; i < RowMax; i++)
			{
				for (int j = 0; j < ColumnMax; j++)
				{
					ATestTetrominoCube* TestActor = Cast<ATestTetrominoCube>(CubeBuffer[i][j]);

					if (i == 0 || i == 19 ||
						j == 0 || j == 9)
						TestActor->SetVitibility(true);
					else
						TestActor->SetVitibility(false);

					if (j == 0)
						TestActor->SetTetrominoType(ETetrominoType::J);

					if (j == 9)
						TestActor->SetTetrominoType(ETetrominoType::Z);

					if (i == 0)
						TestActor->SetTetrominoType(ETetrominoType::I);

					if (i == 19)
						TestActor->SetTetrominoType(ETetrominoType::O);
				}
			}
		}
		else
		{
			for (int i = 0; i < RowMax; i++)
			{
				for (int j = 0; j < ColumnMax; j++)
				{
					ATestTetrominoCube* TestActor = Cast<ATestTetrominoCube>(CubeBuffer[i][j]);

					TestActor->SetTetrominoType(ETetrominoType::Obstacle);
					TestActor->SetVitibility(true);
				}
			}
		}

		TestValue++;
		if (TestValue > 5)
			TestValue = 0;
	}

public:

	virtual void Initialize()
	{
		Super::Initialize();

		for (int i = 0; i < RowMax; i++)
		{
			for (int j = 0; j < ColumnMax; j++)
			{
				ATestTetrominoCube* TestActor = Cast<ATestTetrominoCube>(CubeBuffer[i][j]);
				
				TestActor->SetTetrominoType(ETetrominoType::Obstacle);
				TestActor->SetVitibility(true);

				/*if (i == 0 || i == 19 ||
					j == 0 || j == 9)
					TestActor->SetVitibility(true);

				if (j == 0)
					TestActor->SetTetrominoType(ETetrominoType::J);

				if (j == 9)
					TestActor->SetTetrominoType(ETetrominoType::Z);

				if (i == 0)
					TestActor->SetTetrominoType(ETetrominoType::I);

				if (i == 19)
					TestActor->SetTetrominoType(ETetrominoType::O);*/
			}
		}
	}
};