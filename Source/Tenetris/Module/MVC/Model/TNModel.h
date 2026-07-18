#pragma once

#include "Tenetris/Module/Interface/ITNModel.h"

class FTNModel : public ITNModel
{
public:
	FTNModel() {}
	virtual ~FTNModel() override {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateFieldModel(FTNFieldContext fieldContext, int32 height, int32 width) override;
	virtual void CreateFieldModel(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor) override;
	virtual void UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state) override;
	virtual FTNFieldContext GetFieldContext(const int32 modelKey) override;
	virtual TSharedPtr<FTNFieldModel> GetPlayerFieldModel() override;

	virtual void HandleControlInput(E_TNControlType controlType) override;

	virtual FTNCreateFieldView& GetCreateFieldViewDelegate() override { 
		return CreateFieldViewDelegate; 
	}
	
	virtual FTNCreateFieldViewWithFieldActor& GetCreateFieldViewWithFieldActorDelegate() override { 
		return CreateFieldViewWithFieldActorDelegate; 
	}

	virtual FTNUpdateFieldView& GetUpdateFieldViewDelegate() override {
		return UpdateFieldViewDelegate;
	}

private:
	TMap<int32, TSharedPtr<FTNFieldModel>> FieldModelMap;
	TSharedPtr<FTNFieldModel> PlayerFieldModel;
	int32 ModelKey = 0;

	FTNCreateFieldView CreateFieldViewDelegate;
	FTNCreateFieldViewWithFieldActor CreateFieldViewWithFieldActorDelegate;
	FTNUpdateFieldView UpdateFieldViewDelegate;
};
