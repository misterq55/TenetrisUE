// Fill out your copyright notice in the Description page of Project Settings.


#include "TNField.h"
#include "Module/MVC/View/Field/Actor/Components/TenetrisBufferComponent/TNTenetrisBufferComponent.h"

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

void ATNField::setHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (IsValid(HoldBufferComponent))
	{
		HoldBufferComponent->SetMinoType(x, y, tetrominoType);
	}
}

void ATNField::setVisibilityHoldMino(const int32 x, const int32 y, const bool visible)
{
	if (IsValid(HoldBufferComponent))
	{
		HoldBufferComponent->SetVisibilityMino(x, y, visible);
	}
}

void ATNField::setPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (IsValid(PreviewBufferComponent))
	{
		PreviewBufferComponent->SetMinoType(x, y, tetrominoType);
	}
}

void ATNField::setVisibilityPreviewMino(const int32 x, const int32 y, const bool visible)
{
	if (IsValid(PreviewBufferComponent))
	{
		PreviewBufferComponent->SetVisibilityMino(x, y, visible);
	}
}

void ATNField::HideGuideTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setVisibilityMino(coord.X + tetrominoInfo->GuideTetrominoPosition.X,
		                  coord.Y + tetrominoInfo->GuideTetrominoPosition.Y, false);
	}
}

void ATNField::SetGuideTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setMinoType(coord.X + tetrominoInfo->GuideTetrominoPosition.X,
		            coord.Y + tetrominoInfo->GuideTetrominoPosition.Y, E_TNTetrominoType::Guide);
	}
}

void ATNField::HideHoldTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.HoldTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setVisibilityHoldMino(coord.X + tetrominoInfo->CurrentPosition.X,
		                      coord.Y + tetrominoInfo->CurrentPosition.Y, false);
	}
}

void ATNField::SetHoldTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.HoldTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setHoldMinoType(coord.X + tetrominoInfo->CurrentPosition.X, coord.Y + tetrominoInfo->CurrentPosition.Y,
		                tetrominoInfo->CurrentType);
	}
}

void ATNField::HidePreviewTetromino(const FTNFieldContext& fieldContext)
{
	TArray<TSharedPtr<FTNTetrominoInfo>> previewTetrominoInfos = fieldContext.PreviewTetrominoInfos;

	for (const auto& tetrominoInfo : previewTetrominoInfos)
	{
		if (!tetrominoInfo.IsValid())
		{
			continue;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			setVisibilityPreviewMino(coord.X + tetrominoInfo->CurrentPosition.X,
			                         coord.Y + tetrominoInfo->CurrentPosition.Y, false);
		}
	}
}

void ATNField::SetPreviewTetromino(const FTNFieldContext& fieldContext)
{
	TArray<TSharedPtr<FTNTetrominoInfo>> previewTetrominoInfos = fieldContext.PreviewTetrominoInfos;

	for (const auto& tetrominoInfo : previewTetrominoInfos)
	{
		if (!tetrominoInfo.IsValid())
		{
			continue;
		}

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			setPreviewMinoType(coord.X + tetrominoInfo->CurrentPosition.X,
			                   coord.Y + tetrominoInfo->CurrentPosition.Y, tetrominoInfo->CurrentType);
		}
	}
}

void ATNField::initializePreviewBuffer()
{
	PreviewTetrominoNum = 5;

	PreviewBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}

void ATNField::initializeHoldBuffer()
{
	HoldBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("HoldBufferComponent"));
	HoldBufferComponent->SetBufferSize(4, 5);
	HoldBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoldBufferComponent->SetMobility(EComponentMobility::Movable);
	HoldBufferComponent->SetRelativeLocation(FVector(0.f, -200.f, 150.f));
	HoldBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
}
