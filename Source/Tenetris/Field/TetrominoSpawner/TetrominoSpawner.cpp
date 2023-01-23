#include "TetrominoSpawner.h"

void FTetrominoSpawner::Initialize()
{
  TetrominoLimitCounter = 7;
  TetrominoArrayResetCounter = 5;

  RefillTetrominoArray(TetrominoArrayResetCounter);
}

ETetrominoType FTetrominoSpawner::GetTop()
{
	ETetrominoType TopType = TetrominoArray[0];
  TetrominoArray.RemoveAt(0);

  if (TetrominoArray.Num() <= TetrominoLimitCounter)
  {
    RefillTetrominoArray(TetrominoArrayResetCounter);
  }

	return TopType;
}

ETetrominoType FTetrominoSpawner::GetAt(int32 InIndex)
{
	return TetrominoArray[InIndex];
}

void FTetrominoSpawner::ShuffleTetrominoBag(TArray<ETetrominoType>& InBag)
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

void FTetrominoSpawner::RefillTetrominoArray(int32 InTetrominoArrayResetCounter)
{
  for (int32 i = 0; i < InTetrominoArrayResetCounter; i++)
  {
    /*TArray<ETetrominoType> Bag = { ETetrominoType::I, ETetrominoType::J, ETetrominoType::L, ETetrominoType::O, ETetrominoType::S, ETetrominoType::T, ETetrominoType::Z };
    ShuffleTetrominoBag(Bag);*/
    TArray<ETetrominoType> Bag = { ETetrominoType::I, ETetrominoType::I, ETetrominoType::I, ETetrominoType::I, ETetrominoType::I, ETetrominoType::I, ETetrominoType::I };
    ShuffleTetrominoBag(Bag);
    TetrominoArray.Append(Bag);
  }
}
