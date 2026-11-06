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

	loadVolumes();

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
		UGameplayStatics::PlaySound2D(this, sound, MasterVolume * SFXVolume);
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
	BgmComponent = UGameplayStatics::SpawnSound2D(this, sound, MasterVolume * BGMVolume, 1.f, 0.f, nullptr, bPersistAcrossLevelTransition, bAutoDestroy);
	applyBgmState();
}

void UTNAudioSubsystem::PauseBGM()
{
	bBgmPaused = true;
	applyBgmState();
}

void UTNAudioSubsystem::ResumeBGM()
{
	bBgmPaused = false;
	applyBgmState();
}

void UTNAudioSubsystem::StopBGM()
{
	bBgmPaused = false;

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
	LoopComponent = UGameplayStatics::SpawnSound2D(this, sound, MasterVolume * BGMVolume, 1.f, 0.f, nullptr, bPersistAcrossLevelTransition, bAutoDestroy);
	applyLoopState();
}

void UTNAudioSubsystem::StopLoop()
{
	if (IsValid(LoopComponent))
	{
		LoopComponent->Stop();
		LoopComponent = nullptr;
	}
}

void UTNAudioSubsystem::SetLoopSuspended(const bool bSuspended)
{
	bLoopSuspended = bSuspended;
	applyLoopState();
}

void UTNAudioSubsystem::applyBgmState()
{
	if (IsValid(BgmComponent))
	{
		BgmComponent->SetPaused(bBgmPaused);
	}
}

void UTNAudioSubsystem::applyLoopState()
{
	if (IsValid(LoopComponent))
	{
		LoopComponent->SetPaused(bLoopSuspended);
	}
}

void UTNAudioSubsystem::SetMasterVolume(const float volume)
{
	MasterVolume = FMath::Clamp(volume, 0.f, 1.f);
	applyVolumes();
}

void UTNAudioSubsystem::SetBGMVolume(const float volume)
{
	BGMVolume = FMath::Clamp(volume, 0.f, 1.f);
	applyVolumes();
}

void UTNAudioSubsystem::SetSFXVolume(const float volume)
{
	SFXVolume = FMath::Clamp(volume, 0.f, 1.f);
}

void UTNAudioSubsystem::applyVolumes()
{
	const float musicVolume = MasterVolume * BGMVolume;

	if (IsValid(BgmComponent))
	{
		BgmComponent->SetVolumeMultiplier(musicVolume);
	}

	if (IsValid(LoopComponent))
	{
		LoopComponent->SetVolumeMultiplier(musicVolume);
	}
}

namespace
{
	const TCHAR* const AudioSettingsSection = TEXT("Tenetris.Audio");
}

void UTNAudioSubsystem::loadVolumes()
{
	if (!GConfig)
	{
		return;
	}

	GConfig->GetFloat(AudioSettingsSection, TEXT("MasterVolume"), MasterVolume, GGameUserSettingsIni);
	GConfig->GetFloat(AudioSettingsSection, TEXT("BGMVolume"), BGMVolume, GGameUserSettingsIni);
	GConfig->GetFloat(AudioSettingsSection, TEXT("SFXVolume"), SFXVolume, GGameUserSettingsIni);

	MasterVolume = FMath::Clamp(MasterVolume, 0.f, 1.f);
	BGMVolume = FMath::Clamp(BGMVolume, 0.f, 1.f);
	SFXVolume = FMath::Clamp(SFXVolume, 0.f, 1.f);
}

void UTNAudioSubsystem::SaveVolumes() const
{
	if (!GConfig)
	{
		return;
	}

	GConfig->SetFloat(AudioSettingsSection, TEXT("MasterVolume"), MasterVolume, GGameUserSettingsIni);
	GConfig->SetFloat(AudioSettingsSection, TEXT("BGMVolume"), BGMVolume, GGameUserSettingsIni);
	GConfig->SetFloat(AudioSettingsSection, TEXT("SFXVolume"), SFXVolume, GGameUserSettingsIni);
	GConfig->Flush(false, GGameUserSettingsIni);
}
