#include "Tenetris/Module/Interface/ITNView.h"

class FTNFieldView;
class FTNView : public ITNView
{
public:
	FTNView() {}
	virtual ~FTNView() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;

private:
	TMap<int32, TSharedPtr<FTNFieldView>> FieldViewMap;
};