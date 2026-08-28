// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TestLocalMultipleGameMode/TNTenetrisTestLocalMultipleGameMode.h"
#include "Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TestModel/TNTestModel.h"
#include "Tenetris/Module/MVC/View/TestView/TNTestView.h"
#include "Tenetris/Module/MVC/Controller/TestController/TNTestController.h"
#include "Tenetris/Actors/Field/TNFieldBase.h"

ATNTenetrisTestLocalMultipleGameMode::ATNTenetrisTestLocalMultipleGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ATNTenetrisTestLocalMultipleGameMode::StartPlay()
{
	Super::StartPlay();

	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	holder.SetModel(MakeShareable(new FTNTestModel()));
	holder.SetView(MakeShareable(new FTNTestView()));
	holder.SetController(MakeShareable(new FTNTestController()));

	TSharedPtr<ITNController> tnController = holder.GetController();
	if (tnController.IsValid())
	{
		tnController->Init();
	}

	if (tnController.IsValid())
	{
		FTNFieldContext fieldContext(E_TNFieldType::Player);
		tnController->CreateField(fieldContext, RowMax, ColumnMax, PlayerField);

		for (ATNFieldBase* field : Fields)
		{
			if (field == PlayerField)
			{
				continue;
			}
			FTNFieldContext enemyFieldContext(E_TNFieldType::Enemy);
			tnController->CreateField(enemyFieldContext, RowMax, ColumnMax, field);
		}
	}

	if (tnController.IsValid())
	{
		tnController->StartPlay();
	}
}

void ATNTenetrisTestLocalMultipleGameMode::Tick(float deltaSeconds)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	TSharedPtr<ITNController> tnController = holder.GetController();
	if (tnController.IsValid())
	{
		tnController->Tick(deltaSeconds);
	}
}

ATNFieldBase* ATNTenetrisTestLocalMultipleGameMode::spawnField(FVector location)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		return nullptr;
	}

	return world->SpawnActorDeferred<ATNFieldBase>(FieldClass, FTransform(location), this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
}
