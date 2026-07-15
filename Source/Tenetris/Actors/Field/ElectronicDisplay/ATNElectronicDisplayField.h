// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tenetris/Actors/Field/TNFieldBase.h"
#include "ATNElectronicDisplayField.generated.h"

/**
 * 
 */

class UTNTenetrisBufferComponent;
class ATNMinoBase;

UCLASS()
class TENETRIS_API ATNElectronicDisplayField : public ATNFieldBase
{
	GENERATED_BODY()
	
public:
	ATNElectronicDisplayField();
	virtual ~ATNElectronicDisplayField();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Initialize() override;
	virtual void HandleFieldState(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) override;
	virtual void ApplyPreviewVisible(bool bVisible) override;
	virtual void ApplyHoldVisible(bool bVisible) override;
	
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
	void updateTetromino(const FTNFieldContext& fieldContext);
	void updateHoldTetromino(const FTNFieldContext& fieldContext) const;
	void updatePreviewTetrominoes(const FTNFieldContext& fieldContext) const;
	void lockDown(const FTNFieldContext& fieldContext);
	void rotateField(const FTNFieldContext& fieldContext);

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
	FVector2D HoldTetrominoPosition;
};
