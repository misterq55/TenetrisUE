#include "Tenetris/Module/Interface/ITNView.h"

class FTNView : public ITNView
{
public:
	FTNView() {}
	virtual ~FTNView() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateFieldView(const int32 key) override;
	virtual void CreateFieldViewWithFieldActor(const int32 key, ATNField* fieldActor) override;
	virtual void UpdateFieldView(const int32 modelKey, const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) override;

private:
	TMap<int32, TSharedPtr<FTNFieldView>> FieldViewMap;
};