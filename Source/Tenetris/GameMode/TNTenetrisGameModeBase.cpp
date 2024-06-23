// Copyright Epic Games, Inc. All Rights Reserved.


#include "TNTenetrisGameModeBase.h"
#include "Tenetris/TNTenetrisPawn.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Field/TNField.h"
#include "Tenetris/PlayerController/TNTenetrisPlayerController.h"

ATNTenetrisGameModeBase::ATNTenetrisGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATNTenetrisPawn::StaticClass();
	PlayerControllerClass = ATNTenetrisPlayerController::StaticClass();
}

void ATNTenetrisGameModeBase::AddFields(ATNField* field)
{
	Fields.Add(field);
}

void ATNTenetrisGameModeBase::SetPlayerField(ATNField* playerField)
{
	AddFields(playerField);
}
