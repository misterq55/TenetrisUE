#pragma once

#include "Tenetris/TenetrisDefine.h"

class ITNController
{
public:
	virtual ~ITNController() {}
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void StartPlay() = 0;
	virtual void HandleControlInput(const E_TNControlType controlType) = 0;
};