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

void FTNController::StartMoveLeft()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->StartMoveLeft();
	}
}

void FTNController::StopMoveLeft()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->StopMoveLeft();
	}
}

void FTNController::StartMoveRight()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->StartMoveRight();
	}
}

void FTNController::StopMoveRight()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->StopMoveRight();
	}
}

void FTNController::StartSoftDrop()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->StartSoftDrop();
	}
}

void FTNController::StopSoftDrop()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->StopSoftDrop();
	}
}

void FTNController::RotateClockWise()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->RotateClockWise();
	}
}

void FTNController::RotateCounterClockWise()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->RotateCounterClockWise();
	}
}

void FTNController::Hold()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->Hold();
	}
}

void FTNController::ToggleSpaceInversion()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->ToggleSpaceInversion();
	}
}

void FTNController::HardDrop()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->HardDrop();
	}
}
