#include "TNView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"
#include "Tenetris/Module/MVC/Model/Field/TNFieldModel.h"
#include "Tenetris/Module/MVC/View/Field/TNFieldView.h"

void FTNView::Init()
{
	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	if (tnModel.IsValid())
	{
		tnModel->GetCreateFieldViewWithFieldActorDelegate().BindRaw(this, &FTNView::CreateFieldViewWithFieldActor);
		tnModel->GetUpdateFieldViewDelegate().BindRaw(this, &FTNView::UpdateFieldView);
	}
}

void FTNView::Tick(float deltaTime)
{
}

void FTNView::CreateFieldView(const int32 key)
{
	TSharedPtr<FTNFieldView> fieldView = MakeShareable(new FTNFieldView());
	if (fieldView.IsValid())
	{
		FieldViewMap.Emplace(key, fieldView);
	}
}

void FTNView::CreateFieldViewWithFieldActor(const int32 key, ATNFieldBase* fieldActor)
{
	TSharedPtr<FTNFieldView> fieldView = MakeShareable(new FTNFieldView());
	if (fieldView.IsValid())
	{
		fieldView->SetFieldActor(fieldActor);
		FieldViewMap.Emplace(key, fieldView);
	}
}

void FTNView::UpdateFieldView()
{
	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	if (!tnModel.IsValid())
	{
		return;
	}

	tnModel->GetFieldModelMap();

	for (const auto& [key, fieldModel] : tnModel->GetFieldModelMap())
	{
		if (!fieldModel.IsValid())
		{
			continue;
		}

		TSharedPtr<FTNFieldView>* fieldView = FieldViewMap.Find(key);
		if (fieldView && fieldView->IsValid())
		{
			const FTNFieldInfo& fieldInfo = fieldModel->GetFieldInfo();
			const int32 bufferHeight = fieldInfo.BufferHeight;
			const int32 bufferWidth = fieldInfo.BufferWidth;

			for (int32 i = 0; i < bufferHeight; i++)
			{
				for (int32 j = 0; j < bufferWidth; j++)
				{
					const E_TNTetrominoType tetrominoType = fieldInfo.CheckBuffer[i][j];
					(*fieldView)->SetBackgroundCubeType(j, i, tetrominoType);
				}
			}
		}
	}
}
