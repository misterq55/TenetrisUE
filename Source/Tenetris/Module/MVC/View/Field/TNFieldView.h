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
	void hideTetromino(const FTNFieldContext& fieldContext) const;
	void showTetromino(const FTNFieldContext& fieldContext) const;
	void hideGuideTetromino(const FTNFieldContext& fieldContext) const;
	void setGuideTetromino(const FTNFieldContext& fieldContext) const;
	void hideHoldTetromino(const FTNFieldContext& fieldContext) const;
	void setHoldTetromino(const FTNFieldContext& fieldContext) const;
	void hidePreviewTetromino(const FTNFieldContext& fieldContext) const;
	void showPreviewTetromino(const FTNFieldContext& fieldContext) const;
	void rotateField(const FTNFieldContext& fieldContext) const;
	void lockDown(const FTNFieldContext& fieldContext) const;

private:
	TWeakObjectPtr<ATNField> FieldActor;
};
