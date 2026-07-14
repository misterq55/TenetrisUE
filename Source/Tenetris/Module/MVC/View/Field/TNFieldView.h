#pragma once

#include "Tenetris/TenetrisDefine.h"

class UWorld;
class ATNField;
struct FTNFieldContext;

class FTNFieldView
{
public:
	void SetFieldActor(ATNField* fieldActor);
	void CreateFieldActor(TSubclassOf<ATNField> fieldActorClass, UWorld* world);
	void Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) const;

private:
	TWeakObjectPtr<ATNField> FieldActor;
};
