#include "TNField.h"
#include "Tenetris/Module/MVC/View/Field/Actor/Mino/TNMinoBase.h"
#include "Module/MVC/View/Field/Actor/Components/TenetrisBufferComponent/TNTenetrisBufferComponent.h"

ATNField::ATNField()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT"));

	TenetrisBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("BufferComponent"));
	TenetrisBufferComponent->SetBufferSize(RowMax, ColumnMax);
	TenetrisBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TenetrisBufferComponent->SetMobility(EComponentMobility::Movable);

	MinoClass = ATNMinoBase::StaticClass();

	initializePreviewBuffer();
	initializeHoldBuffer();
}

ATNField::~ATNField()
{
}

void ATNField::BeginPlay()
{
	Super::BeginPlay();
}

void ATNField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotationRemainingTime > 0.f && IsValid(TenetrisBufferComponent))
	{
		RotationRemainingTime -= DeltaTime;
		if (RotationRemainingTime <= 0.f)
		{
			RotationRemainingTime = 0.f;
		}

		const float elapsed = RotationDuration - RotationRemainingTime;
		TenetrisBufferComponent->RotateField(FMath::Clamp(elapsed / RotationDuration, 0.f, 1.f), bCachedSpaceInverted);
	}
}

void ATNField::Initialize()
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->Initialize();
	}

	if (IsValid(PreviewBufferComponent))
	{
		PreviewBufferComponent->Initialize();
	}

	if (IsValid(HoldBufferComponent))
	{
		HoldBufferComponent->Initialize();
	}
}

void ATNField::RotateField(const FTNFieldContext& fieldContext)
{
	RotationRemainingTime = RotationDuration;
	bCachedSpaceInverted = fieldContext.bSpaceInverted;
}

void ATNField::HideTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setVisibilityMino(coord.X + tetrominoInfo->CurrentPosition.X,
		                  coord.Y + tetrominoInfo->CurrentPosition.Y, false);
	}
}

void ATNField::ShowTetromino(const FTNFieldContext& fieldContext)
{
	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		setMinoType(coord.X + tetrominoInfo->CurrentPosition.X,
		            coord.Y + tetrominoInfo->CurrentPosition.Y,
		            tetrominoInfo->CurrentType);
	}
}

void ATNField::UpdateTetromino(const FTNFieldContext& fieldContext)
{
	if (!IsValid(TenetrisBufferComponent))
	{
		return;
	}

	TenetrisBufferComponent->CleanMinoBuffer();

	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.PlayerTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		TenetrisBufferComponent->SetMinoType(coord.X + tetrominoInfo->CurrentPosition.X,
		                                     coord.Y + tetrominoInfo->CurrentPosition.Y,
		                                     tetrominoInfo->CurrentType);

		TenetrisBufferComponent->SetMinoType(coord.X + tetrominoInfo->GuideTetrominoPosition.X,
		                                     coord.Y + tetrominoInfo->GuideTetrominoPosition.Y,
		                                     E_TNTetrominoType::Guide);
	}
}

void ATNField::LockDown(const FTNFieldContext& fieldContext)
{
	const int32 bufferHeight = fieldContext.BufferHeight;
	const int32 bufferWidth = fieldContext.BufferWidth;

	for (int32 i = 0; i < bufferHeight; i++)
	{
		for (int32 j = 0; j < bufferWidth; j++)
		{
			const E_TNTetrominoType tetrominoType = fieldContext.CheckBuffer[i + 1][j + 1];
			setBackgroundCubeType(j, i, tetrominoType);
		}
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

void ATNField::ShowGuideTetromino(const FTNFieldContext& fieldContext)
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

void ATNField::UpdateHoldTetromino(const FTNFieldContext& fieldContext) const
{
	if (!IsValid(HoldBufferComponent))
	{
		return;
	}

	HoldBufferComponent->CleanMinoBuffer();

	TSharedPtr<FTNTetrominoInfo> tetrominoInfo = fieldContext.HoldTetrominoInfo;

	if (!tetrominoInfo.IsValid())
	{
		return;
	}

	for (const auto& coord : tetrominoInfo->Coordinate)
	{
		HoldBufferComponent->SetMinoType(coord.X + HoldTetrominoPosition.X,
		                                 coord.Y + HoldTetrominoPosition.Y, tetrominoInfo->CurrentType);
	}
}

void ATNField::UpdatePreviewTetrominoes(const FTNFieldContext& fieldContext) const
{
	if (!IsValid(PreviewBufferComponent))
	{
		return;
	}

	PreviewBufferComponent->CleanMinoBuffer();

	const TArray<TSharedPtr<FTNTetrominoInfo>>& previewTetrominoInfos = fieldContext.PreviewTetrominoInfos;

	for (int32 i = 0; i < PreviewTetrominoNum; i++)
	{
		TSharedPtr<FTNTetrominoInfo> tetrominoInfo = previewTetrominoInfos[i];
		if (!tetrominoInfo.IsValid())
		{
			continue;
		}

		const FVector2D pos = PreviewTetrominoPositions[i];

		for (const auto& coord : tetrominoInfo->Coordinate)
		{
			PreviewBufferComponent->SetMinoType(coord.X + pos.X, coord.Y + pos.Y, tetrominoInfo->CurrentType);
		}
	}
}

void ATNField::initializePreviewBuffer()
{
	PreviewTetrominoNum = PreviewTetrominoMax;

	PreviewBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("PreviewBufferComponent"));
	PreviewBufferComponent->SetBufferSize(PreviewTetrominoNum * 3 + 1, 5);
	PreviewBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewBufferComponent->SetMobility(EComponentMobility::Movable);
	PreviewBufferComponent->SetRelativeLocation(FVector(0.f, 200.f, 50.f));
	PreviewBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	PreviewTetrominoPositions.Reserve(PreviewTetrominoNum);
	for (int32 i = 0; i < PreviewTetrominoNum; i++)
	{
		PreviewTetrominoPositions.Add(FVector2D(2, (PreviewTetrominoNum - i - 1) * 3 + 1));
	}
}

void ATNField::initializeHoldBuffer()
{
	HoldBufferComponent = CreateDefaultSubobject<UTNTenetrisBufferComponent>(TEXT("HoldBufferComponent"));
	HoldBufferComponent->SetBufferSize(4, 5);
	HoldBufferComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoldBufferComponent->SetMobility(EComponentMobility::Movable);
	HoldBufferComponent->SetRelativeLocation(FVector(0.f, -200.f, 150.f));
	HoldBufferComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	HoldTetrominoPosition = FVector2D(2, 1);
}

void ATNField::setMinoClassType(TSubclassOf<ATNMinoBase> minoClass)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetMinoClassType(minoClass);
	}
}

void ATNField::setMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetMinoType(x, y, tetrominoType);
	}
}

void ATNField::setVisibilityMino(const int32 x, const int32 y, const bool visible)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetVisibilityMino(x, y, visible);
	}
}

void ATNField::setBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	if (IsValid(TenetrisBufferComponent))
	{
		TenetrisBufferComponent->SetBackgroundCubeType(x, y, tetrominoType);
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
