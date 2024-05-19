// Fill out your copyright notice in the Description page of Project Settings.


#include "TNField.h"
#include "Module/MVC/View/Field/Actor/Components/TenetrisBufferComponent/TNOldTenetrisBufferComponent.h"

ATNField::ATNField()
	: ATNFieldBase()
{
	PrimaryActorTick.bCanEverTick = true;

	initializePreviewBuffer();
	initializeHoldBuffer();
}

void ATNField::Initialize()
{
	Super::Initialize();

	if (IsValid(PreviewBufferComponent))
	{
		PreviewBufferComponent->Initialize();
	}

	if (IsValid(HoldBufferComponent))
	{
		HoldBufferComponent->Initialize();
	}
}

void ATNField::initializePreviewBuffer()
{
	PreviewTetrominoNum = 5;

	PreviewBufferComponent = CreateDefaultSubobject<UTNOldTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void ATNField::initializeHoldBuffer()
{
	HoldBufferComponent = CreateDefaultSubobject<UTNOldTenetrisBufferComponent>(TEXT("HoldBufferComponent"));
	HoldBufferComponent->SetBufferSize(4, 5);
	HoldBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoldBufferComponent->SetMobility(EComponentMobility::Movable);
	HoldBufferComponent->SetRelativeLocation(FVector(0.f, -200.f, 150.f));
	HoldBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}
