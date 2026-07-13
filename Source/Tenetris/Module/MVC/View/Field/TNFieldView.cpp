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

	case E_TNFieldModelStateType::HideHoldTetromino:
		{
			hideHoldTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::ShowHoldTetromino:
		{
			setHoldTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::HidePreviewTetromino:
		{
			hidePreviewTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::ShowPreviewTetromino:
		{
			setPreviewTetromino(fieldContext);
		}
		break;

	case E_TNFieldModelStateType::RotateField:
		{
			rotateField(fieldContext);
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

void FTNFieldView::hideHoldTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->HideHoldTetromino(fieldContext);
	}
}

void FTNFieldView::setHoldTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->ShowHoldTetromino(fieldContext);
	}
}

void FTNFieldView::hidePreviewTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->HidePreviewTetromino(fieldContext);
	}
}

void FTNFieldView::setPreviewTetromino(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->ShowPreviewTetromino(fieldContext);
	}
}

void FTNFieldView::rotateField(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->RotateField(fieldContext);
	}
}

void FTNFieldView::lockDown(const FTNFieldContext& fieldContext) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->LockDown(fieldContext);
	}
}
