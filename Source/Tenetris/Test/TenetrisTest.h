// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/PlayerTenetrisField/PlayerTenetrisField.h"
#include "Tenetris/Field/Tetromino/TetrominoCube/TetrominoCubeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"
// #include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/PlayerTetromino/PlayerTetromino.h"
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

		if (!CubeMeshComponent)
			return;
		
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
};

class FTestTetromino : public FPlayerTetromino
{
public:
	FTestTetromino()
		: FPlayerTetromino()
	{
		
	}

	FTestTetromino(ATenetrisFieldBase* InCurrentTenetrisField)
		: FPlayerTetromino(InCurrentTenetrisField)
	{
		/*TetrominoCoordinate.Add(FVector2D(1.f, 0.f));
		TetrominoCoordinate.Add(FVector2D(1.f, 1.f));
		TetrominoCoordinate.Add(FVector2D(2.f, 0.f));
		TetrominoCoordinate.Add(FVector2D(2.f, 1.f));*/

		/*TetrominoInfo.TetrominoCoordinate.Add(FVector2D(0.f, 1.f));
		TetrominoInfo.TetrominoCoordinate.Add(FVector2D(0.f, 0.f));
		TetrominoInfo.TetrominoCoordinate.Add(FVector2D(1.f, 0.f));
		TetrominoInfo.TetrominoCoordinate.Add(FVector2D(2.f, 0.f));*/
	}
};

UCLASS()
class TENETRIS_API ATestTenetrisField : public APlayerTenetrisField
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestTenetrisField()
		: APlayerTenetrisField()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		SetTetrominoCubeClassType(ATestTetrominoCube::StaticClass());
		CurrentTetromino = new FTestTetromino(this);

		CurrentTetromino->SetStartingLocation(3, 20);
	}

	// friend class FTestTetromino;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() 
	{
		Super::BeginPlay();
	}

	virtual void Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

		/*static int32 TestValue = 0;
		if (TestValue % 2 == 0)
		{
			for (int i = 0; i < RowMax; i++)
			{
				for (int j = 0; j < ColumnMax; j++)
				{
					if (!(i == 0 || i == 19 ||
						j == 0 || j == 9))
						SetVisibilityBackgroundCube(j, i, false);

					if (j == 0)
						SetBackgroundCubeType(j, i, ETetrominoType::J);

					if (j == 9)
						SetBackgroundCubeType(j, i, ETetrominoType::Z);

					if (i == 0)
						SetBackgroundCubeType(j, i, ETetrominoType::I);

					if (i == 19)
						SetBackgroundCubeType(j, i, ETetrominoType::O);
				}
			}
		}
		else
		{
			for (int i = 0; i < RowMax; i++)
			{
				for (int j = 0; j < ColumnMax; j++)
				{
					SetBackgroundCubeType(j, i, ETetrominoType::Obstacle);
				}
			}
		}

		TestValue++;
		if (TestValue > 5)
			TestValue = 0;*/
	}

public:

	virtual void Initialize()
	{
		Super::Initialize();

		//for (int i = 0; i < RowMax; i++)
		//{
		//	for (int j = 0; j < ColumnMax; j++)
		//	{
		//		// SetBackgroundCubeType(j, i, ETetrominoType::Obstacle);

		//		/*if (j == 0)
		//			SetBackgroundCubeType(j, i, ETetrominoType::J);

		//		if (j == 7)
		//			SetBackgroundCubeType(j, i, ETetrominoType::Z);*/

		//		if (i == 2)
		//			SetBackgroundCubeType(j, i, ETetrominoType::Obstacle);

		//		/*if (i == 19)
		//			SetBackgroundCubeType(j, i, ETetrominoType::O);*/
		//	}
		//}

		SetBackgroundCubeType(5, 3, ETetrominoType::Obstacle);
		SetBackgroundCubeType(4, 3, ETetrominoType::Obstacle);
		SetBackgroundCubeType(5, 4, ETetrominoType::Obstacle);

		CurrentTetromino->SetTetrominoType(ETetrominoType::T);
		CurrentTetromino->Spawn();
	}
};