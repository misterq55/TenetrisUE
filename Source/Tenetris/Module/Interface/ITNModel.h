#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTNFieldModel;
class ATNFieldBase;

DECLARE_DELEGATE_OneParam(FTNCreateFieldView, const int32);
DECLARE_DELEGATE_TwoParams(FTNCreateFieldViewWithFieldActor, const int32, ATNFieldBase* field);
DECLARE_DELEGATE_ThreeParams(FTNUpdateFieldView, const int32, const FTNFieldContext&, const E_TNFieldModelStateType state);

class ITNModel
{
public:
	virtual ~ITNModel() {}
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldModel(FTNFieldContext fieldContext, int32 height, int32 width) = 0;
	virtual void CreateFieldModel(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor) = 0;
	virtual void UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state) = 0;
	virtual FTNFieldContext GetFieldContext(const int32 modelKey) = 0;
	virtual TSharedPtr<FTNFieldModel> GetPlayerFieldModel() = 0;
	virtual void HandleControlInput(const E_TNControlType controlType) = 0;
	virtual FTNCreateFieldView& GetCreateFieldViewDelegate() = 0;
	virtual FTNCreateFieldViewWithFieldActor& GetCreateFieldViewWithFieldActorDelegate() = 0;
	virtual FTNUpdateFieldView& GetUpdateFieldViewDelegate() = 0;
};