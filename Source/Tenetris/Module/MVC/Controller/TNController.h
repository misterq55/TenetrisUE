#include "../../Interface/ITNController.h"

class FTNController : public ITNController
{
public:
	FTNController() {}
	virtual ~FTNController() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
private:

};