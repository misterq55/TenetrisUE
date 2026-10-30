// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TNAudioSubsystem.generated.h"

class UTNAudioData;
class USoundBase;
class UAudioComponent;

// Central audio playback. Lives on the game instance so BGM survives level travel.
// Sound assets are supplied by the UTNGameInstance's UTNAudioData (name-keyed);
// this subsystem never hardcodes asset paths.
UCLASS()
class TENETRIS_API UTNAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	// Fire-and-forget 2D sound effect.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySFX(const FName key);

	// Starts (or restarts) the main looping BGM, keeping a handle for pause/resume.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayBGM(const FName key);

	// Pauses the main BGM, preserving its playback position.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PauseBGM();

	// Resumes the main BGM from where it was paused.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ResumeBGM();

	// Stops the main BGM entirely.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopBGM();

	// Starts a looping secondary track (e.g. the rewind BGM) separate from the main BGM.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayLoop(const FName key);

	// Stops the secondary looping track started by PlayLoop.
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopLoop();

private:
	USoundBase* findSound(const FName key) const;

	UPROPERTY()
	TObjectPtr<UTNAudioData> AudioData;

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> BgmComponent;

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> LoopComponent;
};
