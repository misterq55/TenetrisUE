#include "../../Util/Singleton/Singleton.h"

class ITNModel;
class ITNView;
class ITNController;

class FTNMVCHolder : public TTNSingleton<FTNMVCHolder>
{
private:
	TSharedPtr<ITNModel> TNModel;
	TSharedPtr<ITNView> TNView;
	TSharedPtr<ITNController> TNController;

public:
	void SetModel(TSharedPtr<ITNModel> tnModel);
	void SetView(TSharedPtr<ITNView> tnView);
	void SetController(TSharedPtr<ITNController> tnController);

	TSharedPtr<ITNModel> GetModel();
	TSharedPtr<ITNView> GetView();
	TSharedPtr<ITNController> GetController();
};