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
		if (IsValid(PlayerFieldPositionActor))
		{
			PlayerField = spawnField(PlayerFieldPositionActor->GetActorLocation(), FVector(1.f, 1.f, 1.f));
			FTNFieldContext fieldContext(E_TNFieldType::Player);
			tnController->CreateField(fieldContext, RowMax, ColumnMax, PlayerField);
		}

		for (AActor* fieldsPositionActor : FieldsPositionActors)
		{
			if (!IsValid(fieldsPositionActor))
			{
				continue;
			}

			ATNFieldBase* field = spawnField(fieldsPositionActor->GetActorLocation(), FVector(0.5f, 0.5f, 0.5f));
			if (!IsValid(field))
			{
				continue;
			}

			field->ApplyHoldVisible(false);
			field->ApplyPreviewVisible(false);
			
			Fields.Add(field);
		}
		
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

ATNFieldBase* ATNTenetrisTestLocalMultipleGameMode::spawnField(const FVector& location, const FVector& scale)
{
	UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		return nullptr;
	}
	
	ATNFieldBase* field = world->SpawnActorDeferred<ATNFieldBase>(FieldClass, FTransform(location), this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	field->SetActorScale3D(scale);
	
	return field;
}
