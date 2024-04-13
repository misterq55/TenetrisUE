#include "TNFieldView.h"
#include "../../../../Field/TNFieldBase.h"
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
