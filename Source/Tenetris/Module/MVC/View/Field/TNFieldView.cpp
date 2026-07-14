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

void FTNFieldView::Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) const
{
	switch (state)
	{
	case E_TNFieldModelStateType::LockDown:
		{
			lockDown(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::HideTetromino:
		{
			hideTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::ShowTetromino:
		{
			showTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::HideGuideTetromino:
		{
			hideGuideTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::ShowGuideTetromino:
		{
			setGuideTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::RotateField:
		{
			rotateField(fieldContext);
		}
		break;
		
	case E_TNFieldModelStateType::UpdateHoldTetromino:
		{
			updateHoldTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::UpdatePreviewTetrominoes:
		{
			updatePreviewTetrominoes(fieldContext);
		}
		break;
	default:
		break;
	}
}

void FTNFieldView::hideTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->HideTetromino(fieldContext);
	}
}

void FTNFieldView::showTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->ShowTetromino(fieldContext);
	}
}

void FTNFieldView::hideGuideTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->HideGuideTetromino(fieldContext);
	}
}

void FTNFieldView::setGuideTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->ShowGuideTetromino(fieldContext);
	}
}

void FTNFieldView::updateHoldTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->UpdateHoldTetromino(fieldContext);
	}
}

void FTNFieldView::rotateField(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->RotateField(fieldContext);
	}
}

void FTNFieldView::updatePreviewTetrominoes(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->UpdatePreviewTetrominoes(fieldContext);
	}
}

void FTNFieldView::lockDown(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->LockDown(fieldContext);
	}
}
