#pragma once

#include "CoreMinimal.h"

enum class E_TNTetrominoDirection : uint32;

class FTNLockDown
{
public:
	FTNLockDown();
	~FTNLockDown() {}

	void StartLockDown();
	bool UpdateLockDown(float deltaTime);
	void CheckRemainCount(E_TNTetrominoDirection tetrominoDirection);
	void CheckRemainCount();

private:
	float LockDownDelay = 0.5f;
	float LockDownTime = 0.f;
	bool LockDownStart = false;
	const int32 MaxLockDownRemainCount = 15;
	int32 LockDownRemainCount = 0;
};
