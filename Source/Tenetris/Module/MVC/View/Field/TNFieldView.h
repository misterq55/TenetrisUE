#include "Tenetris/TenetrisDefine.h"

class UWorld;
class UClass;
class ATNField;

class FTNFieldView
{
public:
	void SetFieldActor(TObjectPtr<ATNField> fieldActor);
	void CreateFieldActor(TSubclassOf<ATNField> fieldActorClass, UWorld* world);
	void SetBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType);
	void SetMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityMino(const int32 x, const int32 y, const bool visible);
	void SetHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityHoldMino(const int32 x, const int32 y, const bool visible);
	void SetPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	void SetVisibilityPreviewMino(const int32 x, const int32 y, const bool visible);
	void SetVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible);

private:
	TObjectPtr<ATNField> FieldActor = nullptr;
};