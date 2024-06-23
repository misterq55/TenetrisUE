#include "Tenetris/TenetrisDefine.h"

class FTNFieldView;
class ATNField;

class ITNView
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldViewWithFieldActor(const int32 key, ATNField* fieldActor) = 0;

private:

};