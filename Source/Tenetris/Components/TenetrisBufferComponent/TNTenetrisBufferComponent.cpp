// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/mino/TNMinoBase.h"

// Sets default values for this component's properties
UTNTenetrisBufferComponent::UTNTenetrisBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	BackgroundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BACKGROUNDMESH_COMPONENT"));
	BackgroundMeshComponent->SetStaticMesh(MeshObj.Object);
	BackgroundMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMeshComponent->SetMobility(EComponentMobility::Movable);
	BackgroundMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	BackGroundMinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BackGroundMinoBufferPivot"));
	MinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MinoBufferPivot"));

	BackGroundMinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundMinoBufferPivot->SetMobility(EComponentMobility::Movable);

	MinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	MinoBufferPivot->SetMobility(EComponentMobility::Movable);
}


// Called when the game starts
void UTNTenetrisBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTNTenetrisBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	if (bLineDeleting)
	{
		for (const int32 deleteLine : DeletedLines)
		{
			for (int32 j = 0; j < BufferWidth; j++)
			{
				SetValueToCheckBuffer(j, deleteLine, E_TNTetrominoType::None);
				SetVisibilityBackgroundCube(j, deleteLine, false);
			}
		}

		if (DeleteLineCheckTime >= 0.5f)
		{
			TArray<int32> lineChecker;
			int32 lineDeleteValue = 0;

			for (int32 i = 0; i < BufferHeight; i++)
			{
				if (DeletedLines.Find(i) != -1)
				{
					lineChecker.Add(-1);
					lineDeleteValue++;
				}
				else
				{
					lineChecker.Add(lineDeleteValue);
				}
			}

			for (int32 i = 0; i < BufferHeight; i++)
			{
				if (lineChecker[i] != -1)
				{
					for (int32 j = 0; j < BufferWidth; j++)
					{
						const E_TNTetrominoType value = GetValueFromCheckBuffer(j, i);
						SetValueToCheckBuffer(j, i, E_TNTetrominoType::None);
						SetVisibilityBackgroundCube(j, i, false);
						
						if (value == E_TNTetrominoType::None)
						{
							SetValueToCheckBuffer(j, i - lineChecker[i], E_TNTetrominoType::None);
						}
						else
						{
							SetBackgroundCubeType(j, i - lineChecker[i], value);
						}
					}
				}
			}

			DeletedLines.Empty();
			DeleteLineCheckTime = 0.f;
			bLineDeleting = false;
		}

		DeleteLineCheckTime += DeltaTime;
	}
}

void UTNTenetrisBufferComponent::Initialize()
{
	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		for (int32 j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* childComponent = NewObject<UChildActorComponent>(this);
			
			if (!IsValid(childComponent))
			{
				continue;
			}

			childComponent->SetChildActorClass(MinoClass);
			childComponent->AttachToComponent(BackGroundMinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			childComponent->CreateChildActor();

			ATNMinoBase* minoBase = Cast<ATNMinoBase>(childComponent->GetChildActor());

			if (!IsValid(minoBase))
			{
				continue;
			}

			minoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));

			BackgroundCubeBuffer[i].Add(minoBase);
		}
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		for (int32 j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* childComponent = NewObject<UChildActorComponent>(this);

			if (!IsValid(childComponent))
			{
				continue;
			}

			childComponent->SetChildActorClass(MinoClass);
			childComponent->AttachToComponent(MinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			childComponent->CreateChildActor();

			ATNMinoBase* minoBase = Cast<ATNMinoBase>(childComponent->GetChildActor());

			if (!IsValid(minoBase))
			{
				continue;
			}

			minoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));

			MinoBuffer[i].Add(minoBase);
		}
	}
}

void UTNTenetrisBufferComponent::SetMinoClassType(const TSubclassOf<ATNMinoBase> minoClass)
{
	MinoClass = minoClass;
}

void UTNTenetrisBufferComponent::SetBackgroundCubeType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = BufferWidth - x - 1;
	}

	ATNMinoBase* mino = BackgroundCubeBuffer[y][newX];

	if (IsValid(mino))
	{
		mino->SetVitibility(true);
		mino->SetTetrominoType(tetrominoType);
	}

	SetValueToCheckBuffer(x, y, tetrominoType);
}

void UTNTenetrisBufferComponent::SetVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible)
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = BufferWidth - x - 1;
	}

	ATNMinoBase* mino = BackgroundCubeBuffer[y][newX];

	if (IsValid(mino))
	{
		mino->SetVitibility(visible);
	}

	SetValueToCheckBuffer(x, y, E_TNTetrominoType::None);
}

void UTNTenetrisBufferComponent::SetMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	ATNMinoBase* mino = MinoBuffer[y][x];

	if (IsValid(mino))
	{
		mino->SetVitibility(true);
		mino->SetTetrominoType(tetrominoType);
	}
}

void UTNTenetrisBufferComponent::SetVisibilityMino(const int32 x, const int32 y, const bool visible)
{
	ATNMinoBase* mino = MinoBuffer[y][x];

	if (IsValid(mino))
	{
		mino->SetVitibility(visible);
	}
}

bool UTNTenetrisBufferComponent::CheckMino(const int32 x, const int32 y)
{
	if (y < 0 || x < 0) return true;
	if (y >= BufferHeight || x >= BufferWidth) return true;

	return GetValueFromCheckBuffer(x, y) != E_TNTetrominoType::None;
}

void UTNTenetrisBufferComponent::SetBufferSize(const int32 bufferHeight, const int32 bufferWidth)
{
	BufferHeight = bufferHeight;
	BufferWidth = bufferWidth;

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATNMinoBase*> buffer;
		buffer.Reserve(BufferWidth);

		BackgroundCubeBuffer.Add(buffer);
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATNMinoBase*> buffer;
		buffer.Reserve(BufferWidth);

		MinoBuffer.Add(buffer);
	}

	for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
	{
		TArray<E_TNTetrominoType> buffer;
		for (int32 j = 0; j < BufferWidth + 2; j++)
		{
			if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				buffer.Add(E_TNTetrominoType::Obstacle);
			else
				buffer.Add(E_TNTetrominoType::None);
		}

		CheckBuffer.Add(buffer);
	}

	if (IsValid(BackgroundMeshComponent))
	{
		BackgroundMeshComponent->SetRelativeScale3D(FVector(MinoRatio * BufferHeight, MinoRatio * BufferWidth, 1));
	}
}

int32 UTNTenetrisBufferComponent::CalculateGuideMinoHeight(const int32 x, const int32 y)
{
	int32 result = 0;
	int32 height = 0;

	for (int32 i = y; i >= -1; i--)
	{
		if (GetValueFromCheckBuffer(x, i) != E_TNTetrominoType::None)
		{
			height = i;
			break;
		}
	}

	result = y - height - 1;

	return result;
}

void UTNTenetrisBufferComponent::CheckLineDelete(const TArray<int32> heights)
{
	for (const int32& height : heights)
	{
		bool lineDeleted = true;

		for (int32 j = 0; j < BufferWidth; j++)
		{
			if (GetValueFromCheckBuffer(j, height) == E_TNTetrominoType::None)
			{
				lineDeleted = false;
				break;
			}
		}

		if (lineDeleted)
		{
			DeletedLines.AddUnique(height);
		}
	}

	if (DeletedLines.Num())
	{
		bLineDeleting = true;
	}
}

E_TNTetrominoType UTNTenetrisBufferComponent::GetValueFromCheckBuffer(const int32 x, const int32 y)
{
	int32 newX = x;
	
	if (bSpaceInverted)
	{
		newX = BufferWidth - x - 1;
	}

	return CheckBuffer[y + 1][newX + 1];
}

void UTNTenetrisBufferComponent::SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = BufferWidth - x - 1;
	}

	CheckBuffer[y + 1][newX + 1] = tetrominoType;
}

void UTNTenetrisBufferComponent::ToggleSpaceInversion()
{
	if (!BackGroundMinoBufferPivot)
	{
		return;
	}

	bSpaceInverted = !bSpaceInverted;

	if (bSpaceInverted)
	{
		BackGroundMinoBufferPivot->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	}
	else
	{
		BackGroundMinoBufferPivot->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
}

