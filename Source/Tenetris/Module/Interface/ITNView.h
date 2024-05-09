
class FTNFieldView;
class ATNFieldBase;

class ITNView
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void CreateFieldViewWithFieldActor(const int32 key, ATNFieldBase* fieldActor) = 0;

private:

};