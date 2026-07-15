#pragma once

#include "Tenetris/TenetrisDefine.h"

class UWorld;
class ATNFieldBase;
struct FTNFieldContext;

class FTNFieldView
{
public:
	void SetFieldActor(ATNFieldBase* fieldActor);
	void CreateFieldActor(TSubclassOf<ATNFieldBase> fieldActorClass, UWorld* world);
	void Update(const FTNFieldContext& fieldContext, const E_TNFieldModelStateType state) const;

private:
	TWeakObjectPtr<ATNFieldBase> FieldActor;
};
