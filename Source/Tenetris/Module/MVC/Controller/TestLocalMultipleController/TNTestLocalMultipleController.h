#pragma once

#include "Tenetris/Module/Interface/ITNController.h"

// Test controller for local multiplayer.
// Responsible for the tick loop's phase ordering:
//   1. Decide : a read-only AI stub queries all boards through the provider
//   2. Resolve: model->Tick() (writes into the provider happen internally when locked)
// The provider does not enforce this timing; the phase ordering is purely this controller's job.
class FTNTestLocalMultipleController : public ITNController
{
public:
	FTNTestLocalMultipleController() {}
	virtual ~FTNTestLocalMultipleController() override {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateField(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor) override;
	virtual void StartPlay() override;
	virtual void HandleControlInput(const E_TNControlType controlType) override;

private:
	// Decide phase: a read-only AI stub that queries all boards using only the
	// provider's read API (GetBoardIds/ReadFieldState). It never touches the
	// FieldModelMap (live models). For now, as this only validates the path, it
	// performs a trivial decision/log.
	void runAIReadStub();

private:
	int32 NextFieldModelId = 0;
};



