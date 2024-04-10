// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tenetris/Field/PlayerField/TNPlayerField.h"
#include "Tenetris/Field/Tetromino/Mino/TNMinoBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Tenetris/Components/TenetrisBufferComponent/TNTenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/PlayerTetromino/TNPlayerTetromino.h"
#include "Tenetris/Field/Tetromino/PreviewTetromino/TNPreviewTetromino.h"
#include "Tenetris/Field/TetrominoGenerator/TNTetrominoGenerator.h"
#include "TenetrisTest.generated.h"

UCLASS()
class TENETRIS_API ATNTestMino : public ATNMinoBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATNTestMino()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
		
		InitailizeMesh(MeshObj.Object);
	}

	void SetTetrominoType(E_TNTetrominoType tetrominoType)
	{
		Super::SetTetrominoType(tetrominoType);

		if (!MinoMeshComponent)
			return;
		
		FSoftObjectPath DefaultTranslucentMaterialName;
		switch (tetrominoType)
		{
		case E_TNTetrominoType::I:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/IMino.IMino'");
			break;

		case E_TNTetrominoType::J:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/JMino.JMino'");
			break;

		case E_TNTetrominoType::L:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/LMino.LMino'");
			break;

		case E_TNTetrominoType::O:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/OMino.OMino'");
			break;

		case E_TNTetrominoType::S:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/SMino.SMino'");
			break;

		case E_TNTetrominoType::T:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/TMino.TMino'");
			break;

		case E_TNTetrominoType::Z:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/ZMino.ZMino'");
			break;

		case E_TNTetrominoType::Guide:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/Guide.Guide'");
			break;

		case E_TNTetrominoType::Obstacle:
			DefaultTranslucentMaterialName = FSoftObjectPath("Material'/Game/TetrominoResources/Test/ObstacleMino.ObstacleMino'");
			break;

		default:
			break;
		}

		UMaterialInterface* TranslucentMaterial = Cast<UMaterialInterface>(DefaultTranslucentMaterialName.TryLoad());
		MinoMeshComponent->SetMaterial(0, TranslucentMaterial);
	}
};

class FTNTestTetromino : public FTNPlayerTetromino
{
public:
	FTNTestTetromino()
		: FTNPlayerTetromino()
	{
		
	}
};

UCLASS()
class TENETRIS_API ATNTestField : public ATNPlayerField
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATNTestField()
		: ATNPlayerField()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		SetMinoClassType(ATNTestMino::StaticClass());
		CurrentTetromino = new FTNTestTetromino();

		BindTetrominoToBuffer(CurrentTetromino, TenetrisBufferComponent);
		CurrentTetromino->SetStartingLocation(4, 18);

		PreviewBufferComponent->SetMinoClassType(ATNTestMino::StaticClass());
		HoldBufferComponent->SetMinoClassType(ATNTestMino::StaticClass());
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
						setBackgroundCubeType(j, i, E_TNTetrominoType::J);

					if (j == 9)
						setBackgroundCubeType(j, i, E_TNTetrominoType::Z);

					if (i == 0)
						setBackgroundCubeType(j, i, E_TNTetrominoType::I);

					if (i == 19)
						setBackgroundCubeType(j, i, E_TNTetrominoType::O);
				}
			}
		}
		else
		{
			for (int i = 0; i < RowMax; i++)
			{
				for (int j = 0; j < ColumnMax; j++)
				{
					setBackgroundCubeType(j, i, E_TNTetrominoType::Obstacle);
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

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < ColumnMax; j++)
			{
				if (j == 5)
					continue;
				
				if (i % 2 == 1 && j == 3)
					continue;

				setBackgroundCubeType(j, i, E_TNTetrominoType::Obstacle);
			}
		}

		//for (int i = 0; i < RowMax; i++)
		//{
		//	for (int j = 0; j < ColumnMax; j++)
		//	{
		//		// setBackgroundCubeType(j, i, E_TNTetrominoType::Obstacle);

		//		/*if (j == 0)
		//			setBackgroundCubeType(j, i, E_TNTetrominoType::J);

		//		if (j == 7)
		//			setBackgroundCubeType(j, i, E_TNTetrominoType::Z);*/

		//		if (i == 2)
		//			setBackgroundCubeType(j, i, E_TNTetrominoType::Obstacle);

		//		/*if (i == 19)
		//			setBackgroundCubeType(j, i, E_TNTetrominoType::O);*/
		//	}
		//}

		/*setBackgroundCubeType(5, 3, E_TNTetrominoType::Obstacle);
		setBackgroundCubeType(4, 3, E_TNTetrominoType::Obstacle);
		setBackgroundCubeType(5, 4, E_TNTetrominoType::Obstacle);*/

		Spawn();
	}
};