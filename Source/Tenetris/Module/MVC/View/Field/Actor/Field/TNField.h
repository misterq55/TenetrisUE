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

private:
	void initializePreviewBuffer();
	void initializeHoldBuffer();

private:
	int32 PreviewTetrominoNum;
	UTNTenetrisBufferComponent* PreviewBufferComponent;
	UTNTenetrisBufferComponent* HoldBufferComponent;
};
