// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TNGameInstance.generated.h"

class UTNAudioData;

// Project game instance. Holds the audio data asset so the audio subsystem can
// read it on initialization (assign DA_TNAudio in the BP_TNGameInstance subclass).
UCLASS()
class TENETRIS_API UTNGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTNAudioData* GetAudioData() const { return AudioData; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<UTNAudioData> AudioData;
};
