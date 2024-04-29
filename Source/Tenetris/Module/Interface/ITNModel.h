#include "Tenetris/TenetrisDefine.h"

class FTNFieldModel;
class ATNFieldBase;

class ITNModel
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldModel(FTNFieldInfo fieldInfo) = 0;
	virtual void CreateFieldModel(FTNFieldInfo fieldInfo, ATNFieldBase* fieldActor) = 0;
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
};