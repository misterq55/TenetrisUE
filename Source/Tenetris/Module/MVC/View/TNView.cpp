#include "TNView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"
#include "Tenetris/Module/MVC/Model/Field/TNFieldModel.h"
#include "Tenetris/Module/MVC/View/Field/TNFieldView.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetrominoBase.h"

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

// TODO 모델이 아니라 컨텍스트를 반환하도록 하자
// 아래 로직은 필드 뷰로 다 옮기자
void FTNView::UpdateFieldView(const int32 modelKey, const E_TNFieldModelStateType state)
{
	if (state == E_TNFieldModelStateType::LockDown)
	{
		TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
		if (!tnModel.IsValid())
		{
			return;
		}

		tnModel->GetFieldModelMap();

		const auto modelMap = tnModel->GetFieldModelMap();
		const TSharedPtr<FTNFieldModel>* fieldModeltPtr = modelMap.Find(modelKey);
		if (!fieldModeltPtr)
		{
			return;
		}

		TSharedPtr<FTNFieldModel> fieldModel = *fieldModeltPtr;

		if (!fieldModel.IsValid())
		{
			return;
		}

		TSharedPtr<FTNFieldView>* fieldView = FieldViewMap.Find(modelKey);
		if (fieldView && fieldView->IsValid())
		{
			const FTNFieldContext& fieldContext = fieldModel->GetFieldContext();
			const int32 bufferHeight = fieldContext.BufferHeight;
			const int32 bufferWidth = fieldContext.BufferWidth;

			for (int32 i = 0; i < bufferHeight; i++)
			{
				for (int32 j = 0; j < bufferWidth; j++)
				{
					const E_TNTetrominoType tetrominoType = fieldContext.CheckBuffer[i + 1][j + 1];
					(*fieldView)->SetBackgroundCubeType(j, i, tetrominoType);
				}
			}
		}
	}

	else if (state == E_TNFieldModelStateType::HideTetromino)
	{
		TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
		if (!tnModel.IsValid())
		{
			return;
		}

		tnModel->GetFieldModelMap();

		const auto modelMap = tnModel->GetFieldModelMap();
		const TSharedPtr<FTNFieldModel>* fieldModeltPtr = modelMap.Find(modelKey);
		if (!fieldModeltPtr)
		{
			return;
		}

		TSharedPtr<FTNFieldModel> fieldModel = *fieldModeltPtr;

		if (!fieldModel.IsValid())
		{
			return;
		}

		TSharedPtr<FTNTetrominoBase> currentTetromino = fieldModel->GetCurrentTetromino();
		if (!currentTetromino.IsValid())
		{
			return;
		}

		FTNTetrominoInfo& tetrominoInfo = currentTetromino->GetTetrominoInfo();

		TSharedPtr<FTNFieldView>* fieldView = FieldViewMap.Find(modelKey);
		if (fieldView && fieldView->IsValid())
		{
			for (const auto& coord : tetrominoInfo.Coordinate)
			{
				(*fieldView)->SetVisibilityMino(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, false);
			}
		}
	}

	else if (state == E_TNFieldModelStateType::SetTetromino)
	{
		TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
		if (!tnModel.IsValid())
		{
			return;
		}

		tnModel->GetFieldModelMap();

		const auto modelMap = tnModel->GetFieldModelMap();
		const TSharedPtr<FTNFieldModel>* fieldModeltPtr = modelMap.Find(modelKey);
		if (!fieldModeltPtr)
		{
			return;
		}

		TSharedPtr<FTNFieldModel> fieldModel = *fieldModeltPtr;

		if (!fieldModel.IsValid())
		{
			return;
		}

		TSharedPtr<FTNTetrominoBase> currentTetromino = fieldModel->GetCurrentTetromino();
		if (!currentTetromino.IsValid())
		{
			return;
		}

		FTNTetrominoInfo& tetrominoInfo = currentTetromino->GetTetrominoInfo();

		TSharedPtr<FTNFieldView>* fieldView = FieldViewMap.Find(modelKey);
		if (fieldView && fieldView->IsValid())
		{
			for (const auto& coord : tetrominoInfo.Coordinate)
			{
				(*fieldView)->SetMinoType(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, tetrominoInfo.CurrentType);
			}
		}
	}
}
