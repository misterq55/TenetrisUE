#include "Tenetris/TenetrisDefine.h"

class UWorld;
class UClass;
class ATNFieldBase;

class FTNFieldView
{
public:
	void SetFieldActor(TObjectPtr<ATNFieldBase> fieldActor);
	void CreateFieldActor(TSubclassOf<ATNFieldBase> fieldActorClass, UWorld* world);
	void SetBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType);

private:
	TObjectPtr<ATNFieldBase> FieldActor = nullptr;
};