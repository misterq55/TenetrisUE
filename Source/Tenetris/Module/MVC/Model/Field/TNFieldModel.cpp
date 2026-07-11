#include "TNFieldModel.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetrominoBase.h"

// TODO ��Ʈ�ι̳� ���丮�� �и� [05/21/2024]
#include "Tenetris/Module/MVC/Model/Field/Tetromino/PlayerTetromino/TNPlayerTetromino.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/PreviewTetromino/TNPreviewTetromino.h"
#include "Tenetris/Module/MVC/Model/Field/TetrominoGenerator/TNTetrominoGenerator.h"

FTNFieldModel::FTNFieldModel(FTNFieldContext fieldContext)
	: FieldContext(MoveTemp(fieldContext))
{
	if (!CurrentTetromino.IsValid())
	{
		// TODO ��Ʈ�ι̳븦 di�� �� �ֵ��� �����ؾ��� [05/21/2024]
		switch (FieldContext.FieldType)
		{
		case E_TNFieldType::Player:
		{
			CurrentTetromino = MakeShareable(new FTNPlayerTetromino(FieldContext.PlayerTetrominoInfo));

			CurrentTetromino->OnBackgroundCubeType.BindRaw(this, &FTNFieldModel::SetValueToCheckBuffer);
			CurrentTetromino->OnCheckMino.BindRaw(this, &FTNFieldModel::CheckMino);
			CurrentTetromino->OnCalulateGuideMino.BindRaw(this, &FTNFieldModel::CalculateGuideMinoHeight);
			
			CurrentTetromino->OnHideTetromino.BindRaw(this, &FTNFieldModel::HideTetromino);
			CurrentTetromino->OnSetTetromino.BindRaw(this, &FTNFieldModel::SetTetromino);

			CurrentTetromino->OnHideGuideTetromino.BindRaw(this, &FTNFieldModel::HideGuideTetromino);
			CurrentTetromino->OnSetGuideTetromino.BindRaw(this, &FTNFieldModel::SetGuideTetromino);
		}
			break;
		default:
			break;
		}
	}

	if (!TetrominoGenerator.IsValid())
	{
		TetrominoGenerator = MakeShareable(new FTNTetrominoGenerator());
		TetrominoGenerator->Initialize();
	}

	initializePreviewTetrominos();
	initializeHoldTetromino();
}

void FTNFieldModel::Initialize()
{
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->SetStartingLocation(4, 18);
	}

	spawn();
}

void FTNFieldModel::SetId(const int32 id)
{
	Id = id;
}

int32 FTNFieldModel::GetId() const
{
	return Id;
}

void FTNFieldModel::Tick(float deltaTime)
{
	tetrominoFall(deltaTime);
	setMoveState(deltaTime, LeftDirectionState, E_TNTetrominoDirection::Left);
	setMoveState(deltaTime, RightDirectionState, E_TNTetrominoDirection::Right);
	updateLockDown(deltaTime);
	waitForSpawn();
	updateLineDelete(deltaTime);
}

E_TNTetrominoType FTNFieldModel::GetValueFromCheckBuffer(const int32 x, const int32 y) const
{
	const TArray<TArray<E_TNTetrominoType>>& bufferToUse = FieldContext.bSpaceInverted ? FieldContext.ReversedBuffer : FieldContext.CheckBuffer;
	
	return bufferToUse[y + 1][x + 1];
}

void FTNFieldModel::SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	TArray<TArray<E_TNTetrominoType>>& currentBuffer = FieldContext.bSpaceInverted ? FieldContext.ReversedBuffer : FieldContext.CheckBuffer;
	TArray<TArray<E_TNTetrominoType>>& otherBuffer = FieldContext.bSpaceInverted ? FieldContext.CheckBuffer : FieldContext.ReversedBuffer;
	
	currentBuffer[y + 1][x + 1] = tetrominoType;
	otherBuffer[y + 1][FieldContext.BufferWidth - x] = tetrominoType;
}

bool FTNFieldModel::CheckMino(const int32 x, const int32 y) const
{
	if (y < 0 || x < 0) return true;
	if (y >= FieldContext.BufferHeight || x >= FieldContext.BufferWidth) return true;

	return GetValueFromCheckBuffer(x, y) != E_TNTetrominoType::None;
}

int32 FTNFieldModel::CalculateGuideMinoHeight(const int32 x, const int32 y) const
{
	int32 height = 0;

	for (int32 i = y; i >= -1; i--)
	{
		if (GetValueFromCheckBuffer(x, i) != E_TNTetrominoType::None)
		{
			height = i;
			break;
		}
	}

	return y - height - 1;
}

void FTNFieldModel::HidePreviewTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::HidePreviewTetromino);
}

void FTNFieldModel::SetPreviewTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::SetPreviewTetromino);
}

void FTNFieldModel::HideHoldTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::HideHoldTetromino);
}

void FTNFieldModel::SetHoldTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::SetHoldTetromino);
}

void FTNFieldModel::HideTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::HideTetromino);
}

void FTNFieldModel::SetTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::SetTetromino);
}

void FTNFieldModel::HideGuideTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::HideGuideTetromino);
}

void FTNFieldModel::SetGuideTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::SetGuideTetromino);
}

void FTNFieldModel::CheckLineDelete(const TArray<int32>& heights)
{
	TArray<int32> linesToDelete;

	// �� ���̿� ���� �� ���� ���θ� �˻��մϴ�.
	for (int32 height : heights)
	{
		if (IsLineDeleted(height))
		{
			linesToDelete.AddUnique(height);
		}
	}

	// ������ ���� �ִ� ��� ó���մϴ�.
	if (linesToDelete.Num() > 0)
	{
		HandleLineDeletion(linesToDelete);
	}
}

bool FTNFieldModel::IsLineDeleted(int32 height) const
{
	// �־��� ���̿� ���� �ش� ���� ��� �����Ǿ����� Ȯ���մϴ�.
	for (int32 j = 0; j < FieldContext.BufferWidth; ++j)
	{
		if (GetValueFromCheckBuffer(j, height) == E_TNTetrominoType::None)
		{
			return false; // �ϳ��� ������ ������ �������� ���� ���Դϴ�.
		}
	}

	return true; // ��� ������ �����Ͽ� ���� �����Ǿ����ϴ�.
}

void FTNFieldModel::HandleLineDeletion(const TArray<int32>& linesToDelete)
{
	// ������ �ٿ� ���� ó���� �����մϴ�.
	for (int32 height : linesToDelete)
	{
		DeletedLines.AddUnique(height);
	}

	// �� ���� �÷��׸� �����մϴ�.
	bLineDeleting = true;
}

void FTNFieldModel::spawn()
{
	CurrentTime = 0.f;
	spawnNextTetromino();
	renewPreviewTetromino();
	bCanHold = true;

	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::SetTetromino);
}

bool FTNFieldModel::moveTetromino(E_TNTetrominoDirection tetrominoDirection)
{
	if (CurrentTetromino.IsValid())
	{
		if (!CurrentTetromino->Move(tetrominoDirection))
		{
			LockDown.CheckRemainCount(tetrominoDirection);

			return false;
		}
	}

	return true;
}

void FTNFieldModel::rotateTetromino(E_TNTetrominoRotation tetrominoRotation)
{
	if (CurrentTetromino.IsValid())
	{
		if (!CurrentTetromino->Rotate(tetrominoRotation))
		{
			LockDown.CheckRemainCount();
		}
	}
}

void FTNFieldModel::setMoveDirection(E_TNTetrominoDirection tetrominoDirection, bool pressed)
{
	if (pressed)
	{
		if (tetrominoDirection == E_TNTetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = true;
		}
		else if (tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			RightDirectionState.Pressed = true;
		}

		TetrominoMoveDirection = tetrominoDirection;
	}
	else
	{
		if (tetrominoDirection == E_TNTetrominoDirection::Left)
		{
			LeftDirectionState.Pressed = false;
		}
		else if (tetrominoDirection == E_TNTetrominoDirection::Right)
		{
			RightDirectionState.Pressed = false;
		}

		if (LeftDirectionState.Pressed)
		{
			TetrominoMoveDirection = E_TNTetrominoDirection::Left;
		}

		if (RightDirectionState.Pressed)
		{
			TetrominoMoveDirection = E_TNTetrominoDirection::Right;
		}

		if (!LeftDirectionState.Pressed && !RightDirectionState.Pressed)
		{
			TetrominoMoveDirection = E_TNTetrominoDirection::None;
		}
	}
}

void FTNFieldModel::tetrominoFall(float deltaTime)
{
	if (bWaitForSpawn)
	{
		return;
	}

	CurrentTime += deltaTime;
	if (CurrentTime >= getFallingSpeed())
	{
		if (moveTetromino(E_TNTetrominoDirection::Down))
		{
			LockDown.StartLockDown();
		}

		CurrentTime = 0.f;
	}
}

void FTNFieldModel::setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirction)
{
	if (bWaitForSpawn)
	{
		return;
	}

	if (moveState.Pressed && TetrominoMoveDirection == tetrominoDirction)
	{
		if (moveState.PressedTime == 0.f)
		{
			moveTetromino(tetrominoDirction);
		}
		else if (moveState.PressedTime > KickInDelay)
		{
			moveState.AutoRepeatKickIn = true;
			moveState.PressedTime = KickInDelay;
		}

		moveState.PressedTime += deltaTime;
	}
	else
	{
		moveState.AutoRepeatKickIn = false;
		moveState.PressedTime = 0.f;
	}

	if (moveState.AutoRepeatKickIn)
	{
		if (moveState.PressedTime >= MoveSpeed)
		{
			moveTetromino(tetrominoDirction);
			moveState.PressedTime = 0.f;
		}

		moveState.PressedTime += deltaTime;
	}
}

void FTNFieldModel::updateLockDown(float deltaTime)
{
	if (bWaitForSpawn)
	{
		return;
	}

	if (LockDown.UpdateLockDown(deltaTime))
	{
		doLockDown();
	}
}

void FTNFieldModel::updateLineDelete(float deltaTime)
{
	if (bLineDeleting)
	{
		for (const int32 deleteLine : DeletedLines)
		{
			for (int32 j = 0; j < FieldContext.BufferWidth; j++)
			{
				SetValueToCheckBuffer(j, deleteLine, E_TNTetrominoType::None);
			}
		}

		if (DeleteLineCheckTime >= 0.5f)
		{
			TArray<int32> lineChecker;
			int32 lineDeleteValue = 0;

			for (int32 i = 0; i < FieldContext.BufferHeight; i++)
			{
				if (DeletedLines.Find(i) != -1)
				{
					lineChecker.Add(-1);
					lineDeleteValue++;
				}
				else
				{
					lineChecker.Add(lineDeleteValue);
				}
			}

			for (int32 i = 0; i < FieldContext.BufferHeight; i++)
			{
				int32 lineChecValue = lineChecker[i];

				if (lineChecValue != -1)
				{
					for (int32 j = 0; j < FieldContext.BufferWidth; j++)
					{
						const E_TNTetrominoType value = GetValueFromCheckBuffer(j, i);
						SetValueToCheckBuffer(j, i - lineChecValue, value);
					}
				}
			}

			DeleteLineCheckTime = 0.f;
			bLineDeleting = false;

			DeletedLines.Empty();

			OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::LockDown);
		}

		DeleteLineCheckTime += deltaTime;
	}
}

void FTNFieldModel::lineDelete()
{
	if (CurrentTetromino.IsValid())
	{
		CheckLineDelete(CurrentTetromino->GetMinoHeights());
	}
}

void FTNFieldModel::doLockDown()
{
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->LockDown();
		lineDelete();
		bWaitForSpawn = true;

		OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::LockDown);
	}
}

void FTNFieldModel::waitForSpawn()
{
	if (bWaitForSpawn && !bLineDeleting)
	{
		spawn();
		bWaitForSpawn = false;
	}
}

void FTNFieldModel::spawnNextTetromino()
{
	if (CurrentTetromino.IsValid() && TetrominoGenerator.IsValid())
	{
		CurrentTetromino->SetTetrominoType(TetrominoGenerator->GetTop());
		CurrentTetromino->Spawn();
	}
}

void FTNFieldModel::renewPreviewTetromino()
{
	if (!TetrominoGenerator.IsValid())
	{
		return;
	}

	HidePreviewTetromino();

	for (int32 i = 0; i < PreviewTetrominos.Num(); i++)
	{
		TSharedPtr<FTNTetrominoBase> previewTetromino = PreviewTetrominos[i];
		if (!previewTetromino.IsValid())
		{
			continue;
		}

		previewTetromino->HideTetromino();
		previewTetromino->SetTetrominoType(TetrominoGenerator->GetAt(i));
		previewTetromino->Spawn();
	}
	
	SetPreviewTetromino();
}

float FTNFieldModel::getFallingSpeed() const
{
	float multiflier = 1.f;

	if (bSoftDrop)
	{
		multiflier /= 20.f;
	}

	return TetrominoFallingSpeed * multiflier;
}

void FTNFieldModel::initializePreviewTetrominos()
{
	for (int32 i = 0; i < FieldContext.PreviewTetrominoNum; i++)
	{
		TSharedPtr<FTNTetrominoBase> previewTetromino = MakeShareable(new FTNPreviewTetromino(FieldContext.PreviewTetrominoInfos[i]));
		PreviewTetrominos.Add(previewTetromino);
		previewTetromino->SetStartingLocation(2, (FieldContext.PreviewTetrominoNum - i - 1) * 3 + 1);
	}
}

void FTNFieldModel::initializeHoldTetromino()
{
	HoldTetromino = MakeShareable(new FTNPreviewTetromino(FieldContext.HoldTetrominoInfo));
	HoldTetromino->OnHideTetromino.BindRaw(this, &FTNFieldModel::HideHoldTetromino);
	HoldTetromino->OnSetTetromino.BindRaw(this, &FTNFieldModel::SetHoldTetromino);
	HoldTetromino->SetStartingLocation(2, 1);
}

void FTNFieldModel::StartMoveLeft()
{
	setMoveDirection(E_TNTetrominoDirection::Left, true);
}

void FTNFieldModel::StopMoveLeft()
{
	setMoveDirection(E_TNTetrominoDirection::Left, false);
}

void FTNFieldModel::StartMoveRight()
{
	setMoveDirection(E_TNTetrominoDirection::Right, true);
}

void FTNFieldModel::StopMoveRight()
{
	setMoveDirection(E_TNTetrominoDirection::Right, false);
}

void FTNFieldModel::StartSoftDrop()
{
	bSoftDrop = true;
}

void FTNFieldModel::StopSoftDrop()
{
	bSoftDrop = false;
}

void FTNFieldModel::RotateClockWise()
{
	rotateTetromino(E_TNTetrominoRotation::ClockWise);
}

void FTNFieldModel::RotateCounterClockWise()
{
	rotateTetromino(E_TNTetrominoRotation::CounterClockWise);
}

void FTNFieldModel::Hold()
{
	if (!bCanHold || !HoldTetromino.IsValid() || !CurrentTetromino.IsValid())
	{
		return;
	}

	const E_TNTetrominoType holdTetrominoType = HoldTetromino->GetTetrominoType();
	const E_TNTetrominoType currentTetrominoType = CurrentTetromino->GetTetrominoType();

	CurrentTetromino->HideTetromino();
	CurrentTetromino->HideGuideTetromino();

	HoldTetromino->HideTetromino();
	HoldTetromino->SetTetrominoType(currentTetrominoType);
	HoldTetromino->Spawn();

	if (holdTetrominoType != E_TNTetrominoType::None)
	{
		CurrentTime = 0.f;
		CurrentTetromino->SetTetrominoType(holdTetrominoType);
		CurrentTetromino->Spawn();
	}
	else
	{
		CurrentTime = 0.f;
		spawnNextTetromino();
		renewPreviewTetromino();
	}

	bCanHold = false;
}

void FTNFieldModel::ToggleSpaceInversion()
{
	FieldContext.bSpaceInverted = !FieldContext.bSpaceInverted;
	
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->ResetGuideTetromino();
	}
	
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::RotateField);
}

void FTNFieldModel::HardDrop()
{
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->HardDrop();
		doLockDown();
	}
}
