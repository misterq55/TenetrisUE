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

	void SetHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityHoldMino(const int32 x, const int32 y, const bool visible);
	void SetPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityPreviewMino(const int32 x, const int32 y, const bool visible);

private:
	void initializePreviewBuffer();
	void initializeHoldBuffer();

private:
	int32 PreviewTetrominoNum;
protected:
	UTNTenetrisBufferComponent* PreviewBufferComponent;
	UTNTenetrisBufferComponent* HoldBufferComponent;
};
