// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisGameMode.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"
#include "Tenetris/Module/MVC/View/TNView.h"
#include "Tenetris/Module/MVC/Controller/TNController.h"

void ATNTenetrisGameMode::StartPlay()
{
	Super::StartPlay();

	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	holder.SetModel(MakeShareable(new FTNModel()));
	holder.SetView(MakeShareable(new FTNView()));
	holder.SetController(MakeShareable(new FTNController()));

	holder.GetModel();
	holder.GetView();
	holder.GetController();
}

void ATNTenetrisGameMode::Tick(float deltaSeconds)
{

}
