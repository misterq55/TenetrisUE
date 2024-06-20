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
	void SetMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityMino(const int32 x, const int32 y, const bool visible);
	void SetVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible);

private:
	TObjectPtr<ATNFieldBase> FieldActor = nullptr;
};