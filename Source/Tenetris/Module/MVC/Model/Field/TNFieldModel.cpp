#include "TNFieldModel.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetromino.h"
#include "Tenetris/Module/MVC/Model/Field/LockDown/TNLockDown.h"
#include "Tenetris/Module/MVC/Model/Field/Recorder/TNRecorder.h"

// TODO 테트로미노 리스트 분리 [05/21/2024]
#include "Tenetris/Module/MVC/Model/Field/TetrominoGenerator/TNTetrominoGenerator.h"

FTNFieldModel::FTNFieldModel(FTNFieldContext fieldContext, int32 height, int32 width)
	: FieldContext(MoveTemp(fieldContext))
	, Height(height)
	, Width(width)
{
	initializeBuffers();

	if (!CurrentTetromino.IsValid())
	{
		// TODO 테트로미노를 DI할 수 있도록 해야한다 [05/21/2024]
		switch (FieldContext.FieldType)
		{
		case E_TNFieldType::Player:
		{
			CurrentTetromino = MakeShareable(new FTNTetromino(FieldContext.TetrominoInfo));

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
	
	if (!Recorder.IsValid())
	{
		Recorder = MakeShareable(new FTNRecorder());
		Recorder->Initialize();
	}
}

void FTNFieldModel::Initialize()
{
	if (!TetrominoGenerator.IsValid())
	{
		TetrominoGenerator->Initialize();
	}
	
	if (!Recorder.IsValid())
	{
		Recorder->Initialize();
	}
	
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->SetStartingLocation(StartingPosition);
	}
}

void FTNFieldModel::SetId(const int32 id)
{
	Id = id;
}

void FTNFieldModel::StartPlay()
{
	spawn();
}

void FTNFieldModel::Tick(float deltaTime)
{
	if (bRewind)
	{
		if (!Recorder.IsValid())
		{
			return;
		}
		
		// TODO 되감기 로직[07/21/2026]
		if (!Recorder->IsEmpty())
		{
			CurrentTime += deltaTime;
			if (CurrentTime >= RewindSpeed)
			{
				const FTNBehavior behavior = Recorder->ConsumeLastBehavior();
				
				switch (behavior.BehaviorState)
				{
				case E_TNBehaviorState::Spawn:
					{
						Recorder->PopFieldRecord();
					}
					break;
					
				case E_TNBehaviorState::Transform:
					{
						const FVector2D position = behavior.Position;
						const int32 rotationState = behavior.RotationState;
						
						if (CurrentTetromino.IsValid())
						{
							CurrentTetromino->SetPosition(position);
							CurrentTetromino->SetRotationState(rotationState);
						}
						
						OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::UpdateTetromino);
					}
					break;
					
				case E_TNBehaviorState::RotateField:
					{
						const bool bRotateField = behavior.bRotateField;
						rotateField();
						
						OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::RotateField);
					}
					break;
					
				case E_TNBehaviorState::Hold:
					{
						const bool bSavedCanHold = behavior.bCanHold;
						const E_TNTetrominoType currentTetrominoType = Recorder->ConsumeTetrominoType();
						const E_TNTetrominoType holdTetrominoType = behavior.HoldTetrominoType;
						
						OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::UpdateHoldTetromino);
					}
					break;
				
				case E_TNBehaviorState::LockDown:
					{
						const E_TNTetrominoType tetrominoType = Recorder->ConsumeTetrominoType();
						const TArray<TArray<FTNCellInfo>> normalBuffer = Recorder->ConsumeNormalBuffer();
						const TArray<TArray<FTNCellInfo>> reversedBuffer = Recorder->ConsumeReversedBuffer();
						
						if (CurrentTetromino.IsValid())
						{
							CurrentTetromino->ApplyTetrominoType(tetrominoType);
						}
						
						CheckBuffer = normalBuffer;
						ReversedBuffer = reversedBuffer;
						
						OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::LockDown);
					}
					break;
				default:
					break;
				}
				
				CurrentTime = 0.f;
			}
		}
		else
		{
			Recorder->Initialize();
			bRewind = false;
		}
	}
	else
	{
		tetrominoFall(deltaTime);
		setMoveState(deltaTime, LeftDirectionState, E_TNTetrominoDirection::Left);
		setMoveState(deltaTime, RightDirectionState, E_TNTetrominoDirection::Right);
		updateLockDown(deltaTime);
		waitForSpawn();
		updateLineDelete(deltaTime);	
	}
}

int32 FTNFieldModel::GetId() const
{
	return Id;
}

void FTNFieldModel::HandleControlInput(const E_TNControlType controlType)
{
	if (bRewind)
	{
		return;
	}
	
	switch (controlType)
	{
	case E_TNControlType::StartMoveLeft:
		startMoveLeft();
		break;
	case E_TNControlType::StopMoveLeft:
		stopMoveLeft();
		break;
	case E_TNControlType::StartMoveRight:
		startMoveRight();
		break;
	case E_TNControlType::StopMoveRight:
		stopMoveRight();
		break;
	case E_TNControlType::StartSoftDrop:
		startSoftDrop();
		break;
	case E_TNControlType::StopSoftDrop:
		stopSoftDrop();
		break;
	case E_TNControlType::RotateClockWise:
		rotateClockWise();
		break;
	case E_TNControlType::RotateCounterClockWise:
		rotateCounterClockWise();
		break;
	case E_TNControlType::Hold:
		hold();
		break;
	case E_TNControlType::RotateField:
		rotateField();
		break;
	case E_TNControlType::Rewind:
		rewind();
		break;
	case E_TNControlType::HardDrop:
		hardDrop();
		break;
	default:
		break;
	}
}

void FTNFieldModel::startMoveLeft()
{
	setMoveDirection(E_TNTetrominoDirection::Left, true);
}

void FTNFieldModel::stopMoveLeft()
{
	setMoveDirection(E_TNTetrominoDirection::Left, false);
}

void FTNFieldModel::startMoveRight()
{
	setMoveDirection(E_TNTetrominoDirection::Right, true);
}

void FTNFieldModel::stopMoveRight()
{
	setMoveDirection(E_TNTetrominoDirection::Right, false);
}

void FTNFieldModel::startSoftDrop()
{
	if (isSpaceInverting())
	{
		return;
	}

	bSoftDrop = true;
}

void FTNFieldModel::stopSoftDrop()
{
	bSoftDrop = false;
}

void FTNFieldModel::rotateClockWise() const
{
	rotateTetromino(E_TNTetrominoRotation::ClockWise);
}

void FTNFieldModel::rotateCounterClockWise() const
{
	rotateTetromino(E_TNTetrominoRotation::CounterClockWise);
}

void FTNFieldModel::hold()
{
	if (isSpaceInverting())
	{
		return;
	}
	
	if (!bCanHold || !CurrentTetromino.IsValid())
	{
		return;
	}

	const E_TNTetrominoType holdTetrominoType = FieldContext.HoldTetrominoType;
	const E_TNTetrominoType currentTetrominoType = CurrentTetromino->GetTetrominoType();
	
	FieldContext.HoldTetrominoType = currentTetrominoType;
	updateHoldTetromino();

	CurrentTime = 0.f;
	
	if (holdTetrominoType != E_TNTetrominoType::None)
	{
		CurrentTetromino->ApplyTetrominoType(holdTetrominoType);
		CurrentTetromino->Spawn();
	}
	else
	{
		spawnNextTetromino();
		updatePreviewTetrominoes();
	}

	bCanHold = false;
	
	if (Recorder.IsValid())
	{
		Recorder->RecordTetrominoType(holdTetrominoType);
		Recorder->RecordHold(bCanHold, currentTetrominoType);
	}
}

void FTNFieldModel::rotateField()
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
	
	if (Recorder.IsValid())
	{
		Recorder->RecordRotateField(FieldContext.bSpaceInverted);
	}
	
	OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::RotateField);
}

void FTNFieldModel::rewind()
{
	bRewind = true;
	CurrentTime = 0.f;
}

void FTNFieldModel::hardDrop()
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

FTNCellInfo FTNFieldModel::getValueFromCheckBuffer(const int32 x, const int32 y) const
{
	const TArray<TArray<FTNCellInfo>>& bufferToUse = FieldContext.bSpaceInverted ? ReversedBuffer : CheckBuffer;
	
	return bufferToUse[y + 1][x + 1];
}

void FTNFieldModel::setValueToCheckBuffer(const int32 x, const int32 y, const FTNCellInfo& cellInfo)
{
	TArray<TArray<FTNCellInfo>>& currentBuffer = FieldContext.bSpaceInverted ? ReversedBuffer : CheckBuffer;
	TArray<TArray<FTNCellInfo>>& otherBuffer = FieldContext.bSpaceInverted ? CheckBuffer : ReversedBuffer;
	
	currentBuffer[y + 1][x + 1] = cellInfo;
	otherBuffer[y + 1][Width - x] = cellInfo;
	
	const int32 lockedGridX = FieldContext.bSpaceInverted ? Width - x : x + 1;
	FieldContext.LockedGrid[y + 1][lockedGridX] = cellInfo;
}

void FTNFieldModel::initializeBuffers()
{
	createBuffer(CheckBuffer);
	createBuffer(ReversedBuffer);
	createBuffer(FieldContext.LockedGrid);
}

void FTNFieldModel::createBuffer(TArray<TArray<FTNCellInfo>>& buffer) const
{
	buffer.Empty();
	buffer.Reserve(Height * 2 + 2);

	for (int32 i = 0; i < Height * 2 + 2; i++)
	{
		TArray<FTNCellInfo> row;
		row.Reserve(Width + 2);
		
		for (int32 j = 0; j < Width + 2; j++)
		{
			if (i == 0 || j == 0 || j == Width + 2 - 1)
			{
				row.Emplace(-1, E_TNTetrominoType::Obstacle);
			}
			else
			{
				row.Emplace(-1, E_TNTetrominoType::None);
			}
		}

		buffer.Add(MoveTemp(row));
	}
}

bool FTNFieldModel::checkMino(const int32 x, const int32 y) const
{
	if (y < 0 || x < 0) return true;
	if (y >= Height || x >= Width) return true;

	return getValueFromCheckBuffer(x, y).Type != E_TNTetrominoType::None;
}

int32 FTNFieldModel::calculateGuideMinoHeight(const int32 x, const int32 y) const
{
	int32 height = 0;

	for (int32 i = y; i >= -1; i--)
	{
		if (getValueFromCheckBuffer(x, i).Type != E_TNTetrominoType::None)
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
	// 각 높이에 대해 삭제 될 줄 여부를 검사합니다.
	for (int32 height : heights)
	{
		if (isLineDeleted(height))
		{
			DeletedLines.AddUnique(height);
		}
	}

	// 삭제될 줄이 있는 경우 처리합니다.
	if (DeletedLines.Num() > 0)
	{
		// 줄 삭제 플래그를 설정합니다.
		bLineDeleting = true;
	}
}

bool FTNFieldModel::isLineDeleted(int32 height) const
{
	// 주어진 높이에 대해 해당 칸이 모두 채워져있는지 확인합니다.
	for (int32 j = 0; j < Width; ++j)
	{
		if (getValueFromCheckBuffer(j, height).Type == E_TNTetrominoType::None)
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
	
	if (Recorder.IsValid())
	{
		Recorder->RecordTetrominoType(CurrentTetromino->GetTetrominoType());
		Recorder->RecordSpawn();
	}
	
	bCanHold = true;
}

bool FTNFieldModel::moveTetromino(E_TNTetrominoDirection tetrominoDirection) const
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
			
			if (Recorder.IsValid())
			{
				Recorder->RecordTransform(CurrentTetromino->GetTetrominoInfo()->Position, CurrentTetromino->GetTetrominoInfo()->RotationState);
			}

			return false;
		}
	}

	return true;
}

void FTNFieldModel::rotateTetromino(E_TNTetrominoRotation tetrominoRotation) const
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
			
			if (Recorder.IsValid())
			{
				Recorder->RecordTransform(CurrentTetromino->GetTetrominoInfo()->Position, CurrentTetromino->GetTetrominoInfo()->RotationState);
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

void FTNFieldModel::setMoveState(float deltaTime, FTNMoveDirectionState& moveState, E_TNTetrominoDirection tetrominoDirection) const
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
			for (int32 j = 0; j < Width; j++)
			{
				setValueToCheckBuffer(j, deleteLine, FTNCellInfo());
			}
		}

		if (DeleteLineCheckTime >= 0.5f)
		{
			TArray<int32> lineChecker;
			int32 lineDeleteValue = 0;

			for (int32 i = 0; i < Height; i++)
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

			for (int32 i = 0; i < Height; i++)
			{
				const int32 lineCheckValue = lineChecker[i];

				if (lineCheckValue != -1)
				{
					for (int32 j = 0; j < Width; j++)
					{
						const FTNCellInfo value = getValueFromCheckBuffer(j, i);
						setValueToCheckBuffer(j, i - lineCheckValue, value);
					}
				}
			}

			DeleteLineCheckTime = 0.f;
			bLineDeleting = false;

			DeletedLines.Empty();
			
			OnUpdateModel.ExecuteIfBound(Id, E_TNFieldModelStateType::LineClear);
		}

		DeleteLineCheckTime += deltaTime;
	}
}

void FTNFieldModel::doLockDown()
{
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->LockDown();
		checkLineDelete(CurrentTetromino->GetMinoHeights());
		bWaitForSpawn = true;

		if (Recorder.IsValid())
		{
			Recorder->RecordBuffers(CheckBuffer, ReversedBuffer);
			Recorder->RecordTransform(CurrentTetromino->GetTetrominoInfo()->Position, CurrentTetromino->GetTetrominoInfo()->RotationState);
		}
		
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

void FTNFieldModel::spawnNextTetromino() const
{
	if (CurrentTetromino.IsValid() && TetrominoGenerator.IsValid())
	{
		CurrentTetromino->ApplyTetrominoType(TetrominoGenerator->GetTop());
		CurrentTetromino->Spawn();
	}
}

void FTNFieldModel::updateHoldTetromino() const
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
		FieldContext.PreviewTetrominoTypes[i] = TetrominoGenerator->GetAt(i);	
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
