#include "TNTestController.h"

#include "Tenetris/Module/MVC/View/TestView/TNTestView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TestModel/TNTestModel.h"

void FTNTestController::Init()
{
	NextFieldModelId = 0;

	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	TSharedPtr<ITNView> tnView = FTNMVCHolder::GetInstance().GetView();
	
	if (tnModel.IsValid() && tnView.IsValid())
	{
		tnModel->Init();
		tnView->Init();
		tnModel->GetUpdateFieldViewDelegate().BindSP(tnView.ToSharedRef(), &ITNView::UpdateFieldView);
	}
}

void FTNTestController::Tick(float deltaTime)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->Tick(deltaTime);
	}
}


void FTNTestController::CreateField(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	TSharedPtr<ITNView> tnView = holder.GetView();

	if (tnModel.IsValid())
	{
		tnModel->CreateFieldModel(NextFieldModelId, fieldContext, height, width);	
	}
	
	if (tnView.IsValid())
	{
		tnView->CreateFieldView(NextFieldModelId, fieldActor);
	}
	
	++NextFieldModelId;
}

void FTNTestController::StartPlay()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	TSharedPtr<ITNView> tnView = holder.GetView();
	
	if (tnModel.IsValid())
	{
		tnModel->StartPlay();
	}
	
	if (tnView.IsValid())
	{
		tnView->StartPlay();
	}
}

void FTNTestController::HandleControlInput(const E_TNControlType controlType)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->HandleControlInput(controlType);
	}
}

