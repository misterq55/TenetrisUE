#include "Tenetris/Module/Interface/ITNModel.h"


class FTNModel : public ITNModel
{
public:
	FTNModel() {}
	virtual ~FTNModel() {}
	virtual void Init() override;
	virtual void Tick(float deltaTime) override;
	virtual void CreateFieldModel(FTNFieldContext fieldContext) override;
	virtual void CreateFieldModel(FTNFieldContext fieldContext, ATNField* fieldActor) override;
	virtual void UpdateModel(const int32 modelKey, const E_TNFieldModelStateType state) override;
	virtual TSharedPtr<FTNFieldModel> GetPlayerFieldModel() override;

	virtual void StartMoveLeft() override;
	virtual void StopMoveLeft() override;

	virtual void StartMoveRight() override;
	virtual void StopMoveRight() override;

	virtual void StartSoftDrop() override;
	virtual void StopSoftDrop() override;

	virtual void RotateClockWise() override;
	virtual void RotateCounterClockWise() override;

	virtual void Hold() override;
	virtual void ToggleSpaceInversion() override;
	virtual void HardDrop() override;

	virtual FTNCreateFieldViewWithFieldActor& GetCreateFieldViewWithFieldActorDelegate() { 
		return CreateFieldViewWithFieldActorDelegate; 
	}

	virtual FTNUpdateFieldView& GetUpdateFieldViewDelegate() {
		return UpdateFieldViewDelegate;
	}

	virtual TMap<int32, TSharedPtr<FTNFieldModel>>& GetFieldModelMap() {
		return FieldModelMap;
	}

private:
	TMap<int32, TSharedPtr<FTNFieldModel>> FieldModelMap;
	TSharedPtr<FTNFieldModel> PlayerFieldModel;
	int32 ModelKey = 0;

	FTNCreateFieldViewWithFieldActor CreateFieldViewWithFieldActorDelegate;
	FTNUpdateFieldView UpdateFieldViewDelegate;
};