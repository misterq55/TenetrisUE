#include "../../Interface/ITNController.h"

class FTNController : public ITNController
{
public:
	FTNController() {}
	virtual ~FTNController() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void StartMoveLeft() override;
	virtual void StopMoveLeft() override;

	virtual void StartMoveRight() override;
	virtual void StopMoveRight() override;

	virtual void StartSoftDrop() override;
	virtual void StopSoftDrop() override;

	virtual void RotateClockWise() override;
	virtual void RotateCounterClockWise() override;

	virtual void Hold() override;
	virtual void ToggleSpaceInversion() override;
	virtual void HardDrop() override;
private:

};