#include "TNController.h"

#include "Module/MVC/View/TNView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"

void FTNController::Init()
{
	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	TSharedPtr<ITNView> tnView = FTNMVCHolder::GetInstance().GetView();
	
	if (tnModel.IsValid() && tnView.IsValid())
	{
		tnModel->GetCreateFieldViewDelegate().BindSP(tnView.ToSharedRef(), &ITNView::CreateFieldView);
		tnModel->GetCreateFieldViewWithFieldActorDelegate().BindSP(tnView.ToSharedRef(), &ITNView::CreateFieldViewWithFieldActor);
		tnModel->GetUpdateFieldViewDelegate().BindSP(tnView.ToSharedRef(), &ITNView::UpdateFieldView);
	}
}

void FTNController::Tick(float deltaTime)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->Tick(deltaTime);
	}
}

void FTNController::HandleControlInput(E_TNControlType controlType)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->HandleControlInput(controlType);
	}
}
