#pragma once

#include "Tenetris/Module/Interface/ITNController.h"

class FTNTestController : public ITNController
{
public:
	FTNTestController() {}
	virtual ~FTNTestController() override {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void StartPlay() override;
	virtual void HandleControlInput(const E_TNControlType controlType) override;
private:

};

