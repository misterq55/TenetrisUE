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
}

void FTNController::StopMoveLeft()
{
}

void FTNController::StartMoveRight()
{
}

void FTNController::StopMoveRight()
{
}

void FTNController::StartSoftDrop()
{
}

void FTNController::StopSoftDrop()
{
}

void FTNController::RotateClockWise()
{
}

void FTNController::RotateCounterClockWise()
{
}

void FTNController::Hold()
{
}

void FTNController::ToggleSpaceInversion()
{
}

void FTNController::HardDrop()
{
}
