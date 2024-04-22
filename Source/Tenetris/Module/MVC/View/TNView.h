#include "../../Interface/ITNView.h"

class FTNFieldView;
class FTNView : public ITNView
{
private:
	TMap<int32, TSharedPtr<FTNFieldView>> FieldViewMap;
};