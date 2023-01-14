// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrominoCubeBase.generated.h"

class UStaticMeshComponent;

enum class ETetrominoType
{
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
	Obstacle,
};

UCLASS()
class TENETRIS_API ATetrominoCubeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrominoCubeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVitibility(bool InNewVisibility);
	void SetTetrominoType(ETetrominoType InTetrominoType);
	void SetTetrominoCubePosition(int32 X, int32 Y);

protected:
	UStaticMeshComponent* CubeMeshComponent;
	ETetrominoType TetrominoType;
};
