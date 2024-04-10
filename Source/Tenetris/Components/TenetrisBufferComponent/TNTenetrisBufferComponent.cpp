// Fill out your copyright notice in the Description page of Project Settings.


#include "TNTenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/Mino/TNMinoBase.h"

// Sets default values for this component's properties
UTNTenetrisBufferComponent::UTNTenetrisBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	BackgroundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	BackgroundMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMeshComponent->SetStaticMesh(MeshObj.Object);
	BackgroundMeshComponent->SetMobility(EComponentMobility::Movable);
	// BackgroundMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	BackGroundMinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BackGroundMinoBufferPivot"));
	MinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MinoBufferPivot"));

	BackGroundMinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundMinoBufferPivot->SetMobility(EComponentMobility::Movable);
	// BackGroundMinoBufferPivot->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

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
		for (int32 DeleteLine : DeletedLines)
		{
			for (int32 j = 0; j < BufferWidth; j++)
			{
				SetValueToCheckBuffer(j, DeleteLine, E_TNTetrominoType::None);
				SetVisibilityBackgroundCube(j, DeleteLine, false);
			}
		}

		if (DeleteLineCheckTime >= 0.5f)
		{
			TArray<int32> LineChecker;
			int32 LineDeleteValue = 0;

			for (int32 i = 0; i < BufferHeight; i++)
			{
				if (DeletedLines.Find(i) != -1)
				{
					LineChecker.Add(-1);
					LineDeleteValue++;
				}
				else
					LineChecker.Add(LineDeleteValue);
			}

			for (int32 i = 0; i < BufferHeight; i++)
			{
				if (LineChecker[i] != -1)
				{
					for (int32 j = 0; j < BufferWidth; j++)
					{
						E_TNTetrominoType Value = GetValueFromCheckBuffer(j, i);
						SetValueToCheckBuffer(j, i, E_TNTetrominoType::None);
						SetVisibilityBackgroundCube(j, i, false);
						if (Value == E_TNTetrominoType::None)
							SetValueToCheckBuffer(j, i - LineChecker[i], E_TNTetrominoType::None);
						else
							SetBackgroundCubeType(j, i - LineChecker[i], Value);
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
	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			
			if (!IsValid(ChildComponent))
				continue; 

			ChildComponent->SetChildActorClass(MinoClass);
			ChildComponent->AttachToComponent(BackGroundMinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			ATNMinoBase* MinoBase = Cast<ATNMinoBase>(ChildComponent->GetChildActor());

			if (!IsValid(MinoBase))
				continue;

			MinoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));

			BackgroundCubeBuffer[i].Add(MinoBase);
		}
	}

	// BackGroundMinoBufferPivot->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);

			if (!IsValid(ChildComponent))
			{
				continue;
			}

			ChildComponent->SetChildActorClass(MinoClass);
			ChildComponent->AttachToComponent(MinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			ATNMinoBase* MinoBase = Cast<ATNMinoBase>(ChildComponent->GetChildActor());

			if (!IsValid(MinoBase))
			{
				continue;
			}

			MinoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));

			MinoBuffer[i].Add(MinoBase);
		}
	}
}

void UTNTenetrisBufferComponent::SetMinoClassType(TSubclassOf<ATNMinoBase> minoClass)
{
	MinoClass = minoClass;
}

void UTNTenetrisBufferComponent::SetBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	int32 NewX = x;

	if (bSpaceInverted)
	{
		NewX = BufferWidth - x - 1;
	}

	ATNMinoBase* Mino = BackgroundCubeBuffer[y][NewX];

	if (IsValid(Mino))
	{
		Mino->SetVitibility(true);
		Mino->SetTetrominoType(tetrominoType);
	}

	SetValueToCheckBuffer(x, y, tetrominoType);
}

void UTNTenetrisBufferComponent::SetVisibilityBackgroundCube(int32 x, int32 y, bool visible)
{
	int32 NewX = x;

	if (bSpaceInverted)
		NewX = BufferWidth - x - 1;

	ATNMinoBase* Mino = BackgroundCubeBuffer[y][NewX];

	if (IsValid(Mino))
	{
		Mino->SetVitibility(visible);
	}

	SetValueToCheckBuffer(x, y, E_TNTetrominoType::None);
}

void UTNTenetrisBufferComponent::SetMinoType(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	ATNMinoBase* Mino = MinoBuffer[y][x];

	if (IsValid(Mino))
	{
		Mino->SetVitibility(true);
		Mino->SetTetrominoType(tetrominoType);
	}
}

void UTNTenetrisBufferComponent::SetVisibilityMino(int32 x, int32 y, bool visible)
{
	ATNMinoBase* Mino = MinoBuffer[y][x];

	if (IsValid(Mino))
	{
		Mino->SetVitibility(visible);
	}
}

bool UTNTenetrisBufferComponent::CheckMino(int32 x, int32 y)
{
	if (y < 0 || x < 0) return true;
	if (y >= BufferHeight || x >= BufferWidth) return true;

	return GetValueFromCheckBuffer(x, y) != E_TNTetrominoType::None;
}

void UTNTenetrisBufferComponent::SetBufferSize(int32 bufferHeight, int32 bufferWidth)
{
	BufferHeight = bufferHeight;
	BufferWidth = bufferWidth;

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATNMinoBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		BackgroundCubeBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<ATNMinoBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		MinoBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
	{
		TArray<E_TNTetrominoType> Buffer;
		for (int32 j = 0; j < BufferWidth + 2; j++)
		{
			if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				Buffer.Add(E_TNTetrominoType::Obstacle);
			else
				Buffer.Add(E_TNTetrominoType::None);
		}

		CheckBuffer.Add(Buffer);
	}

	if (IsValid(BackgroundMeshComponent))
	{
		BackgroundMeshComponent->SetRelativeScale3D(FVector(MinoRatio * BufferHeight, MinoRatio * BufferWidth, 0.f));
	}
}

int32 UTNTenetrisBufferComponent::CalculateGuideMinoHeight(int32 x, int32 y)
{
	int32 Result = 0;
	int32 Height = 0;

	for (int32 i = y; i >= -1; i--)
	{
		if (GetValueFromCheckBuffer(x, i) != E_TNTetrominoType::None)
		{
			Height = i;
			break;
		}
	}

	Result = y - Height - 1;

	return Result;
}

void UTNTenetrisBufferComponent::CheckLineDelete(TArray<int32> heights)
{
	for (const int32& Height : heights)
	{
		bool LineDeleted = true;

		for (int32 j = 0; j < BufferWidth; j++)
		{
			if (GetValueFromCheckBuffer(j, Height) == E_TNTetrominoType::None)
			{
				LineDeleted = false;
				break;
			}
		}

		if (LineDeleted)
		{
			DeletedLines.AddUnique(Height);
		}
	}

	if (DeletedLines.Num())
		bLineDeleting = true;
}

E_TNTetrominoType UTNTenetrisBufferComponent::GetValueFromCheckBuffer(int32 x, int32 y)
{
	int32 NewX = x;
	
	if (bSpaceInverted)
	{
		NewX = BufferWidth - x - 1;
	}

	return CheckBuffer[y + 1][NewX + 1];
}

void UTNTenetrisBufferComponent::SetValueToCheckBuffer(int32 x, int32 y, E_TNTetrominoType tetrominoType)
{
	int32 NewX = x;

	if (bSpaceInverted)
	{
		NewX = BufferWidth - x - 1;
	}

	CheckBuffer[y + 1][NewX + 1] = tetrominoType;
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

