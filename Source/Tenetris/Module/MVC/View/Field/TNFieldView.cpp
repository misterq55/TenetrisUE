#include "TNFieldView.h"
#include "Tenetris/Actors/Field/TNField.h"
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
	if (FieldActor.IsValid())
	{
		FieldActor->HandleFieldState(fieldContext, state);
	}
}
