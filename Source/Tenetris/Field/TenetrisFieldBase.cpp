// Fill out your copyright notice in the Description page of Project Settings.


#include "TenetrisFieldBase.h"
#include "Tenetris/Field/Tetromino/TetrominoBase.h"

// Sets default values
ATenetrisFieldBase::ATenetrisFieldBase()
	: CurrentTetromino(nullptr)
	, PrevTetromino(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ATenetrisFieldBase::~ATenetrisFieldBase()
{
	if (CurrentTetromino)
		delete CurrentTetromino;

	if (PrevTetromino)
		delete PrevTetromino;
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
	
}

void ATenetrisFieldBase::RotateTetromino(ETetrominoRotation InTetrominoRotation)
{
}

void ATenetrisFieldBase::MoveTetromino(ETetrominoDirection InTetrominoDirection)
{
}
