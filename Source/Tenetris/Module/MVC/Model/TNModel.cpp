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

void FTNModel::CreateFieldModel()
{
	FieldModelMap.Emplace(ModelKey++, MakeShareable(new FTNFieldModel()));
}

void FTNModel::CreateFieldModel(ATNFieldBase* fieldActor)
{
	TSharedPtr<FTNFieldModel> fieldModel = MakeShareable(new FTNFieldModel());
	fieldModel->SetBufferSize(RowMax, ColumnMax);
	FieldModelMap.Emplace(ModelKey++, fieldModel);
}
