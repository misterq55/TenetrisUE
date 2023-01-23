// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/PlayerField/PlayerField.h"
#include "Tenetris/Field/Tetromino/Mino/MinoBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Tenetris/Components/TenetrisBufferComponent/TenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/PlayerTetromino/PlayerTetromino.h"
#include "Tenetris/Field/Tetromino/PreviewTetromino/PreviewTetromino.h"
#include "Tenetris/Field/TetrominoGenerator/TetrominoGenerator.h"
#include "TenetrisTest.generated.h"

UCLASS()
class TENETRIS_API ATestMino : public AMinoBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestMino()
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

		case ETetrominoType::Guide:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/Guide.Guide'");
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
};

UCLASS()
class TENETRIS_API ATestField : public APlayerField
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestField()
		: APlayerField()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		SetMinoClassType(ATestMino::StaticClass());
		CurrentTetromino = new FTestTetromino();

		BindTetrominoToBuffer(CurrentTetromino, TenetrisBufferComponent);
		CurrentTetromino->SetStartingLocation(4, 18);

		PreviewBufferComponent->SetMinoClassType(ATestMino::StaticClass());
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

		Spawn();
	}
};