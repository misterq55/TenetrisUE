#include "TNFieldView.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Field/TNField.h"
#include "Engine/World.h"

void FTNFieldView::SetFieldActor(TObjectPtr<ATNField> fieldActor)
{
	if (!IsValid(fieldActor))
	{
		return;
	}

	FieldActor = fieldActor;

	if (IsValid(FieldActor))
	{
		FieldActor->Initialize();
	}
}

void FTNFieldView::CreateFieldActor(TSubclassOf<ATNField> fieldActorClass, UWorld* world)
{
	if (!IsValid(world))
	{
		return;
	}

	FieldActor = world->SpawnActor<ATNField>(fieldActorClass);

	if (IsValid(FieldActor))
	{
		FieldActor->Initialize();
	}
}

void FTNFieldView::SetBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetBackgroundCubeType(x, y, tetrominoType);
}

void FTNFieldView::SetMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetMinoType(x, y, tetrominoType);
}

void FTNFieldView::SetVisibilityMino(const int32 x, const int32 y, const bool visible)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetVisibilityMino(x, y, visible);
}

void FTNFieldView::SetHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetHoldMinoType(x, y, tetrominoType);
}

void FTNFieldView::SetVisibilityHoldMino(const int32 x, const int32 y, const bool visible)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetVisibilityHoldMino(x, y, visible);
}

void FTNFieldView::SetPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetPreviewMinoType(x, y, tetrominoType);
}

void FTNFieldView::SetVisibilityPreviewMino(const int32 x, const int32 y, const bool visible)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetVisibilityPreviewMino(x, y, visible);
}

void FTNFieldView::SetVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetVisibilityBackgroundCube(x, y, visible);
}

void FTNFieldView::Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state)
{
	switch (state)
	{
	case E_TNFieldModelStateType::LockDown:
	{
		const int32 bufferHeight = fieldContext.BufferHeight;
		const int32 bufferWidth = fieldContext.BufferWidth;

		for (int32 i = 0; i < bufferHeight; i++)
		{
			for (int32 j = 0; j < bufferWidth; j++)
			{
				const E_TNTetrominoType tetrominoType = fieldContext.CheckBuffer[i + 1][j + 1];
				SetBackgroundCubeType(j, i, tetrominoType);
			}
		}
	}
		break;

	case E_TNFieldModelStateType::HideTetromino:
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

		if (!tetrominoInfo.IsValid())
		{
			return;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			SetVisibilityMino(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, false);
		}
	}
		break;

	case E_TNFieldModelStateType::SetTetromino:
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

		if (!tetrominoInfo.IsValid())
		{
			return;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			SetMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
		}
	}
		break;

	case E_TNFieldModelStateType::HideGuideTetromino:
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

		if (!tetrominoInfo.IsValid())
		{
			return;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			SetVisibilityMino(coord.X + tetrominoInfo->GuideTetrominoPosition.X, coord.Y + tetrominoInfo->GuideTetrominoPosition.Y, false);
		}
	}
		break;

	case E_TNFieldModelStateType::SetGuideTetromino:
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

		if (!tetrominoInfo.IsValid())
		{
			return;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			SetMinoType(coord.X + tetrominoInfo->GuideTetrominoPosition.X, coord.Y + tetrominoInfo->GuideTetrominoPosition.Y, E_TNTetrominoType::Guide);
		}
	}
		break;

	case E_TNFieldModelStateType::HideHoldTetromino:
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.HoldTetrominoInfo;

		if (!tetrominoInfo.IsValid())
		{
			return;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			SetVisibilityHoldMino(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, false);
		}
	}
		break;

	case E_TNFieldModelStateType::SetHoldTetromino:
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.HoldTetrominoInfo;

		if (!tetrominoInfo.IsValid())
		{
			return;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			SetHoldMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
		}
	}
		break;

	case E_TNFieldModelStateType::HidePreviewTetromino:
	{
		TArray<TSharedPtr<FTNTetrominoInfo>> preveiwTetrominoInfos = fieldContext.PreviewTetrominoInfos;

		for (const auto& tetrominoInfo : preveiwTetrominoInfos)
		{
			if (!tetrominoInfo.IsValid())
			{
				continue;
			}

			for (const auto& coord : tetrominoInfo->Coordinate)
			{
				SetVisibilityPreviewMino(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, false);
			}
		}
	}
		break;

	case E_TNFieldModelStateType::SetPreviewTetromino:
	{
		TArray<TSharedPtr<FTNTetrominoInfo>> preveiwTetrominoInfos = fieldContext.PreviewTetrominoInfos;

		for (const auto& tetrominoInfo : preveiwTetrominoInfos)
		{
			if (!tetrominoInfo.IsValid())
			{
				continue;
			}

			for (const auto& coord : tetrominoInfo->Coordinate)
			{
				SetPreviewMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
			}
		}
	}
		break;
	default:
		break;
	}
}
