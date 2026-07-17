#pragma once

#include "Tenetris/TenetrisDefine.h"

class FTNFieldModel;
class ATNFieldBase;

struct FTNFieldContext;

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

	virtual void StartMoveLeft() = 0;
	virtual void StopMoveLeft() = 0;

	virtual void StartMoveRight() = 0;
	virtual void StopMoveRight() = 0;

	virtual void StartSoftDrop() = 0;
	virtual void StopSoftDrop() = 0;

	virtual void RotateClockWise() = 0;
	virtual void RotateCounterClockWise() = 0;

	virtual void Hold() = 0;
	virtual void ToggleSpaceInversion() = 0;
	virtual void HardDrop() = 0;

	virtual FTNCreateFieldViewWithFieldActor& GetCreateFieldViewWithFieldActorDelegate() = 0;
	virtual FTNUpdateFieldView& GetUpdateFieldViewDelegate() = 0;
};