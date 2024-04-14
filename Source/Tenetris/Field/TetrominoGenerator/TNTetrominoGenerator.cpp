#include "TNTetrominoGenerator.h"

void FTNTetrominoGenerator::Initialize()
{
  TetrominoLimitCounter = 7;
  TetrominoArrayResetCounter = 5;

  refillTetrominoArray(TetrominoArrayResetCounter);
}

E_TNTetrominoType FTNTetrominoGenerator::GetTop()
{
	const E_TNTetrominoType topType = TetrominoArray[0];
  TetrominoArray.RemoveAt(0);

  if (TetrominoArray.Num() <= TetrominoLimitCounter)
  {
    refillTetrominoArray(TetrominoArrayResetCounter);
  }

	return topType;
}

E_TNTetrominoType FTNTetrominoGenerator::GetAt(const int32 index)
{
  if (!TetrominoArray.IsValidIndex(index))
  {
    return E_TNTetrominoType::None;
  }

	return TetrominoArray[index];
}

void FTNTetrominoGenerator::shuffleTetrominoBag(TArray<E_TNTetrominoType>& bag)
{
  const int32 lastIndex = bag.Num() - 1;
  for (int32 i = 0; i < lastIndex; ++i)
  {
    const int32 index = FMath::RandRange(0, lastIndex);
    if (i != index)
    {
      bag.Swap(i, index);
    }
  }
}

void FTNTetrominoGenerator::refillTetrominoArray(const int32 tetrominoArrayResetCounter)
{
  for (int32 i = 0; i < tetrominoArrayResetCounter; i++)
  {
    TArray<E_TNTetrominoType> bag = { E_TNTetrominoType::I, E_TNTetrominoType::J, E_TNTetrominoType::L, E_TNTetrominoType::O, E_TNTetrominoType::S, E_TNTetrominoType::T, E_TNTetrominoType::Z };
    shuffleTetrominoBag(bag);
    TetrominoArray.Append(bag);
  }
}
