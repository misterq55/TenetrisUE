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

void ATenetrisGameModeBase::AddFields(AFieldBase* InField)
{
	Fields.Add(InField);
}

void ATenetrisGameModeBase::SetPlayerField(AFieldBase* InPlayerField)
{
	AddFields(InPlayerField);
	PlayerField = InPlayerField;
}

AFieldBase* ATenetrisGameModeBase::GetPlayerField()
{
	return PlayerField;
}