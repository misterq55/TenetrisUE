#pragma once

#include "Tenetris/TenetrisDefine.h"

class ATNFieldBase;

class ITNController
{
public:
	virtual ~ITNController() {}
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateField(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor) = 0;
	virtual void StartPlay() = 0;
	virtual void HandleControlInput(const E_TNControlType controlType) = 0;
};