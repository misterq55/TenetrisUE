// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Field/TenetrisFieldBase.h"
#include "PlayerTenetrisField.generated.h"

class FTetrominoBase;

UCLASS()
class TENETRIS_API APlayerTenetrisField : public ATenetrisFieldBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerTenetrisField();
	virtual ~APlayerTenetrisField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual	void Initialize();

	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void RotateClockWise();
	void RotateCounterClockWise();

protected:
	FTetrominoBase* CurrentTetromino;
	FTetrominoBase* PrevTetromino;
};
