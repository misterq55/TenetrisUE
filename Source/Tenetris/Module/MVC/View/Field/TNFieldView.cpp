#include "TNFieldView.h"
#include "Tenetris/Actors/Field/TNFieldBase.h"
#include "Engine/World.h"

void FTNFieldView::SetFieldActor(ATNFieldBase* fieldActor)
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

void FTNFieldView::CreateFieldActor(TSubclassOf<ATNFieldBase> fieldActorClass, UWorld* world)
{
	if (!IsValid(world))
	{
		return;
	}

	FieldActor = world->SpawnActor<ATNFieldBase>(fieldActorClass);

	if (FieldActor.IsValid())
	{
		FieldActor->Initialize();
	}
}

void FTNFieldView::Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) const
{
	if (FieldActor.IsValid())
	{
		FieldActor->HandleFieldState(fieldContext, state);
	}
}
