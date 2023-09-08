// Copyright 2020 by Nicholas Helish. All Rights Reserved.


#include "UEVirtualCursorSettings.h"

UUEVirtualCursorSettings::UUEVirtualCursorSettings()
: bUseEngineAnalogCursor(false)
, bAnalogCursorNoAcceleration(false)
{
	AnalogCursorAccelerationCurve.EditorCurveData.AddKey(0.0f, 0.0f);
	AnalogCursorAccelerationCurve.EditorCurveData.AddKey(1.0f, 1.0f);
}
