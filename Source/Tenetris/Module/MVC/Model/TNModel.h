#include "../../Interface/ITNModel.h"

class FTNFieldModel;
class FTNModel : public ITNModel
{
private:
	TMap<int32, TSharedPtr<FTNFieldModel>> FieldModelMap;
};