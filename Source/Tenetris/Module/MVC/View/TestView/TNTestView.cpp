#include "TNTestView.h"
#include "Tenetris/Module/MVC/View/Field/TNFieldView.h"

void FTNTestView::Init()
{
	
}

void FTNTestView::Tick(float deltaTime)
{
}

void FTNTestView::StartPlay()
{
}

void FTNTestView::CreateFieldView(const int32 key)
{
	TSharedPtr<FTNFieldView> fieldView = MakeShareable(new FTNFieldView());
	if (fieldView.IsValid())
	{
		FieldViewMap.Emplace(key, fieldView);
	}
}

void FTNTestView::CreateFieldViewWithFieldActor(const int32 key, ATNFieldBase* fieldActor)
{
	TSharedPtr<FTNFieldView> fieldView = MakeShareable(new FTNFieldView());
	if (fieldView.IsValid())
	{
		fieldView->SetFieldActor(fieldActor);
		FieldViewMap.Emplace(key, fieldView);
	}
}

void FTNTestView::UpdateFieldView(const int32 modelKey, const FTNFieldContext& fieldContext,
	const E_TNFieldModelStateType state)
{
	TSharedPtr<FTNFieldView>* fieldView = FieldViewMap.Find(modelKey);
	if (fieldView && fieldView->IsValid())
	{
		(*fieldView)->Update(fieldContext, state);
	}
}

