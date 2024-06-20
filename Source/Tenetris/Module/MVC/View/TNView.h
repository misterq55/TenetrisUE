#include "Tenetris/Module/Interface/ITNView.h"

class FTNView : public ITNView
{
public:
	FTNView() {}
	virtual ~FTNView() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateFieldView(const int32 key);
	virtual void CreateFieldViewWithFieldActor(const int32 key, ATNFieldBase* fieldActor);
	virtual void UpdateFieldView(const int32 modelKey, const E_TNFieldModelStateType state);

private:
	TMap<int32, TSharedPtr<FTNFieldView>> FieldViewMap;
};