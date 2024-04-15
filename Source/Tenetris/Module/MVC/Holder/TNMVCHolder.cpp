#include "TNMVCHolder.h"

void FTNMVCHolder::SetModel(TSharedPtr<ITNModel> tnModel)
{
	TNModel = tnModel;
}

void FTNMVCHolder::SetView(TSharedPtr<ITNView> tnView)
{
	TNView = tnView;
}

void FTNMVCHolder::SetController(TSharedPtr<ITNController> tnController)
{
	TNController = tnController;
}

TSharedPtr<ITNModel> FTNMVCHolder::GetModel()
{
	return TNModel;
}

TSharedPtr<ITNView> FTNMVCHolder::GetView()
{
	return TNView;
}

TSharedPtr<ITNController> FTNMVCHolder::GetController()
{
	return TNController;
}
