// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TNAudioData.generated.h"

class USoundBase;

// Central key constants for audio lookups. Both the DataAsset entries and the
// playback call sites reference these so typos become compile errors, not silent
// runtime failures.
namespace TNSoundKeys
{
	const FName LineClear(TEXT("LineClear"));
	const FName HardDrop(TEXT("HardDrop"));
	const FName SpaceInversion(TEXT("SpaceInversion"));
	const FName TimeInversion(TEXT("TimeInversion"));
	const FName Bgm(TEXT("Bgm"));
	const FName GameOver(TEXT("GameOver"));
	const FName MenuClick(TEXT("MenuClick"));
}

// Name-based sound table. Add/replace sounds in the editor (DA_TNAudio) without
// touching code; look them up by the TNSoundKeys constants.
UCLASS(BlueprintType)
class TENETRIS_API UTNAudioData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Returns the sound registered for Key, or nullptr if absent.
	USoundBase* GetSound(const FName key) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TMap<FName, TObjectPtr<USoundBase>> Sounds;
};
