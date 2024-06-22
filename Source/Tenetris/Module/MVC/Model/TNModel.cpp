#include "TNModel.h"
#include "Field/TNFieldModel.h"

void FTNModel::Init()
{
}

void FTNModel::Tick(float deltaTime)
{
	for (const auto& [key, fieldModel] : FieldModelMap)
	{
		if (!fieldModel.IsValid())
		{
			continue;
		}

		fieldModel->Tick(deltaTime);
	}
}

void FTNModel::CreateFieldModel(FTNFieldContext fieldContext)
{
	TSharedPtr<FTNFieldModel> fieldModel = MakeShareable(new FTNFieldModel(fieldContext));
	if (!fieldModel.IsValid())
	{
		return;
	}

	fieldModel->GetOnUpdateModelDelegate().BindRaw(this, &FTNModel::UpdateModel);
	fieldModel->Initialize();
	fieldModel->SetId(ModelKey);
	FieldModelMap.Emplace(ModelKey, fieldModel);

	if (fieldContext.FieldType == E_TNFieldType::Player)
	{
		PlayerFieldModel = fieldModel;
	}

	ModelKey++;
}

void FTNModel::CreateFieldModel(FTNFieldContext fieldContext, ATNFieldBase* fieldActor)
{
	TSharedPtr<FTNFieldModel> fieldModel = MakeShareable(new FTNFieldModel(fieldContext));
	if (!fieldModel.IsValid())
	{
		return;
	}

	fieldModel->GetOnUpdateModelDelegate().BindRaw(this, &FTNModel::UpdateModel);
	fieldModel->AddFieldActor(fieldActor); // TODO »èÁ¦
	fieldModel->SetId(ModelKey);
	
	FieldModelMap.Emplace(ModelKey, fieldModel);

	if (fieldContext.FieldType == E_TNFieldType::Player)
	{
		PlayerFieldModel = fieldModel;
	}

	CreateFieldViewWithFieldActorDelegate.ExecuteIfBound(ModelKey, fieldActor);

	fieldModel->Initialize();

	ModelKey++;
}

void FTNModel::UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state)
{
	UpdateFieldViewDelegate.ExecuteIfBound(modelKey, state);
}

TSharedPtr<FTNFieldModel> FTNModel::GetPlayerFieldModel()
{
	return PlayerFieldModel;
}

void FTNModel::StartMoveLeft()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->StartMoveLeft();
	}
}

void FTNModel::StopMoveLeft()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->StopMoveLeft();
	}
}

void FTNModel::StartMoveRight()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->StartMoveRight();
	}
}

void FTNModel::StopMoveRight()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->StopMoveRight();
	}
}

void FTNModel::StartSoftDrop()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->StartSoftDrop();
	}
}

void FTNModel::StopSoftDrop()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->StopSoftDrop();
	}
}

void FTNModel::RotateClockWise()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->RotateClockWise();
	}
}

void FTNModel::RotateCounterClockWise()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->RotateCounterClockWise();
	}
}

void FTNModel::Hold()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->Hold();
	}
}

void FTNModel::ToggleSpaceInversion()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->ToggleSpaceInversion();
	}
}

void FTNModel::HardDrop()
{
	if (PlayerFieldModel.IsValid())
	{
		PlayerFieldModel->HardDrop();
	}
}
