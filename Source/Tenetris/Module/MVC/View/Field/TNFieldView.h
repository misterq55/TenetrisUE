#pragma once

#include "Tenetris/TenetrisDefine.h"

class UWorld;
class UClass;
class ATNField;
struct FTNFieldContext;

class FTNFieldView
{
public:
	void SetFieldActor(ATNField* fieldActor);
	void CreateFieldActor(TSubclassOf<ATNField> fieldActorClass, UWorld* world);
	void Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) const;
	
private:
	void setBackgroundCubeType(int32 x, int32 y, E_TNTetrominoType tetrominoType) const;
	void setMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType) const;
	void setVisibilityMino(const int32 x, const int32 y, const bool visible) const;
	void setHoldMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType) const;
	void setVisibilityHoldMino(const int32 x, const int32 y, const bool visible) const;
	void setPreviewMinoType(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType) const;
	void setVisibilityPreviewMino(const int32 x, const int32 y, const bool visible) const;
	void setVisibilityBackgroundCube(const int32 x, const int32 y, const bool visible) const;

private:
	TWeakObjectPtr<ATNField> FieldActor;
};
