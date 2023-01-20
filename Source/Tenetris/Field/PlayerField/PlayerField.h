// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Field/FieldBase.h"
#include "PlayerField.generated.h"

class FTetrominoBase;
class FTetrominoSpawner;

UCLASS()
class TENETRIS_API APlayerField : public AFieldBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerField();
	virtual ~APlayerField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual	void Initialize();

	void MoveTetromino(ETetrominoDirection InTetrominoDirection);
	void RotateTetromino(ETetrominoRotation InTetrominoRotation);
	void HardDrop();

private:
	void RegisterActions();
	void UnRegisterActions();

protected:
	void BindTetrominoToBuffer(FTetrominoBase* InTetromino, UTenetrisBufferComponent* InBuffer);
	void Spawn();

protected:
	FTetrominoBase* CurrentTetromino;
	FTetrominoSpawner* TetrominoSpawner;
	TArray<FTetrominoBase*> PreviewTetrominos;
	UTenetrisBufferComponent* PreviewBufferComponent;
	int32 PreviewTetrominoNum;
};
