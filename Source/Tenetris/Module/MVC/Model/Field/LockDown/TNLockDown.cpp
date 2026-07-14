#include "TNLockDown.h"
#include "Tenetris/Common/TenetrisEnum.h"

FTNLockDown::FTNLockDown()
{
	LockDownRemainCount = MaxLockDownRemainCount;
}

void FTNLockDown::StartLockDown()
{
	LockDownStart = true;
}

bool FTNLockDown::UpdateLockDown(float deltaTime)
{
	if (LockDownStart)
	{
		LockDownTime += deltaTime;

		if (LockDownTime >= LockDownDelay || LockDownRemainCount <= 0)
		{
			LockDownTime = 0.f;
			LockDownStart = false;
			LockDownRemainCount = MaxLockDownRemainCount;

			return true;
		}
	}

	return false;
}

void FTNLockDown::CheckRemainCount(E_TNTetrominoDirection tetrominoDirection)
{
	if (!LockDownStart)
	{
		return;
	}

	if (tetrominoDirection == E_TNTetrominoDirection::Left
		|| tetrominoDirection == E_TNTetrominoDirection::Right)
	{
		LockDownRemainCount--;
	}
	else if (tetrominoDirection == E_TNTetrominoDirection::Down)
	{
		LockDownRemainCount = MaxLockDownRemainCount;
		LockDownStart = false;
	}

	LockDownTime = 0.f;
}

void FTNLockDown::CheckRemainCount()
{
	if (!LockDownStart)
	{
		return;
	}

	LockDownRemainCount--;
	LockDownTime = 0.f;
}

