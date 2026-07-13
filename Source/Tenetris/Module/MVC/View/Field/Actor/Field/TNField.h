// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tenetris/TenetrisDefine.h"
#include "TNField.generated.h"

/**
 * 
 */

class UTNTenetrisBufferComponent;
class ATNMinoBase;

UCLASS()
class TENETRIS_API ATNField : public AActor
{
	GENERATED_BODY()
	
public:
	ATNField();
	virtual ~ATNField();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Field")
	void Initialize();

	void RotateField(const FTNFieldContext& fieldContext);
	void HideTetromino(const FTNFieldContext& fieldContext);
	void ShowTetromino(const FTNFieldContext& fieldContext);
	void LockDown(const FTNFieldContext& fieldContext);
	void HideGuideTetromino(const FTNFieldContext& fieldContext);
	void ShowGuideTetromino(const FTNFieldContext& fieldContext);
	void HideHoldTetromino(const FTNFieldContext& fieldContext);
	void ShowHoldTetromino(const FTNFieldContext& fieldContext);
	void HidePreviewTetromino(const FTNFieldContext& fieldContext);
	void ShowPreviewTetromino(const FTNFieldContext& fieldContext);
	
	void UpdatePreviewTetrominoes(const FTNFieldContext& fieldContext);
	
protected:
	void initializePreviewBuffer();
	void initializeHoldBuffer();
	void setMinoClassType(TSubclassOf<ATNMinoBase> minoClass);
	void setMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void setVisibilityMino(const int32 x, const int32 y, const bool visible);
	void setBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType);
	void setHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void setVisibilityHoldMino(const int32 x, const int32 y, const bool visible);
	void setPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void setVisibilityPreviewMino(const int32 x, const int32 y, const bool visible);

private:
	int32 PreviewTetrominoNum;
	TSubclassOf<ATNMinoBase> MinoClass;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTNTenetrisBufferComponent> TenetrisBufferComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTNTenetrisBufferComponent> PreviewBufferComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTNTenetrisBufferComponent> HoldBufferComponent;

	float RotationRemainingTime = 0.f;
	bool bCachedSpaceInverted = false;
	TArray<FVector2D> PreviewTetrominoPositions;
	TArray<E_TNTetrominoType> PreviewTetrominoTypes;
};
