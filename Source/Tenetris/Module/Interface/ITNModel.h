
class ATNFieldBase;

class ITNModel
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldModel() = 0;
	virtual void CreateFieldModel(ATNFieldBase* fieldActor) = 0;
};