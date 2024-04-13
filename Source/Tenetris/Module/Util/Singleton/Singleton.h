// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

template<typename T>
class TTNSingleton
{
protected:
  TTNSingleton() {}
public:
  TTNSingleton(const TTNSingleton& c) = delete;
  void operator=(const TTNSingleton& c) = delete;

  static T& GetInstance()
  {
    static T instance;
    return instance;
  }
};
