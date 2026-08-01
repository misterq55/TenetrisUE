#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTNFieldModel;

DECLARE_DELEGATE_ThreeParams(FTNUpdateFieldView, const int32, const FTNFieldContext&, const E_TNFieldModelStateType state);

class ITNModel
{
public:
	virtual ~ITNModel() {}
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldModel(const int32 modelKey, FTNFieldContext fieldContext, int32 height, int32 width) = 0;
	virtual void StartPlay() = 0;
	virtual void UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state) = 0;
	virtual FTNFieldContext GetFieldContext(const int32 modelKey) = 0;
	virtual TSharedPtr<FTNFieldModel> GetPlayerFieldModel() = 0;
	virtual void HandleControlInput(const E_TNControlType controlType) = 0;
	virtual FTNUpdateFieldView& GetUpdateFieldViewDelegate() = 0;
};