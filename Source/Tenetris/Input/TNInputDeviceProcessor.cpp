// Fill out your copyright notice in the Description page of Project Settings.

#include "Tenetris/Input/TNInputDeviceProcessor.h"
#include "Tenetris/Input/TNInputDeviceSubsystem.h"

namespace
{
	// Stick drift below this does not count as gamepad use.
	constexpr float AnalogDeadZone = 0.3f;

	// Mouse jitter below this (in pixels) does not count as mouse use.
	constexpr float MouseMoveThreshold = 2.f;
}

bool FTNInputDeviceProcessor::HandleKeyDownEvent(FSlateApplication& slateApp, const FKeyEvent& inKeyEvent)
{
	notify(inKeyEvent.GetKey().IsGamepadKey() ? E_TNInputDevice::Gamepad : E_TNInputDevice::KeyboardMouse);
	return false;
}

bool FTNInputDeviceProcessor::HandleAnalogInputEvent(FSlateApplication& slateApp, const FAnalogInputEvent& inAnalogInputEvent)
{
	if (inAnalogInputEvent.GetKey().IsGamepadKey() && FMath::Abs(inAnalogInputEvent.GetAnalogValue()) > AnalogDeadZone)
	{
		notify(E_TNInputDevice::Gamepad);
	}

	return false;
}

bool FTNInputDeviceProcessor::HandleMouseMoveEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent)
{
	if (mouseEvent.GetCursorDelta().SizeSquared() > MouseMoveThreshold * MouseMoveThreshold)
	{
		notify(E_TNInputDevice::KeyboardMouse);
	}

	return false;
}

bool FTNInputDeviceProcessor::HandleMouseButtonDownEvent(FSlateApplication& slateApp, const FPointerEvent& mouseEvent)
{
	notify(E_TNInputDevice::KeyboardMouse);
	return false;
}

bool FTNInputDeviceProcessor::HandleMouseWheelOrGestureEvent(FSlateApplication& slateApp, const FPointerEvent& inWheelEvent, const FPointerEvent* inGestureEvent)
{
	notify(E_TNInputDevice::KeyboardMouse);
	return false;
}

void FTNInputDeviceProcessor::notify(const E_TNInputDevice device) const
{
	OnInputDeviceDetected.ExecuteIfBound(device);
}
