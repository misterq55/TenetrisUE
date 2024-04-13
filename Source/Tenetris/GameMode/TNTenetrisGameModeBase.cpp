// Copyright Epic Games, Inc. All Rights Reserved.


#include "TNTenetrisGameModeBase.h"
#include "Tenetris/TNTenetrisPawn.h"
#include "Tenetris/Field/TNFieldBase.h"
#include "Tenetris/PlayerController/TNTenetrisPlayerController.h"

ATNTenetrisGameModeBase::ATNTenetrisGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATNTenetrisPawn::StaticClass();
	PlayerControllerClass = ATNTenetrisPlayerController::StaticClass();
}

void ATNTenetrisGameModeBase::Initialize()
{
	for (ATNFieldBase* Field : Fields)
	{
		if (!IsValid(Field))
		{
			continue;
		}

		Field->Initialize();
	}
}

void ATNTenetrisGameModeBase::AddFields(ATNFieldBase* field)
{
	Fields.Add(field);
}

void ATNTenetrisGameModeBase::SetPlayerField(ATNFieldBase* playerField)
{
	AddFields(playerField);
	PlayerField = playerField;
}

ATNFieldBase* ATNTenetrisGameModeBase::GetPlayerField()
{
	return PlayerField;
}