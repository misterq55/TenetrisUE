#pragma once

#include "Tenetris/Module/Interface/ITNController.h"

class FTNTestController : public ITNController
{
public:
	FTNTestController() {}
	virtual ~FTNTestController() override {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateField(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor) override;
	virtual void StartPlay() override;
	virtual void HandleControlInput(const E_TNControlType controlType) override;
private:
	int32 NextFieldModelId = 0;
};

