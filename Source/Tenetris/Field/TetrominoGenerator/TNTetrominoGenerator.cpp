#include "TNTetrominoGenerator.h"

void FTNTetrominoGenerator::Initialize()
{
  TetrominoLimitCounter = 7;
  TetrominoArrayResetCounter = 5;

  RefillTetrominoArray(TetrominoArrayResetCounter);
}

E_TNTetrominoType FTNTetrominoGenerator::GetTop()
{
	E_TNTetrominoType TopType = TetrominoArray[0];
  TetrominoArray.RemoveAt(0);

  if (TetrominoArray.Num() <= TetrominoLimitCounter)
  {
    RefillTetrominoArray(TetrominoArrayResetCounter);
  }

	return TopType;
}

E_TNTetrominoType FTNTetrominoGenerator::GetAt(int32 InIndex)
{
	return TetrominoArray[InIndex];
}

void FTNTetrominoGenerator::ShuffleTetrominoBag(TArray<E_TNTetrominoType>& bag)
{
  const int32 LastIndex = bag.Num() - 1;
  for (int32 i = 0; i < LastIndex; ++i)
  {
    int32 Index = FMath::RandRange(0, LastIndex);
    if (i != Index)
    {
      bag.Swap(i, Index);
    }
  }
}

void FTNTetrominoGenerator::RefillTetrominoArray(int32 tetrominoArrayResetCounter)
{
  for (int32 i = 0; i < tetrominoArrayResetCounter; i++)
  {
    TArray<E_TNTetrominoType> Bag = { E_TNTetrominoType::I, E_TNTetrominoType::J, E_TNTetrominoType::L, E_TNTetrominoType::O, E_TNTetrominoType::S, E_TNTetrominoType::T, E_TNTetrominoType::Z };
    ShuffleTetrominoBag(Bag);
    TetrominoArray.Append(Bag);
  }
}
