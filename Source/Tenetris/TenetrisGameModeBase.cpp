// Copyright Epic Games, Inc. All Rights Reserved.


#include "TenetrisGameModeBase.h"
#include "TenetrisPawn.h"

ATenetrisGameModeBase::ATenetrisGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATenetrisPawn::StaticClass();
}

void ATenetrisGameModeBase::Initialize_Implementation()
{
}

void ATenetrisGameModeBase::AddTenetrisFields(ATenetrisFieldBase* InTenetrisField)
{
	TenetrisFields.Add(InTenetrisField);
}

void ATenetrisGameModeBase::SetPlayerTenetrisField(ATenetrisFieldBase* InPlayerTenetrisField)
{
	AddTenetrisFields(InPlayerTenetrisField);
	PlayerTenetrisField = InPlayerTenetrisField;
}

ATenetrisFieldBase* ATenetrisGameModeBase::GetPlayerTenetrisField()
{
	return PlayerTenetrisField;
}