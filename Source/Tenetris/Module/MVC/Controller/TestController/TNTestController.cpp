#include "TNTestController.h"

#include "Tenetris/Module/MVC/View/TestView/TNTestView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TestModel/TNTestModel.h"

void FTNTestController::Init()
{
	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	TSharedPtr<ITNView> tnView = FTNMVCHolder::GetInstance().GetView();
	
	if (tnModel.IsValid() && tnView.IsValid())
	{
		tnModel->Init();
		tnView->Init();
		tnModel->GetCreateFieldViewDelegate().BindSP(tnView.ToSharedRef(), &ITNView::CreateFieldView);
		tnModel->GetCreateFieldViewWithFieldActorDelegate().BindSP(tnView.ToSharedRef(), &ITNView::CreateFieldViewWithFieldActor);
		tnModel->GetStartPlayDelegate().BindSP(tnView.ToSharedRef(), &ITNView::StartPlay);
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

void FTNTestController::StartPlay()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	
	if (tnModel.IsValid())
	{
		tnModel->StartPlay();
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

