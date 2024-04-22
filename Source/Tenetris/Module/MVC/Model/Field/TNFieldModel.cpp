#include "TNFieldModel.h"

void FTNFieldModel::Tick(float deltaSeconds)
{
	if (bLineDeleting)
	{
		for (const int32 deleteLine : DeletedLines)
		{
			for (int32 j = 0; j < BufferWidth; j++)
			{
				SetValueToCheckBuffer(j, deleteLine, E_TNTetrominoType::None);
			}
		}

		if (DeleteLineCheckTime >= 0.5f)
		{
			TArray<int32> lineChecker;
			int32 lineDeleteValue = 0;

			for (int32 i = 0; i < BufferHeight; i++)
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

			for (int32 i = 0; i < BufferHeight; i++)
			{
				if (lineChecker[i] != -1)
				{
					for (int32 j = 0; j < BufferWidth; j++)
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
	BufferHeight = bufferHeight;
	BufferWidth = bufferWidth;

	for (int32 i = 0; i < BufferHeight * 2 + 2; i++)
	{
		TArray<E_TNTetrominoType> buffer;
		for (int32 j = 0; j < BufferWidth + 2; j++)
		{
			if (i == 0 || j == 0 || j == BufferWidth + 2 - 1)
				buffer.Add(E_TNTetrominoType::Obstacle);
			else
				buffer.Add(E_TNTetrominoType::None);
		}

		CheckBuffer.Add(buffer);
	}
}

E_TNTetrominoType FTNFieldModel::GetValueFromCheckBuffer(const int32 x, const int32 y)
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = BufferWidth - x - 1;
	}

	return CheckBuffer[y + 1][newX + 1];
}

void FTNFieldModel::SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType)
{
	int32 newX = x;

	if (bSpaceInverted)
	{
		newX = BufferWidth - x - 1;
	}

	CheckBuffer[y + 1][newX + 1] = tetrominoType;
}

bool FTNFieldModel::CheckMino(const int32 x, const int32 y)
{
	if (y < 0 || x < 0) return true;
	if (y >= BufferHeight || x >= BufferWidth) return true;

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

void FTNFieldModel::CheckLineDelete(const TArray<int32> heights)
{
	for (const int32& height : heights)
	{
		bool lineDeleted = true;

		for (int32 j = 0; j < BufferWidth; j++)
		{
			if (GetValueFromCheckBuffer(j, height) == E_TNTetrominoType::None)
			{
				lineDeleted = false;
				break;
			}
		}

		if (lineDeleted)
		{
			DeletedLines.AddUnique(height);
		}
	}

	if (DeletedLines.Num())
	{
		bLineDeleting = true;
	}
}

TArray<TArray<E_TNTetrominoType>>& FTNFieldModel::GetCheckBuffer()
{
	return CheckBuffer;
}
