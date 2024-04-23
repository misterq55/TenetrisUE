#include "TNController.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"

void FTNController::Init()
{
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
