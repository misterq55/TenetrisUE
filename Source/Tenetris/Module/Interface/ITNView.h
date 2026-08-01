#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTNFieldView;
class ATNFieldBase;

class ITNView
{
public:
	virtual ~ITNView() = default;
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldView(const int32 key, ATNFieldBase* fieldActor) = 0;
	virtual void StartPlay() = 0;
	virtual void UpdateFieldView(const int32 modelKey, const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) = 0;

private:

};