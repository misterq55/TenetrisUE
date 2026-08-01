#pragma once

#include "Tenetris/Module/Interface/ITNModel.h"

class FTNTestModel : public ITNModel
{
public:
	FTNTestModel() {}
	virtual ~FTNTestModel() override {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void StartPlay() override;
	virtual void CreateFieldModel(const int32 modelKey, FTNFieldContext fieldContext, int32 height, int32 width) override;
	virtual void UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state) override;
	virtual FTNFieldContext GetFieldContext(const int32 modelKey) override;
	virtual TSharedPtr<FTNFieldModel> GetPlayerFieldModel() override;

	virtual void HandleControlInput(const E_TNControlType controlType) override;

	virtual FTNUpdateFieldView& GetUpdateFieldViewDelegate() override {
		return UpdateFieldViewDelegate;
	}
	
private:
	TMap<int32, TSharedPtr<FTNFieldModel>> FieldModelMap;
	TSharedPtr<FTNFieldModel> PlayerFieldModel;

	FTNUpdateFieldView UpdateFieldViewDelegate;
};

