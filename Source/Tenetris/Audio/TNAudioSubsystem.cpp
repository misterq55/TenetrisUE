// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Audio/TNAudioSubsystem.h"
#include "Tenetris/Audio/TNAudioData.h"
#include "Tenetris/Core/TNGameInstance.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UTNAudioSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	if (const UTNGameInstance* gameInstance = Cast<UTNGameInstance>(GetGameInstance()))
	{
		AudioData = gameInstance->GetAudioData();
	}

	if (!IsValid(AudioData))
	{
		UE_LOG(LogTemp, Warning, TEXT("TNAudioSubsystem: no AudioData assigned on the game instance; sounds will not play."));
	}
}

void UTNAudioSubsystem::Deinitialize()
{
	StopBGM();
	StopLoop();

	Super::Deinitialize();
}

USoundBase* UTNAudioSubsystem::findSound(const FName key) const
{
	if (!IsValid(AudioData))
	{
		return nullptr;
	}

	USoundBase* sound = AudioData->GetSound(key);
	if (!sound)
	{
		UE_LOG(LogTemp, Warning, TEXT("TNAudioSubsystem: no sound registered for key '%s'."), *key.ToString());
	}

	return sound;
}

void UTNAudioSubsystem::PlaySFX(const FName key)
{
	if (USoundBase* sound = findSound(key))
	{
		UGameplayStatics::PlaySound2D(this, sound);
	}
}

void UTNAudioSubsystem::PlayBGM(const FName key)
{
	USoundBase* sound = findSound(key);
	if (!sound)
	{
		return;
	}

	StopBGM();

	const bool bPersistAcrossLevelTransition = true;
	const bool bAutoDestroy = false;
	BgmComponent = UGameplayStatics::SpawnSound2D(this, sound, 1.f, 1.f, 0.f, nullptr, bPersistAcrossLevelTransition, bAutoDestroy);
}

void UTNAudioSubsystem::PauseBGM()
{
	if (IsValid(BgmComponent))
	{
		BgmComponent->SetPaused(true);
	}
}

void UTNAudioSubsystem::ResumeBGM()
{
	if (IsValid(BgmComponent))
	{
		BgmComponent->SetPaused(false);
	}
}

void UTNAudioSubsystem::StopBGM()
{
	if (IsValid(BgmComponent))
	{
		BgmComponent->Stop();
		BgmComponent = nullptr;
	}
}

void UTNAudioSubsystem::PlayLoop(const FName key)
{
	USoundBase* sound = findSound(key);
	if (!sound)
	{
		return;
	}

	StopLoop();

	const bool bPersistAcrossLevelTransition = true;
	const bool bAutoDestroy = false;
	LoopComponent = UGameplayStatics::SpawnSound2D(this, sound, 1.f, 1.f, 0.f, nullptr, bPersistAcrossLevelTransition, bAutoDestroy);
}

void UTNAudioSubsystem::StopLoop()
{
	if (IsValid(LoopComponent))
	{
		LoopComponent->Stop();
		LoopComponent = nullptr;
	}
}
