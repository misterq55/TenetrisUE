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

void FTNModel::CreateFieldModel(FTNFieldContext fieldContext, int32 height, int32 width)
{
	TSharedPtr<FTNFieldModel> fieldModel = MakeShareable(new FTNFieldModel(fieldContext, height, width));
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
	
	CreateFieldViewDelegate.ExecuteIfBound(ModelKey);

	ModelKey++;
}

void FTNModel::CreateFieldModel(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor)
{
	TSharedPtr<FTNFieldModel> fieldModel = MakeShareable(new FTNFieldModel(fieldContext, height, width));
	if (!fieldModel.IsValid())
	{
		return;
	}

	fieldModel->GetOnUpdateModelDelegate().BindRaw(this, &FTNModel::UpdateModel);
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
	FTNFieldContext fieldContext = GetFieldContext(modelKey);
	UpdateFieldViewDelegate.ExecuteIfBound(modelKey, fieldContext, state);
}

FTNFieldContext FTNModel::GetFieldContext(const int32 modelKey)
{
	const TSharedPtr<FTNFieldModel>* fieldModeltPtr = FieldModelMap.Find(modelKey);
	if (fieldModeltPtr && fieldModeltPtr->IsValid())
	{
		return (*fieldModeltPtr)->GetFieldContext();
	}

	return FTNFieldContext(E_TNFieldType::None);
}

TSharedPtr<FTNFieldModel> FTNModel::GetPlayerFieldModel()
{
	return PlayerFieldModel;
}

void FTNModel::HandleControlInput(E_TNControlType controlType)
{
	if (!PlayerFieldModel.IsValid())
	{
		return;
	}
	
	PlayerFieldModel->HandleControlInput(controlType);
}
