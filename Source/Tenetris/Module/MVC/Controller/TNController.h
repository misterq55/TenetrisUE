#pragma once

#include "Tenetris/Module/Interface/ITNController.h"

class FTNController : public ITNController
{
public:
	FTNController() {}
	virtual ~FTNController() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void HandleControlInput(E_TNControlType controlType) override;
private:

};