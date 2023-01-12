// Copyright Epic Games, Inc. All Rights Reserved.


#include "TenetrisGameModeBase.h"
#include "TenetrisPawn.h"

ATenetrisGameModeBase::ATenetrisGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATenetrisPawn::StaticClass();
}