// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisBufferComponent.h"
#include "Tenetris/Field/Tetromino/Mino/MinoBase.h"

// Sets default values for this component's properties
UTenetrisBufferComponent::UTenetrisBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	BackgroundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH_COMPONENT"));
	BackgroundMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMeshComponent->SetStaticMesh(MeshObj.Object);
	BackgroundMeshComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	BackGroundMinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BackGroundMinoBufferPivot"));
	MinoBufferPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MinoBufferPivot"));

	BackGroundMinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackGroundMinoBufferPivot->SetMobility(EComponentMobility::Movable);

	MinoBufferPivot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	MinoBufferPivot->SetMobility(EComponentMobility::Movable);
}


// Called when the game starts
void UTenetrisBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTenetrisBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	if (bLineDeleting)
	{
		for (int32 DeleteLine : DeletedLines)
		{
			for (int32 j = 0; j < BufferWidth; j++)
			{
				SetValueToCheckBuffer(j, DeleteLine, ETetrominoType::None);
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
						ETetrominoType Value = GetValueFromCheckBuffer(j, i);
						SetValueToCheckBuffer(j, i, ETetrominoType::None);
						SetVisibilityBackgroundCube(j, i, false);
						if (Value == ETetrominoType::None)
							SetValueToCheckBuffer(j, i - LineChecker[i], ETetrominoType::None);
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

void UTenetrisBufferComponent::Initialize()
{
	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			ChildComponent->SetChildActorClass(MinoClass);
			ChildComponent->AttachToComponent(BackGroundMinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			AMinoBase* MinoBase = Cast<AMinoBase>(ChildComponent->GetChildActor());
			MinoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));

			BackgroundCubeBuffer[i].Add(MinoBase);
		}
	}

	for (int i = 0; i < BufferHeight + 2; i++)
	{
		for (int j = 0; j < BufferWidth; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			ChildComponent->SetChildActorClass(MinoClass);
			ChildComponent->AttachToComponent(MinoBufferPivot, FAttachmentTransformRules::KeepRelativeTransform);
			ChildComponent->CreateChildActor();

			AMinoBase* MinoBase = Cast<AMinoBase>(ChildComponent->GetChildActor());
			MinoBase->SetActorRelativeLocation(FVector(0.f, MinoRatio * 100.f * (j - (BufferWidth - 1) * 0.5f), MinoRatio * 100.f * (i - BufferHeight * 0.5f)));

			MinoBuffer[i].Add(MinoBase);
		}
	}
}

void UTenetrisBufferComponent::SetMinoClassType(TSubclassOf<AMinoBase> InMinoClass)
{
	MinoClass = InMinoClass;
}

void UTenetrisBufferComponent::SetBackgroundCubeType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	AMinoBase* Mino = BackgroundCubeBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(true);
		Mino->SetTetrominoType(InTetrominoType);
	}

	SetValueToCheckBuffer(X, Y, InTetrominoType);
}

void UTenetrisBufferComponent::SetVisibilityBackgroundCube(int32 X, int32 Y, bool InVisible)
{
	AMinoBase* Mino = BackgroundCubeBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(InVisible);
	}

	SetValueToCheckBuffer(X, Y, ETetrominoType::None);
}

void UTenetrisBufferComponent::SetMinoType(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	AMinoBase* Mino = MinoBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(true);
		Mino->SetTetrominoType(InTetrominoType);
	}
}

void UTenetrisBufferComponent::SetVisibilityMino(int32 X, int32 Y, bool InVisible)
{
	AMinoBase* Mino = MinoBuffer[Y][X];

	if (Mino)
	{
		Mino->SetVitibility(InVisible);
	}
}

bool UTenetrisBufferComponent::CheckMino(int32 X, int32 Y)
{
	if (Y < 0 || X < 0) return true;
	if (Y >= BufferHeight || X >= BufferWidth) return true;

	return GetValueFromCheckBuffer(X, Y) != ETetrominoType::None;
}

void UTenetrisBufferComponent::SetBufferSize(int32 InBufferHeight, int32 InBufferWidth)
{
	BufferHeight = InBufferHeight;
	BufferWidth = InBufferWidth;

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<AMinoBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		BackgroundCubeBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight + 2; i++)
	{
		TArray<AMinoBase*> Buffer;
		Buffer.Reserve(BufferWidth);

		MinoBuffer.Add(Buffer);
	}

	for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
	{
		TArray<ETetrominoType> Buffer;
		for (int32 j = 0; j < BufferWidth + 2; j++)
		{
			if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				Buffer.Add(ETetrominoType::Obstacle);
			else
				Buffer.Add(ETetrominoType::None);
		}

		CheckBuffer.Add(Buffer);
	}

	BackgroundMeshComponent->SetRelativeScale3D(FVector(MinoRatio * BufferHeight, MinoRatio * BufferWidth, 0.f));
}

int32 UTenetrisBufferComponent::CalculateGuideMinoHeight(int32 X, int32 Y)
{
	int32 Result = 0;
	int32 Height = 0;

	for (int32 i = Y; i >= -1; i--)
	{
		if (GetValueFromCheckBuffer(X, i) != ETetrominoType::None)
		{
			Height = i;
			break;
		}
	}

	Result = Y - Height - 1;

	return Result;
}

void UTenetrisBufferComponent::CheckLineDelete(TArray<int32> Heights)
{
	for (int32 Height : Heights)
	{
		bool LineDeleted = true;

		for (int32 j = 0; j < BufferWidth; j++)
		{
			if (GetValueFromCheckBuffer(j, Height) == ETetrominoType::None)
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

ETetrominoType UTenetrisBufferComponent::GetValueFromCheckBuffer(int32 X, int32 Y)
{
	return CheckBuffer[Y + 1][X + 1];
}

void UTenetrisBufferComponent::SetValueToCheckBuffer(int32 X, int32 Y, ETetrominoType InTetrominoType)
{
	CheckBuffer[Y + 1][X + 1] = InTetrominoType;
}

