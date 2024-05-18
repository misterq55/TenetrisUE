// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNMinoBase.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class TENETRIS_API ATNMinoBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATNMinoBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitailizeMesh(UStaticMesh* StaticMesh);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVitibility(bool newVisibility);
	virtual void SetTetrominoType(E_TNTetrominoType tetrominoType);

protected:
	UStaticMeshComponent* MinoMeshComponent;
	E_TNTetrominoType TetrominoType;
};
