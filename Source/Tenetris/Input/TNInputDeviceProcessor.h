// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"

enum class E_TNInputDevice : uint8;

DECLARE_DELEGATE_OneParam(FTNOnInputDeviceDetected, E_TNInputDevice);

// Slate input pre-processor that only observes which device produced the latest input.
// It never consumes events, so gameplay and UI input are unaffected.
class TENETRIS_API FTNInputDeviceProcessor : public IInputProcessor
{
public:
	virtual void Tick(const float deltaTime, FSlateApplication& slateApp, TSharedRef<ICursor> cursor) override {}

	virtual bool HandleKeyDownEvent(FSlateApplication& slateApp, const FKeyEvent& inKeyEvent) override;
	virtual bool HandleAnalogInputEvent(FSlateApplication& slateApp, const FAnalogInputEvent& inAnalogInputEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent) override;
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& slateApp, const FPointerEvent& inWheelEvent, const FPointerEvent* inGestureEvent) override;

	virtual const TCHAR* GetDebugName() const override { return TEXT("TNInputDeviceProcessor"); }

	FTNOnInputDeviceDetected OnInputDeviceDetected;

private:
	void notify(const E_TNInputDevice device) const;
};
