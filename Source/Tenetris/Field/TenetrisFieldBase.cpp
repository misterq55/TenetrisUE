// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisFieldBase.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"
#include "Tenetris/Field/Tetromino/TetrominoCube/TetrominoCubeBase.h"

// Sets default values
ATenetrisFieldBase::ATenetrisFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(1.f);
	TetrominoCubeClass = ATetrominoCubeBase::StaticClass();
	
	for (int32 i = 0; i < RowMax; i++)
	{
		TArray<ATetrominoCubeBase*> Buffer;
		Buffer.Reserve(ColumnMax);

		CubeBuffer.Add(Buffer);
	}
}

ATenetrisFieldBase::~ATenetrisFieldBase()
{

}

// Called when the game starts or when spawned
void ATenetrisFieldBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATenetrisFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATenetrisFieldBase::Initialize()
{
	for (int i = 0; i < RowMax; i++)
	{
		for (int j = 0; j < ColumnMax; j++)
		{
			UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(this);
			ChildComponent->SetChildActorClass(TetrominoCubeClass);
			ChildComponent->SetupAttachment(RootComponent);
			ChildComponent->CreateChildActor();
			
			ATetrominoCubeBase* TetrominoCubeBase = Cast<ATetrominoCubeBase>(ChildComponent->GetChildActor());
			TetrominoCubeBase->SetTetrominoCubePosition(j, i);

			CubeBuffer[i].Add(TetrominoCubeBase);
		}
	}
}
