#include "TNTenetrisTestGameMode.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TestModel/TNTestModel.h"
#include "Tenetris/Module/MVC/View/TestView/TNTestView.h"
#include "Tenetris/Module/MVC/Controller/TestController/TNTestController.h"

ATNTenetrisTestGameMode::ATNTenetrisTestGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ATNTenetrisTestGameMode::StartPlay()
{
	Super::StartPlay();

	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	holder.SetModel(MakeShareable(new FTNTestModel()));
	holder.SetView(MakeShareable(new FTNTestView()));
	holder.SetController(MakeShareable(new FTNTestController()));

	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	
	TSharedPtr<ITNController> tnController = holder.GetController();
	if (tnController.IsValid())
	{
		tnController->Init();
	}

	if (tnModel.IsValid())
	{
		for (const auto& fieldActor : Fields)
		{
			FTNFieldContext fieldContext(E_TNFieldType::Player);
			tnModel->CreateFieldModel(fieldContext, RowMax, ColumnMax, fieldActor);
		}
	}
	
	if (tnController.IsValid())
	{
		tnController->StartPlay();
	}
}

void ATNTenetrisTestGameMode::Tick(float deltaSeconds)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	TSharedPtr<ITNController> tnController = holder.GetController();
	if (tnController.IsValid())
	{
		tnController->Tick(deltaSeconds);
	}
}
