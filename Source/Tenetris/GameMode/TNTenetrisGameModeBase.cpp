// Copyright Epic Games, Inc. All Rights Reserved.


#include "TNTenetrisGameModeBase.h"
#include "Tenetris/Actors/Pawn/TNTenetrisPawn.h"
#include "Tenetris/PlayerController/TNTenetrisPlayerController.h"

ATNTenetrisGameModeBase::ATNTenetrisGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATNTenetrisPawn::StaticClass();
	PlayerControllerClass = ATNTenetrisPlayerController::StaticClass();
}

void ATNTenetrisGameModeBase::AddFields(ATNFieldBase* field)
{
	Fields.Add(field);
}

void ATNTenetrisGameModeBase::SetPlayerField(ATNFieldBase* playerField)
{
	PlayerField = playerField;
}
