
class UWorld;
class UClass;
class ATNFieldBase;

class FTNFieldView
{
public:
	void SetFieldActor(TObjectPtr<ATNFieldBase> fieldActor);
	void CreateFieldActor(TSubclassOf<ATNFieldBase> fieldActorClass, UWorld* world);

private:
	TObjectPtr<ATNFieldBase> FieldActor = nullptr;
};