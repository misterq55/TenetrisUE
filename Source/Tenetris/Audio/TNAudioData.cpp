// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Audio/TNAudioData.h"
#include "Sound/SoundBase.h"

USoundBase* UTNAudioData::GetSound(const FName key) const
{
	const TObjectPtr<USoundBase>* found = Sounds.Find(key);
	return found ? *found : nullptr;
}
