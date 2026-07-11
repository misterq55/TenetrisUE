#include "TNFieldView.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Field/TNField.h"
#include "Engine/World.h"

void FTNFieldView::SetFieldActor(ATNField* fieldActor)
{
	if (!IsValid(fieldActor))
	{
		return;
	}

	FieldActor = fieldActor;

	if (FieldActor.IsValid())
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

	if (FieldActor.IsValid())
	{
		FieldActor->Initialize();
	}
}

void FTNFieldView::setBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetBackgroundCubeType(x, y, tetrominoType);
}

void FTNFieldView::setMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetMinoType(x, y, tetrominoType);
}

void FTNFieldView::setVisibilityMino(const int32 x, const int32 y, const bool visible) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetVisibilityMino(x, y, visible);
}

void FTNFieldView::setHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetHoldMinoType(x, y, tetrominoType);
}

void FTNFieldView::setVisibilityHoldMino(const int32 x, const int32 y, const bool visible) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetVisibilityHoldMino(x, y, visible);
}

void FTNFieldView::setPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetPreviewMinoType(x, y, tetrominoType);
}

void FTNFieldView::setVisibilityPreviewMino(const int32 x, const int32 y, const bool visible) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetVisibilityPreviewMino(x, y, visible);
}

void FTNFieldView::setVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible) const
{
	if (!FieldActor.IsValid())
	{
		return;
	}

	FieldActor->SetVisibilityBackgroundCube(x, y, visible);
}

void FTNFieldView::Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) const
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
							setBackgroundCubeType(j, i, tetrominoType);
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
			setVisibilityMino(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, false);
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
			setMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
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
			setVisibilityMino(coord.X + tetrominoInfo->GuideTetrominoPosition.X, coord.Y + tetrominoInfo->GuideTetrominoPosition.Y, false);
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
			setMinoType(coord.X + tetrominoInfo->GuideTetrominoPosition.X, coord.Y + tetrominoInfo->GuideTetrominoPosition.Y, E_TNTetrominoType::Guide);
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
			setVisibilityHoldMino(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, false);
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
			setHoldMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
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
				setVisibilityPreviewMino(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, false);
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
				setPreviewMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
			}
		}
	}
		break;
		
	case E_TNFieldModelStateType::RotateField:
		{
			if (FieldActor.IsValid())
			{
				FieldActor->RotateField(fieldContext.bSpaceInverted);
			}
		}
		break;
	default:
		break;
	}
}
