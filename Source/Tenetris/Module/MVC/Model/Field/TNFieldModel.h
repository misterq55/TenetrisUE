#include <Tenetris/TenetrisDefine.h>

class FTNFieldModel
{
public:
	void Tick(float deltaSeconds);
	void SetBufferSize(const int32 bufferHeight, const int32 bufferWidth);
	E_TNTetrominoType GetValueFromCheckBuffer(const int32 x, const int32 y);
	void SetValueToCheckBuffer(const int32 x, const int32 y, const E_TNTetrominoType tetrominoType);
	bool CheckMino(const int32 x, const int32 y);
	int32 CalculateGuideMinoHeight(const int32 x, const int32 y);
	void CheckLineDelete(const TArray<int32> heights);
	TArray<TArray<E_TNTetrominoType>>& GetCheckBuffer();

private:
	TArray<TArray<E_TNTetrominoType>> CheckBuffer;
	int32 BufferHeight = 0;
	int32 BufferWidth = 0;
	TArray<int32> DeletedLines;
	float DeleteLineCheckTime = 0.f;
	bool bLineDeleting = false;
	bool bSpaceInverted = false;
};