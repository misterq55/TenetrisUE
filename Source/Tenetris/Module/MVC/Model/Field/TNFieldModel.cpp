#include "TNFieldModel.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetromino.h"
#include "Tenetris/Module/MVC/Model/Field/LockDown/TNLockDown.h"

// TODO 테트로미노 리스트 분리 [05/21/2024]
#include "Tenetris/Module/MVC/Model/Field/TetrominoGenerator/TNTetrominoGenerator.h"

FTNFieldModel::FTNFieldModel(FTNFieldContext fieldContext)
	: FieldContext(MoveTemp(fieldContext))
{
	if (!CurrentTetromino.IsValid())
	{
		// TODO 테트로미노를 DI할 수 있도록 해야한다 [05/21/2024]
		switch (FieldContext.FieldType)
		{
		case E_TNFieldType::Player:
		{
			CurrentTetromino = MakeShareable(new FTNTetromino(FieldContext.PlayerTetrominoInfo));

			CurrentTetromino->OnMoveTetrominoToCheckBuffer.BindRaw(this, &FTNFieldModel::setValueToCheckBuffer);
			CurrentTetromino->OnCheckMino.BindRaw(this, &FTNFieldModel::checkMino);
			CurrentTetromino->OnCalculateGuideMino.BindRaw(this, &FTNFieldModel::calculateGuideMinoHeight);
			CurrentTetromino->OnUpdateTetromino.BindRaw(this, &FTNFieldModel::updateTetromino);
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
	
	if (!LockDown.IsValid())
	{
		LockDown = MakeShareable(new FTNLockDown());
	}
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

void FTNFieldModel::Tick(float deltaTime)
{
	tetrominoFall(deltaTime);
	setMoveState(deltaTime, LeftDirectionState, E_TNTetrominoDirection::Left);
	setMoveState(deltaTime, RightDirectionState, E_TNTetrominoDirection::Right);
	updateLockDown(deltaTime);
	waitForSpawn();
	updateLineDelete(deltaTime);
}

int32 FTNFieldModel::GetId() const
{
	return Id;
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
	if (isSpaceInverting())
	{
		return;
	}

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
	if (isSpaceInverting())
	{
		return;
	}
	
	if (!bCanHold || !CurrentTetromino.IsValid())
	{
		return;
	}

	const E_TNTetrominoType holdTetrominoType = FieldContext.HoldTetrominoInfo->CurrentType;
	const E_TNTetrominoType currentTetrominoType = CurrentTetromino->GetTetrominoType();
	
	FieldContext.HoldTetrominoInfo->ApplyTetrominoType(currentTetrominoType);
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::UpdateHoldTetromino);

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
		updatePreviewTetrominoes();
	}

	bCanHold = false;
}

void FTNFieldModel::ToggleSpaceInversion()
{
	if (isSpaceInverting())
	{
		return;
	}

	FieldContext.bSpaceInverted = !FieldContext.bSpaceInverted;
	RotationPauseRemainingTime = RotationDuration;
	CurrentTime = 0.f;
	
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->ResetGuideTetromino();
	}
	
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::RotateField);
}

void FTNFieldModel::HardDrop()
{
	if (isSpaceInverting())
	{
		return;
	}

	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->HardDrop();
		doLockDown();
	}
}

E_TNTetrominoType FTNFieldModel::getValueFromCheckBuffer(const int32 x, const int32 y) const
{
	const TArray<TArray<E_TNTetrominoType>>& bufferToUse = FieldContext.bSpaceInverted ? FieldContext.ReversedBuffer : FieldContext.CheckBuffer;
	
	return bufferToUse[y + 1][x + 1];
}

void FTNFieldModel::setValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	TArray<TArray<E_TNTetrominoType>>& currentBuffer = FieldContext.bSpaceInverted ? FieldContext.ReversedBuffer : FieldContext.CheckBuffer;
	TArray<TArray<E_TNTetrominoType>>& otherBuffer = FieldContext.bSpaceInverted ? FieldContext.CheckBuffer : FieldContext.ReversedBuffer;
	
	currentBuffer[y + 1][x + 1] = tetrominoType;
	otherBuffer[y + 1][FieldContext.BufferWidth - x] = tetrominoType;
}

bool FTNFieldModel::checkMino(const int32 x, const int32 y) const
{
	if (y < 0 || x < 0) return true;
	if (y >= FieldContext.BufferHeight || x >= FieldContext.BufferWidth) return true;

	return getValueFromCheckBuffer(x, y) != E_TNTetrominoType::None;
}

int32 FTNFieldModel::calculateGuideMinoHeight(const int32 x, const int32 y) const
{
	int32 height = 0;

	for (int32 i = y; i >= -1; i--)
	{
		if (getValueFromCheckBuffer(x, i) != E_TNTetrominoType::None)
		{
			height = i;
			break;
		}
	}

	return y - height - 1;
}

void FTNFieldModel::updateTetromino() const
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::UpdateTetromino);
}

void FTNFieldModel::checkLineDelete(const TArray<int32>& heights)
{
	TArray<int32> linesToDelete;

	// 각 높이에 대해 삭제 될 줄 여부를 검사합니다.
	for (int32 height : heights)
	{
		if (isLineDeleted(height))
		{
			linesToDelete.AddUnique(height);
		}
	}

	// 삭제될 줄이 있는 경우 처리합니다.
	if (linesToDelete.Num() > 0)
	{
		handleLineDeletion(linesToDelete);
	}
}

bool FTNFieldModel::isLineDeleted(int32 height) const
{
	// 주어진 높이에 대해 해당 칸이 모두 채워져있는지 확인합니다.
	for (int32 j = 0; j < FieldContext.BufferWidth; ++j)
	{
		if (getValueFromCheckBuffer(j, height) == E_TNTetrominoType::None)
		{
			return false; // 하나의 칸이라도 비어있으면 완성된 줄이 아닙니다.
		}
	}

	return true; // 모든 칸이 채워져 줄이 완성되었습니다.
}

void FTNFieldModel::spawn()
{
	CurrentTime = 0.f;
	spawnNextTetromino();
	updatePreviewTetrominoes();
	bCanHold = true;
}

bool FTNFieldModel::moveTetromino(E_TNTetrominoDirection tetrominoDirection)
{
	if (isSpaceInverting())
	{
		return false;
	}
	
	if (CurrentTetromino.IsValid())
	{
		if (!CurrentTetromino->Move(tetrominoDirection))
		{
			if (LockDown.IsValid())
			{
				LockDown->CheckRemainCount(tetrominoDirection);
			}

			return false;
		}
	}

	return true;
}

void FTNFieldModel::rotateTetromino(E_TNTetrominoRotation tetrominoRotation)
{
	if (isSpaceInverting())
	{
		return;
	}
	
	if (CurrentTetromino.IsValid())
	{
		if (!CurrentTetromino->Rotate(tetrominoRotation))
		{
			if (LockDown.IsValid())
			{
				LockDown->CheckRemainCount();
			}
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

	if (isSpaceInverting())
	{
		RotationPauseRemainingTime -= deltaTime;
		if (RotationPauseRemainingTime < 0.f)
		{
			RotationPauseRemainingTime = 0.f;
		}

		return;
	}

	CurrentTime += deltaTime;
	if (CurrentTime >= getFallingSpeed())
	{
		if (moveTetromino(E_TNTetrominoDirection::Down))
		{
			if (LockDown.IsValid())
			{
				LockDown->StartLockDown();
			}
		}

		CurrentTime = 0.f;
	}
}

void FTNFieldModel::setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirection)
{
	if (bWaitForSpawn)
	{
		return;
	}
	
	if (isSpaceInverting())
	{
		return;
	}

	if (moveState.Pressed && TetrominoMoveDirection == tetrominoDirection)
	{
		if (moveState.PressedTime == 0.f)
		{
			moveTetromino(tetrominoDirection);
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
			moveTetromino(tetrominoDirection);
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

	if (LockDown.IsValid() && LockDown->UpdateLockDown(deltaTime))
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
				setValueToCheckBuffer(j, deleteLine, E_TNTetrominoType::None);
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
						const E_TNTetrominoType value = getValueFromCheckBuffer(j, i);
						setValueToCheckBuffer(j, i - lineChecValue, value);
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
		checkLineDelete(CurrentTetromino->GetMinoHeights());
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

void FTNFieldModel::handleLineDeletion(const TArray<int32>& linesToDelete)
{
	// 삭제될 줄을 처리 목록에 추가합니다.
	for (int32 height : linesToDelete)
	{
		DeletedLines.AddUnique(height);
	}

	// 줄 삭제 플래그를 설정합니다.
	bLineDeleting = true;
}

void FTNFieldModel::spawnNextTetromino() const
{
	if (CurrentTetromino.IsValid() && TetrominoGenerator.IsValid())
	{
		CurrentTetromino->SetTetrominoType(TetrominoGenerator->GetTop());
		CurrentTetromino->Spawn();
	}
}

void FTNFieldModel::updateHoldTetromino()
{
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::UpdateHoldTetromino);
}

void FTNFieldModel::updatePreviewTetrominoes()
{
	if (!TetrominoGenerator.IsValid())
	{
		return;
	}
	
	for (int32 i = 0; i < FieldContext.PreviewTetrominoNum; i++)
	{
		FieldContext.PreviewTetrominoInfos[i]->ApplyTetrominoType(TetrominoGenerator->GetAt(i));
	}
	
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::UpdatePreviewTetrominoes);
}

float FTNFieldModel::getFallingSpeed() const
{
	float multiplier = 1.f;

	if (bSoftDrop)
	{
		multiplier /= 20.f;
	}

	return TetrominoFallingSpeed * multiplier;
}
