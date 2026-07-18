#pragma once

#include "Tenetris/Module/Interface/ITNController.h"

class FTNTestController : public ITNController
{
public:
	FTNTestController() {}
	virtual ~FTNTestController() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void HandleControlInput(E_TNControlType controlType) override;
private:

};

