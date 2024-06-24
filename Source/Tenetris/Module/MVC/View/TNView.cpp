#include "TNView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TNModel.h"
#include "Tenetris/Module/MVC/Model/Field/TNFieldModel.h"
#include "Tenetris/Module/MVC/View/Field/TNFieldView.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetrominoBase.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/PlayerTetromino/TNPlayerTetromino.h"

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

void FTNView::CreateFieldViewWithFieldActor(const int32 key, ATNField* fieldActor)
{
	TSharedPtr<FTNFieldView> fieldView = MakeShareable(new FTNFieldView());
	if (fieldView.IsValid())
	{
		fieldView->SetFieldActor(fieldActor);
		FieldViewMap.Emplace(key, fieldView);
	}
}

// TODO ���� �ƴ϶� ���ؽ�Ʈ�� ��ȯ�ϵ��� ����
// �Ʒ� ������ �ʵ� ��� �� �ű���
void FTNView::UpdateFieldView(const int32 modelKey, const E_TNFieldModelStateType state)
{
	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	if (!tnModel.IsValid())
	{
		return;
	}

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
		if (state == E_TNFieldModelStateType::LockDown)
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
		else
		{
			

			if (state == E_TNFieldModelStateType::HideTetromino)
			{
				TSharedPtr<FTNPlayerTetromino> currentTetromino = fieldModel->GetCurrentTetromino();
				if (!currentTetromino.IsValid())
				{
					return;
				}

				FTNTetrominoInfo& tetrominoInfo = currentTetromino->GetTetrominoInfo();

				for (const auto& coord : tetrominoInfo.Coordinate)
				{
					(*fieldView)->SetVisibilityMino(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, false);
				}
			}
			else if (state == E_TNFieldModelStateType::SetTetromino)
			{
				TSharedPtr<FTNPlayerTetromino> currentTetromino = fieldModel->GetCurrentTetromino();
				if (!currentTetromino.IsValid())
				{
					return;
				}

				FTNTetrominoInfo& tetrominoInfo = currentTetromino->GetTetrominoInfo();

				for (const auto& coord : tetrominoInfo.Coordinate)
				{
					(*fieldView)->SetMinoType(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, tetrominoInfo.CurrentType);
				}
			}
			else if (state == E_TNFieldModelStateType::HideGuideTetromino)
			{
				TSharedPtr<FTNPlayerTetromino> currentTetromino = fieldModel->GetCurrentTetromino();
				if (!currentTetromino.IsValid())
				{
					return;
				}

				FTNTetrominoInfo& tetrominoInfo = currentTetromino->GetTetrominoInfo();

				for (const auto& coord : tetrominoInfo.Coordinate)
				{
					(*fieldView)->SetVisibilityMino(coord.X + tetrominoInfo.GuideTetrominoPosition.X, coord.Y + tetrominoInfo.GuideTetrominoPosition.Y, false);
				}
			}
			else if (state == E_TNFieldModelStateType::SetGuideTetromino)
			{
				TSharedPtr<FTNPlayerTetromino> currentTetromino = fieldModel->GetCurrentTetromino();
				if (!currentTetromino.IsValid())
				{
					return;
				}

				FTNTetrominoInfo& tetrominoInfo = currentTetromino->GetTetrominoInfo();

				for (const auto& coord : tetrominoInfo.Coordinate)
				{
					(*fieldView)->SetMinoType(coord.X + tetrominoInfo.GuideTetrominoPosition.X, coord.Y + tetrominoInfo.GuideTetrominoPosition.Y, E_TNTetrominoType::Guide);
				}
			}
			else if (state == E_TNFieldModelStateType::HideHoldTetromino)
			{
				TSharedPtr<FTNTetrominoBase> holdTetromino = fieldModel->GetHoldTetromino();
				if (!holdTetromino.IsValid())
				{
					return;
				}

				FTNTetrominoInfo& tetrominoInfo = holdTetromino->GetTetrominoInfo();

				for (const auto& coord : tetrominoInfo.Coordinate)
				{
					(*fieldView)->SetVisibilityHoldMino(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, false);
				}
			}
			else if (state == E_TNFieldModelStateType::SetHoldTetromino)
			{
				TSharedPtr<FTNTetrominoBase> holdTetromino = fieldModel->GetHoldTetromino();
				if (!holdTetromino.IsValid())
				{
					return;
				}

				FTNTetrominoInfo& tetrominoInfo = holdTetromino->GetTetrominoInfo();

				for (const auto& coord : tetrominoInfo.Coordinate)
				{
					(*fieldView)->SetHoldMinoType(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, tetrominoInfo.CurrentType);
				}
			}
			else if (state == E_TNFieldModelStateType::HidePreviewTetromino)
			{
				TArray<TSharedPtr<FTNTetrominoBase>>& previewTetrominos = fieldModel->GetPreviewTetrominos();

				for (const auto& previewTetromino : previewTetrominos)
				{
					FTNTetrominoInfo& tetrominoInfo = previewTetromino->GetTetrominoInfo();

					for (const auto& coord : tetrominoInfo.Coordinate)
					{
						(*fieldView)->SetVisibilityPreviewMino(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, false);
					}
				}
			}
			else if (state == E_TNFieldModelStateType::SetPreviewTetromino)
			{
				TArray<TSharedPtr<FTNTetrominoBase>>& previewTetrominos = fieldModel->GetPreviewTetrominos();

				for (const auto& previewTetromino : previewTetrominos)
				{
					FTNTetrominoInfo& tetrominoInfo = previewTetromino->GetTetrominoInfo();

					for (const auto& coord : tetrominoInfo.Coordinate)
					{
						(*fieldView)->SetPreviewMinoType(coord.X + tetrominoInfo.CurrentPosition.X, coord.Y + tetrominoInfo.CurrentPosition.Y, tetrominoInfo.CurrentType);
					}
				}
			}
		}
	}
}
