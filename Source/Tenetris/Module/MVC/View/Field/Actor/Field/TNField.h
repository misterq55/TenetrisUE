// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Module/MVC/View/Field/Actor/Field/TNFieldBase.h"
#include "TNField.generated.h"

/**
 * 
 */

class UTNTenetrisBufferComponent;

UCLASS()
class TENETRIS_API ATNField : public ATNFieldBase
{
	GENERATED_BODY()
	
public:
	ATNField();
	virtual	void Initialize() override;

	void HideGuideTetromino(const FTNFieldContext& fieldContext);
	void SetGuideTetromino(const FTNFieldContext& fieldContext);
	void HideHoldTetromino(const FTNFieldContext& fieldContext);
	void SetHoldTetromino(const FTNFieldContext& fieldContext);
	void HidePreviewTetromino(const FTNFieldContext& fieldContext);
	void SetPreviewTetromino(const FTNFieldContext& fieldContext);
	
private:
	void initializePreviewBuffer();
	void initializeHoldBuffer();
	void setHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void setVisibilityHoldMino(const int32 x, const int32 y, const bool visible);
	void setPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void setVisibilityPreviewMino(const int32 x, const int32 y, const bool visible);

private:
	int32 PreviewTetrominoNum;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTNTenetrisBufferComponent> PreviewBufferComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTNTenetrisBufferComponent> HoldBufferComponent;
};
