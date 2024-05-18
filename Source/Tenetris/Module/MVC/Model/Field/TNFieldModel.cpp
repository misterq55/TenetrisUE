#include "TNFieldModel.h"
#include "Tenetris/Module/MVC/Model/Field/Tetromino/TNTetrominoBase.h"

FTNFieldModel::FTNFieldModel(FTNFieldInfo fieldInfo)
	: FieldInfo(MoveTemp(fieldInfo))
	, FieldActor(nullptr)
	, PreviewTetrominoNum(0)
{

}

void FTNFieldModel::Tick(float deltaSeconds)
{
	if (bLineDeleting)
	{
		for (const int32 deleteLine : DeletedLines)
		{
			for (int32 j = 0; j < FieldInfo.BufferWidth; j++)
			{
				SetValueToCheckBuffer(j, deleteLine, E_TNTetrominoType::None);
			}
		}

		if (DeleteLineCheckTime >= 0.5f)
		{
			TArray<int32> lineChecker;
			int32 lineDeleteValue = 0;

			for (int32 i = 0; i < FieldInfo.BufferHeight; i++)
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

			for (int32 i = 0; i < FieldInfo.BufferHeight; i++)
			{
				if (lineChecker[i] != -1)
				{
					for (int32 j = 0; j < FieldInfo.BufferWidth; j++)
					{
						const E_TNTetrominoType value = GetValueFromCheckBuffer(j, i);
						SetValueToCheckBuffer(j, i, E_TNTetrominoType::None);
						
					}
				}
			}

			DeleteLineCheckTime = 0.f;
			bLineDeleting = false;
		}

		DeleteLineCheckTime += deltaSeconds;
	}
}

void FTNFieldModel::SetBufferSize(const int32 bufferHeight, const int32 bufferWidth)
{
	FieldInfo.BufferHeight = bufferHeight;
	FieldInfo.BufferWidth = bufferWidth;

	for (int32 i = 0; i < FieldInfo.BufferHeight * 2 + 2; i++)
	{
		TArray<E_TNTetrominoType> buffer;
		for (int32 j = 0; j < FieldInfo.BufferWidth + 2; j++)
		{
			if (i == 0 || j == 0 || j == FieldInfo.BufferWidth + 2 - 1)
				buffer.Add(E_TNTetrominoType::Obstacle);
			else
				buffer.Add(E_TNTetrominoType::None);
		}

		FieldInfo.CheckBuffer.Add(buffer);
	}
}

E_TNTetrominoType FTNFieldModel::GetValueFromCheckBuffer(const int32 x, const int32 y) const
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = FieldInfo.BufferWidth - x - 1;
	}

	return FieldInfo.CheckBuffer[y + 1][newX + 1];
}

void FTNFieldModel::SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = FieldInfo.BufferWidth - x - 1;
	}

	FieldInfo.CheckBuffer[y + 1][newX + 1] = tetrominoType;
}

bool FTNFieldModel::CheckMino(const int32 x, const int32 y)
{
	if (y < 0 || x < 0) return true;
	if (y >= FieldInfo.BufferHeight || x >= FieldInfo.BufferWidth) return true;

	return GetValueFromCheckBuffer(x, y) != E_TNTetrominoType::None;
}

int32 FTNFieldModel::CalculateGuideMinoHeight(const int32 x, const int32 y)
{
	int32 result = 0;
	int32 height = 0;

	for (int32 i = y; i >= -1; i--)
	{
		if (GetValueFromCheckBuffer(x, i) != E_TNTetrominoType::None)
		{
			height = i;
			break;
		}
	}

	result = y - height - 1;

	return result;
}

void FTNFieldModel::CheckLineDelete(const TArray<int32>& heights)
{
	TArray<int32> linesToDelete;

	// 각 높이에 대해 줄 삭제 여부를 검사합니다.
	for (int32 height : heights)
	{
		if (IsLineDeleted(height))
		{
			linesToDelete.AddUnique(height);
		}
	}

	// 삭제된 줄이 있는 경우 처리합니다.
	if (linesToDelete.Num() > 0)
	{
		HandleLineDeletion(linesToDelete);
	}
}

bool FTNFieldModel::IsLineDeleted(int32 height) const
{
	// 주어진 높이에 대해 해당 줄이 모두 삭제되었는지 확인합니다.
	for (int32 j = 0; j < FieldInfo.BufferWidth; ++j)
	{
		if (GetValueFromCheckBuffer(j, height) == E_TNTetrominoType::None)
		{
			return false; // 하나라도 블록이 없으면 삭제되지 않은 줄입니다.
		}
	}

	return true; // 모든 블록이 존재하여 줄이 삭제되었습니다.
}

void FTNFieldModel::HandleLineDeletion(const TArray<int32>& linesToDelete)
{
	// 삭제된 줄에 대해 처리를 수행합니다.
	for (int32 height : linesToDelete)
	{
		DeletedLines.AddUnique(height);
	}

	// 줄 삭제 플래그를 설정합니다.
	bLineDeleting = true;
}

TArray<TArray<E_TNTetrominoType>>& FTNFieldModel::GetCheckBuffer()
{
	return FieldInfo.CheckBuffer;
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
	}
}

void FTNFieldModel::AddFieldActor(ATNFieldBase* fieldActor)
{
	FieldActor = fieldActor;
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
	setSoftDrop(true);
}

void FTNFieldModel::StopSoftDrop()
{
	setSoftDrop(false);
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
	
}

void FTNFieldModel::ToggleSpaceInversion()
{
	
}

void FTNFieldModel::HardDrop()
{
	if (CurrentTetromino.IsValid())
	{
		CurrentTetromino->HardDrop();
		// doLockDown();
	}
}