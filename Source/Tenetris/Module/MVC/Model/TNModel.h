#include "../../Interface/ITNModel.h"

class FTNFieldModel;
class ATNFieldBase;

class FTNModel : public ITNModel
{
public:
	FTNModel() {}
	virtual ~FTNModel() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateFieldModel() override;
	virtual void CreateFieldModel(ATNFieldBase* fieldActor) override;

private:
	TMap<int32, TSharedPtr<FTNFieldModel>> FieldModelMap;
	int32 ModelKey = 0;
};