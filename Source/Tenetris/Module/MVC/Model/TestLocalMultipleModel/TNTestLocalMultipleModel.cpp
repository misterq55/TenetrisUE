#include "TNTestLocalMultipleModel.h"
#include "Tenetris/Module/MVC/Model/Field/TNFieldModel.h"

void FTNTestLocalMultipleModel::Init()
{
}

void FTNTestLocalMultipleModel::Tick(float deltaTime)
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

void FTNTestLocalMultipleModel::StartPlay()
{
	for (const auto& [key, fieldModel] : FieldModelMap)
	{
		if (!fieldModel.IsValid())
		{
			continue;
		}

		fieldModel->StartPlay();
	}
}

void FTNTestLocalMultipleModel::CreateFieldModel(const int32 modelKey, FTNFieldContext fieldContext, int32 height, int32 width)
{
	TSharedPtr<FTNFieldModel> fieldModel = MakeShareable(new FTNFieldModel(fieldContext, height, width));
	if (!fieldModel.IsValid())
	{
		return;
	}

	fieldModel->GetOnUpdateModelDelegate().BindRaw(this, &FTNTestLocalMultipleModel::UpdateModel);
	fieldModel->SetId(modelKey);

	FieldModelMap.Emplace(modelKey, fieldModel);

	if (fieldContext.FieldType == E_TNFieldType::Player)
	{
		PlayerFieldModel = fieldModel;
	}

	fieldModel->Initialize();

	// Write the initial confirmed state into the provider once so that every BoardId is immediately queryable.
	writeFieldStateToProvider(modelKey);
}

void FTNTestLocalMultipleModel::UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state)
{
	// Only write the latest confirmed buffer into the provider on states that change
	// confirmed (locked) cells. (Display-only updates such as tetromino move/preview
	// do not change the confirmed state, so they are not written.)
	switch (state)
	{
	case E_TNFieldModelStateType::LockDown:
	case E_TNFieldModelStateType::ReverseLockDown:
	case E_TNFieldModelStateType::LineClear:
	case E_TNFieldModelStateType::ReverseLineClear:
		writeFieldStateToProvider(modelKey);
		break;
	default:
		break;
	}

	FTNFieldContext fieldContext = GetFieldContext(modelKey);
	UpdateFieldViewDelegate.ExecuteIfBound(modelKey, fieldContext, state);
}

FTNFieldContext FTNTestLocalMultipleModel::GetFieldContext(const int32 modelKey)
{
	const TSharedPtr<FTNFieldModel>* fieldModeltPtr = FieldModelMap.Find(modelKey);
	if (fieldModeltPtr && fieldModeltPtr->IsValid())
	{
		return (*fieldModeltPtr)->GetFieldContext();
	}

	return FTNFieldContext(E_TNFieldType::None);
}

TSharedPtr<FTNFieldModel> FTNTestLocalMultipleModel::GetPlayerFieldModel()
{
	return PlayerFieldModel;
}

void FTNTestLocalMultipleModel::HandleControlInput(const E_TNControlType controlType)
{
	if (!PlayerFieldModel.IsValid())
	{
		return;
	}

	PlayerFieldModel->HandleControlInput(controlType);
}

void FTNTestLocalMultipleModel::writeFieldStateToProvider(const int32 modelKey)
{
	const TSharedPtr<FTNFieldModel>* fieldModelPtr = FieldModelMap.Find(modelKey);
	if (fieldModelPtr && fieldModelPtr->IsValid())
	{
		FieldStateProvider.WriteFieldState(modelKey, (*fieldModelPtr)->GetLatestConfirmedBuffer());
	}
}


