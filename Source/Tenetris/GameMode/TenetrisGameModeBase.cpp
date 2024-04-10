// Copyright Epic Games, Inc. All Rights Reserved.


#include "TenetrisGameModeBase.h"
#include "Tenetris/TenetrisPawn.h"
#include "Tenetris/Field/FieldBase.h"
#include "Tenetris/PlayerController/TenetrisPlayerController.h"

ATenetrisGameModeBase::ATenetrisGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATenetrisPawn::StaticClass();
	PlayerControllerClass = ATenetrisPlayerController::StaticClass();
}

void ATenetrisGameModeBase::Initialize()
{
	for (AFieldBase* Field : Fields)
	{
		Field->Initialize();
	}
}

void ATenetrisGameModeBase::AddFields(AFieldBase* field)
{
	Fields.Add(field);
}

void ATenetrisGameModeBase::SetPlayerField(AFieldBase* playerField)
{
	AddFields(playerField);
	PlayerField = playerField;
}

AFieldBase* ATenetrisGameModeBase::GetPlayerField()
{
	return PlayerField;
}