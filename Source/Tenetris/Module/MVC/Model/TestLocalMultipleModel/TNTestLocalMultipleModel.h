#pragma once

#include "Tenetris/Module/Interface/ITNModel.h"
#include "Tenetris/Module/MVC/Model/Provider/TNFieldStateProvider.h"

// Test model for local multiplayer.
// Has the same field-model management structure as FTNTestModel, but additionally
// owns a single FTNFieldStateProvider (the blackboard) that holds the latest
// confirmed state of multiple boards.
//   - write: each field model writes its confirmed buffer into the provider when locked
//   - read : (the AI) queries all boards only through the provider (no direct FieldModelMap access)
// The view does not reference the provider (existing push rendering is kept: model -> UpdateFieldView).
class FTNTestLocalMultipleModel : public ITNModel
{
public:
	FTNTestLocalMultipleModel() {}
	virtual ~FTNTestLocalMultipleModel() override {}
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

	// Getter for the read consumer (AI) to access the provider. Only this pair
	// (the LocalMultiple controller) uses it. The store may be used through this
	// reference for either read or write, and it never knows who uses it or why.
	FTNFieldStateProvider& GetFieldStateProvider() {
		return FieldStateProvider;
	}

private:
	// Writes the latest confirmed buffer of the field model matching modelKey into the provider.
	void writeFieldStateToProvider(const int32 modelKey);

private:
	TMap<int32, TSharedPtr<FTNFieldModel>> FieldModelMap;
	TSharedPtr<FTNFieldModel> PlayerFieldModel;

	// The single shared lookup store (blackboard) owned at the match/game level.
	FTNFieldStateProvider FieldStateProvider;

	FTNUpdateFieldView UpdateFieldViewDelegate;
};



