#include "TNFieldView.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Field/TNFieldBase.h"
#include "Engine/World.h"

void FTNFieldView::SetFieldActor(TObjectPtr<ATNFieldBase> fieldActor)
{
	if (!IsValid(fieldActor))
	{
		return;
	}

	FieldActor = fieldActor;
}

void FTNFieldView::CreateFieldActor(TSubclassOf<ATNFieldBase> fieldActorClass, UWorld* world)
{
	if (!IsValid(world))
	{
		return;
	}

	FieldActor = world->SpawnActor<ATNFieldBase>(fieldActorClass);
}

void FTNFieldView::SetBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	if (!IsValid(FieldActor))
	{
		return;
	}

	FieldActor->SetBackgroundCubeType(x, y, tetrominoType);
}
