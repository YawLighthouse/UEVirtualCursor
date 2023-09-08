// Copyright 2020 by Nicholas Helish. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UEVirtualCursorSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class UEVIRTUALCURSOR_API UUEVirtualCursorSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	
	UUEVirtualCursorSettings();

	FORCEINLINE const FRichCurve* GetAnalogCursorAccelerationCurve() const
	{
		return AnalogCursorAccelerationCurve.GetRichCurveConst();
	}
	
	FORCEINLINE float GetMaxAnalogCursorSpeed() const
	{
		return MaxAnalogCursorSpeed;
	}

	FORCEINLINE float GetMaxAnalogCursorSpeedWhenHovered() const
	{
		return MaxAnalogCursorSpeedWhenHovered;
	}

	FORCEINLINE float GetAnalogCursorDragCoefficient() const
	{
		return FMath::Min<float>(AnalogCursorDragCoefficientWhenHovered, AnalogCursorDragCoefficient);
	}

	FORCEINLINE float GetAnalogCursorDragCoefficientWhenHovered() const
	{
		return FMath::Max<float>(AnalogCursorDragCoefficientWhenHovered, AnalogCursorDragCoefficient);
	}

	FORCEINLINE float GetMinAnalogCursorSpeed() const
	{
		return MinAnalogCursorSpeed;
	}

	FORCEINLINE float GetAnalogCursorAccelerationMultiplier() const
	{
		return AnalogCursorAccelerationMultiplier;
	}

	FORCEINLINE float GetAnalogCursorDeadZone() const
	{
		return AnalogCursorDeadZone;
	}

	FORCEINLINE float GetAnalogCursorSize() const
	{
		return FMath::Max<float>(AnalogCursorSize, 1.0f);
	}

	FORCEINLINE FVector2D GetAnalogCursorSizeVector() const
	{
		return FVector2D(GetAnalogCursorSize(), GetAnalogCursorSize());
	}

	FORCEINLINE float GetAnalogCursorRadius() const
	{
		return GetAnalogCursorSize() / 2.0f;
	}

	FORCEINLINE bool GetUseEngineAnalogCursor() const
	{
		return bUseEngineAnalogCursor;
	}

	FORCEINLINE bool GetAnalogCursorNoAcceleration() const
	{
		return bAnalogCursorNoAcceleration;
	}

private:
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta=(
		XAxisName="Strength",
		YAxisName="Acceleration" ))
	FRuntimeFloatCurve AnalogCursorAccelerationCurve;

	/** The max speed of the Analog Cursor */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "1.0"))
	float MaxAnalogCursorSpeed = 1300.0f;

	/** The max speed of the Analog Cursor when hovering over a widget that is interactable */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "1.0"))
	float MaxAnalogCursorSpeedWhenHovered = 650.0f;

	/** The coefficient of drag applied to the cursor */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0"))
	float AnalogCursorDragCoefficient = 8.0f;

	/** The coefficient of drag applied to the cursor when hovering */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0"))
	float AnalogCursorDragCoefficientWhenHovered = 14.0;

	/** The min speed of the analog cursor. If it goes below this value, the speed is set to 0. */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0"))
	float MinAnalogCursorSpeed = 5.0f;

	/** Deadzone value for input from the analog stick */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AnalogCursorDeadZone = 0.15f;

	/** Multiplied by the value spit out by the normalized acceleration curve */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "1.0"))
	float AnalogCursorAccelerationMultiplier = 9000.0f;

	/** The size (on screen) of the analog cursor */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0"))
	float AnalogCursorSize = 40.0f;

	/** If true, defaults to the Engine's Analog Cursor */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor")
	uint8 bUseEngineAnalogCursor : 1;

	/** If True, AnalogCursorAccelerationCurve will be used as a Velocity Curve */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor")
	uint8 bAnalogCursorNoAcceleration : 1;
};
