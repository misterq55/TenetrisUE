#include "TNTestLocalMultipleController.h"

#include "Tenetris/Module/MVC/View/TestView/TNTestView.h"
#include "Tenetris/Module/MVC/Holder/TNMVCHolder.h"
#include "Tenetris/Module/MVC/Model/TestLocalMultipleModel/TNTestLocalMultipleModel.h"

void FTNTestLocalMultipleController::Init()
{
	NextFieldModelId = 0;

	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	TSharedPtr<ITNView> tnView = FTNMVCHolder::GetInstance().GetView();

	if (tnModel.IsValid() && tnView.IsValid())
	{
		tnModel->Init();
		tnView->Init();
		tnModel->GetUpdateFieldViewDelegate().BindSP(tnView.ToSharedRef(), &ITNView::UpdateFieldView);
	}
}

void FTNTestLocalMultipleController::Tick(float deltaTime)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();

	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	// 1. Decide: the AI reads all boards from the provider (blackboard) (read-only stub)
	runAIReadStub();

	// 2. Resolve: model Tick (writes into the provider happen internally when locked)
	if (tnModel.IsValid())
	{
		tnModel->Tick(deltaTime);
	}
}

void FTNTestLocalMultipleController::CreateField(FTNFieldContext fieldContext, int32 height, int32 width, ATNFieldBase* fieldActor)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	TSharedPtr<ITNView> tnView = holder.GetView();

	if (tnModel.IsValid())
	{
		tnModel->CreateFieldModel(NextFieldModelId, fieldContext, height, width);
	}

	if (tnView.IsValid())
	{
		tnView->CreateFieldView(NextFieldModelId, fieldActor);
	}

	++NextFieldModelId;
}

void FTNTestLocalMultipleController::StartPlay()
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();
	TSharedPtr<ITNView> tnView = holder.GetView();

	if (tnModel.IsValid())
	{
		tnModel->StartPlay();
	}

	if (tnView.IsValid())
	{
		tnView->StartPlay();
	}
}

void FTNTestLocalMultipleController::HandleControlInput(const E_TNControlType controlType)
{
	FTNMVCHolder& holder = FTNMVCHolder::GetInstance();
	TSharedPtr<ITNModel> tnModel = holder.GetModel();

	if (tnModel.IsValid())
	{
		tnModel->HandleControlInput(controlType);
	}
}

void FTNTestLocalMultipleController::runAIReadStub()
{
	TSharedPtr<ITNModel> tnModel = FTNMVCHolder::GetInstance().GetModel();
	if (!tnModel.IsValid())
	{
		return;
	}

	// This controller is paired with the LocalMultiple model. Downcast to access the provider.
	TSharedPtr<FTNTestLocalMultipleModel> localMultipleModel = StaticCastSharedPtr<FTNTestLocalMultipleModel>(tnModel);
	if (!localMultipleModel.IsValid())
	{
		return;
	}

	const FTNFieldStateProvider& provider = localMultipleModel->GetFieldStateProvider();

	// Iterate all boards using only the provider's read API (no direct FieldModelMap access).
	// Trivial decision example: pick the board with the most confirmed cells as the target.
	int32 targetBoardId = INDEX_NONE;
	int32 maxFilledCells = -1;

	const TArray<int32> boardIds = provider.GetBoardIds();
	for (int32 boardId : boardIds)
	{
		const TArray<TArray<FTNCellInfo>>* fieldBuffer = provider.ReadFieldState(boardId);
		if (fieldBuffer == nullptr)
		{
			continue;
		}

		int32 filledCells = 0;
		for (const TArray<FTNCellInfo>& row : *fieldBuffer)
		{
			for (const FTNCellInfo& cell : row)
			{
				// Count only confirmed mino cells, excluding borders (Obstacle) and empty cells (None).
				if (cell.Type != E_TNTetrominoType::None && cell.Type != E_TNTetrominoType::Obstacle)
				{
					++filledCells;
				}
			}
		}

		if (filledCells > maxFilledCells)
		{
			maxFilledCells = filledCells;
			targetBoardId = boardId;
		}
	}

	// Path-validation log. Actual actions (driving enemy boards/attacks) are out of scope for the next step.
	UE_LOG(LogTemp, Verbose, TEXT("[AIReadStub] boards=%d, targetBoardId=%d, filledCells=%d"),
		boardIds.Num(), targetBoardId, maxFilledCells);
}



