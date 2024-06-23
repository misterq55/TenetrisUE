#include "TNFieldView.h"
// #include "Tenetris/Module/MVC/View/Field/Actor/Field/TNFieldBase.h"
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
