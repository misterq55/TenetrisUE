// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisGameMode.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"
#include "Tenetris/Module/MVC/View/TNView.h"
#include "Tenetris/Module/MVC/Controller/TNController.h"

ATNTenetrisGameMode::ATNTenetrisGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ATNTenetrisGameMode::StartPlay()
{
	Super::StartPlay();

	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	holder.SetModel(MakeShareable(new FTNModel()));
	holder.SetView(MakeShareable(new FTNView()));
	holder.SetController(MakeShareable(new FTNController()));

	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	if (tnModel.IsValid())
	{
		tnModel->Init();
	}

	TSharedPtr<ITNView> tnView = holder.GetView();
	if (tnView.IsValid())
	{
		tnView->Init();
	}
	
	TSharedPtr<ITNController> tnController = holder.GetController();
	if (tnController.IsValid())
	{
		tnController->Init();
	}

	if (tnModel.IsValid())
	{
		for (const auto& fieldActor : Fields)
		{
			tnModel->CreateFieldModel(FTNFieldContext(E_TNFieldType::Player), fieldActor);
		}
	}
}

void ATNTenetrisGameMode::Tick(float deltaSeconds)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	TSharedPtr<ITNController> tnController = holder.GetController();
	if (tnController.IsValid())
	{
		tnController->Tick(deltaSeconds);
	}
}
