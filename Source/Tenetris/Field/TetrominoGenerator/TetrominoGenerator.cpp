#include "TetrominoGenerator.h"

void FTetrominoGenerator::Initialize()
{
  TetrominoLimitCounter = 7;
  TetrominoArrayResetCounter = 5;

  RefillTetrominoArray(TetrominoArrayResetCounter);
}

ETetrominoType FTetrominoGenerator::GetTop()
{
	ETetrominoType TopType = TetrominoArray[0];
  TetrominoArray.RemoveAt(0);

  if (TetrominoArray.Num() <= TetrominoLimitCounter)
  {
    RefillTetrominoArray(TetrominoArrayResetCounter);
  }

	return TopType;
}

ETetrominoType FTetrominoGenerator::GetAt(int32 InIndex)
{
	return TetrominoArray[InIndex];
}

void FTetrominoGenerator::ShuffleTetrominoBag(TArray<ETetrominoType>& InBag)
{
  const int32 LastIndex = InBag.Num() - 1;
  for (int32 i = 0; i < LastIndex; ++i)
  {
    int32 Index = FMath::RandRange(0, LastIndex);
    if (i != Index)
    {
      InBag.Swap(i, Index);
    }
  }
}

void FTetrominoGenerator::RefillTetrominoArray(int32 InTetrominoArrayResetCounter)
{
  for (int32 i = 0; i < InTetrominoArrayResetCounter; i++)
  {
    TArray<ETetrominoType> Bag = { ETetrominoType::I, ETetrominoType::J, ETetrominoType::L, ETetrominoType::O, ETetrominoType::S, ETetrominoType::T, ETetrominoType::Z };
    // ShuffleTetrominoBag(Bag);
    TetrominoArray.Append(Bag);
  }
}
