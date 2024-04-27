
class ITNController
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
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