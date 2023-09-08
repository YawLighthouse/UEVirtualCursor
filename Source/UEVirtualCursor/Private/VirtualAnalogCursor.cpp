﻿// Copyright 2020 by Nicholas Helish. All Rights Reserved.

#include "VirtualAnalogCursor.h"

#include "UEVirtualCursorSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/UserInterfaceSettings.h"

DEFINE_LOG_CATEGORY(LogVirtualAnalogCursor);

// Helper macro for getting the settings without being able to edit the settings
#define GET_SETTINGS GetDefault<UUEVirtualCursorSettings>()
// Helper macro for getting the settings while being able to edit the settings
#define GET_SETTINGS_MUTABLE GetMutableDefault<UUEVirtualCursorSettings>()

FVirtualAnalogCursor::FVirtualAnalogCursor(ULocalPlayer* InLocalPlayer, UWorld* InWorld, float InRadius)
	: bDebugging(false)
	, bAnalogDebug(false)
	, Velocity(FVector2D::ZeroVector)
	, CurrentPosition(FLT_MAX, FLT_MAX)
	, LastCursorDirection(FVector2D::ZeroVector)
	, HoveredWidgetName(NAME_None)
	, bIsUsingAnalogCursor(false)
	, Radius(FMath::Max<float>(InRadius, 16.0f))
	, PlayerContext(InLocalPlayer, InWorld)
{
	ensure(PlayerContext.IsValid());	
}

FVirtualAnalogCursor::FVirtualAnalogCursor(class APlayerController* PC, float InRadius)
: bDebugging(false)
, bAnalogDebug(false)
, Velocity(FVector2D::ZeroVector)
, CurrentPosition(FLT_MAX, FLT_MAX)
, LastCursorDirection(FVector2D::ZeroVector)
, HoveredWidgetName(NAME_None)
, bIsUsingAnalogCursor(false)
, Radius(FMath::Max<float>(InRadius, 16.0f))
, PlayerContext(PC)
{
	ensure(PlayerContext.IsValid());
}

int32 FVirtualAnalogCursor::GetOwnerUserIndex() const
{		
	if (ULocalPlayer* LP = PlayerContext.GetLocalPlayer())
	{
		return LP->GetControllerId();
	}
	return 0;
}

bool FVirtualAnalogCursor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	// So we only read from the correct player index(to handle for local coop)
	if (!IsRelevantInput(InKeyEvent))
	{
		// If the index of whoever pressed a key is not this cursor's index then its another local player(so they dont control the inputs)
		return false;
	}

	const FKey& PressedKey = InKeyEvent.GetKey();

	if (InKeyEvent.IsRepeat())
	{
		if (bDebugging)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "KEY: " + PressedKey.ToString() + " Held");
		}
	}
	else
	{
		PressedKeys.Add(PressedKey);
		if (bDebugging)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "KEY: " + PressedKey.ToString() + " Pressed");
		}
	}

	return FAnalogCursor::HandleKeyDownEvent(SlateApp, InKeyEvent);
}

bool FVirtualAnalogCursor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	// So we only read from the correct player index(to handle for local coop)
	if (!IsRelevantInput(InKeyEvent))
	{
		// If the index of whoever pressed a key is not this cursor's index then its another local player(so they dont control the inputs)
		return false;
	}

	const FKey& ReleasedKey = InKeyEvent.GetKey();

	PressedKeys.Remove(ReleasedKey);
	if (bDebugging)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "KEY: " + ReleasedKey.ToString() + " Released");
	}

	return FAnalogCursor::HandleKeyUpEvent(SlateApp, InKeyEvent);
}

bool FVirtualAnalogCursor::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{
	// So we only read from the correct player index(to handle for local coop)
	if (!IsRelevantInput(InAnalogInputEvent))
	{
		// If the index of whoever pressed a key is not this cursor's index then its another local player(so they dont control the inputs)
		return false;
	}

	if (bAnalogDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "ANALOG: " + InAnalogInputEvent.GetKey().ToString());
	}
	return FAnalogCursor::HandleAnalogInputEvent(SlateApp, InAnalogInputEvent);
}

bool FVirtualAnalogCursor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	// So we only read from the correct player index(to handle for local coop)
	if (!IsRelevantInput(MouseEvent))
	{
		// If the index of whoever pressed a key is not this cursor's index then its another local player(so they dont control the inputs)
		return false;
	}

	const FKey& PressedKey = MouseEvent.GetEffectingButton();
	if (PressedKeys.Contains(PressedKey))
	{
		if (bDebugging)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "MOUSE: " + PressedKey.ToString() + " Held");
		}
	}
	else
	{
		PressedKeys.Add(PressedKey);
		if (bDebugging)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "MOUSE: " + PressedKey.ToString() + " Pressed");
		}
	}
	return false;
}

bool FVirtualAnalogCursor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	// So we only read from the correct player index(to handle for local coop)
	if (!IsRelevantInput(MouseEvent))
	{
		// If the index of whoever pressed a key is not this cursor's index then its another local player(so they dont control the inputs)
		return false;
	}

	const FKey& ReleasedKey = MouseEvent.GetEffectingButton();
	if (bDebugging)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "MOUSE: " + ReleasedKey.ToString() + " Released");
	}
	PressedKeys.Remove(ReleasedKey);
	return false;
}

void FVirtualAnalogCursor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	if (PlayerContext.IsValid() && PlayerContext.GetPlayerController())
	{
		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(PlayerContext.GetPlayerController());
		const float DPIScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(FMath::RoundToInt(ViewportSize.X), FMath::RoundToInt(ViewportSize.Y)));
		
		const UUEVirtualCursorSettings* Settings = GET_SETTINGS;

		// If we have no acceleration curve, then move on;
		if (Settings->GetUseEngineAnalogCursor())
		{
			FAnalogCursor::Tick(DeltaTime, SlateApp, Cursor);
			return;
		}

		// Set the current position if we haven't already
		const FVector2D CurrentPositionTruc = FVector2D(FMath::TruncToFloat(CurrentPosition.X), FMath::TruncToFloat(CurrentPosition.Y));
		if (CurrentPositionTruc != Cursor->GetPosition())
		{
			CurrentPosition = Cursor->GetPosition();
			Velocity = FVector2D::ZeroVector;
			LastCursorDirection = FVector2D::ZeroVector;
			bIsUsingAnalogCursor = false;
			FSlateApplication::Get().SetCursorRadius(0.0f);
		}

		// Cache the old position
		const FVector2D OldPosition = CurrentPosition;

		// Figure out if we should clamp the speed or not
		const float MaxSpeedNoHover = Settings->GetMaxAnalogCursorSpeed()*DPIScale;
		const float MaxSpeedHover = Settings->GetMaxAnalogCursorSpeedWhenHovered()*DPIScale;
		const float DragCoNoHover = Settings->GetAnalogCursorDragCoefficient()*DPIScale;
		const float DragCoHovered = Settings->GetAnalogCursorDragCoefficientWhenHovered()*DPIScale;
		const float MinCursorSpeed = Settings->GetMinAnalogCursorSpeed()*DPIScale;

		// We're about to no longer be hovering a widget
		if(HoveredWidgetName != NAME_None)
		{
			OnHoverStateChanged.Broadcast(NAME_None);
		}
		HoveredWidgetName = NAME_None;
		float DragCo = DragCoNoHover;
		
		// Part of base class now
		MaxSpeed = MaxSpeedNoHover;

		// See if we are hovered over a widget or not
		FWidgetPath WidgetPath = SlateApp.LocateWindowUnderMouse(OldPosition, SlateApp.GetInteractiveTopLevelWindows());
		if (WidgetPath.IsValid())
		{
			for (int32 i = WidgetPath.Widgets.Num() - 1; i >= 0; --i)
			{
				// Grab the widget
				FArrangedWidget& ArrangedWidget = WidgetPath.Widgets[i];
				TSharedRef<SWidget> Widget = ArrangedWidget.Widget;

				// See if it is acceptable or not
				if (IsWidgetInteractable(Widget))
				{
					HoveredWidgetName = Widget->GetType();
					OnHoverStateChanged.Broadcast(HoveredWidgetName);
					DragCo = DragCoHovered;
					MaxSpeed = MaxSpeedHover;
					break;
				}
			}
		}

		// Grab the cursor acceleration
		const FVector2D AccelFromAnalogStick = GetAnalogCursorAccelerationValue(GetAnalogValues(), DPIScale);

		FVector2D NewAccelerationThisFrame = FVector2D::ZeroVector;
		if (!Settings->GetAnalogCursorNoAcceleration())
		{
			// Calculate a new velocity. RK4.
			if (!AccelFromAnalogStick.IsZero() || !Velocity.IsZero())
			{
				const FVector2D A1 = (AccelFromAnalogStick - (DragCo * Velocity)) * DeltaTime;
				const FVector2D A2 = (AccelFromAnalogStick - (DragCo * (Velocity + (A1 * 0.5f)))) * DeltaTime;
				const FVector2D A3 = (AccelFromAnalogStick - (DragCo * (Velocity + (A2 * 0.5f)))) * DeltaTime;
				const FVector2D A4 = (AccelFromAnalogStick - (DragCo * (Velocity + A3))) * DeltaTime;
				NewAccelerationThisFrame = (A1 + (2.0f * A2) + (2.0f * A3) + A4) / 6.0f;
				Velocity += NewAccelerationThisFrame;
			}
		}
		else
		{
			// Else, use what is coming straight from the analog stick
			Velocity = AccelFromAnalogStick;
		}

		// If we are smaller than out min speed, zero it out
		const float VelSizeSq = Velocity.SizeSquared();
		if (VelSizeSq < (MinCursorSpeed * MinCursorSpeed))
		{
			Velocity = FVector2D::ZeroVector;
		}
		else if (VelSizeSq > (MaxSpeed * MaxSpeed))
		{
			// also cap us if we are larger than our max speed
			Velocity = Velocity.GetSafeNormal() * MaxSpeed;
		}

		// store off the last cursor direction
		if (!Velocity.IsZero())
		{
			LastCursorDirection = Velocity.GetSafeNormal();
		}

		// Update the new position
		CurrentPosition += (Velocity * DeltaTime);
		
		// Update the cursor position
		UpdateCursorPosition(SlateApp, SlateApp.GetUser(GetOwnerUserIndex()).ToSharedRef(), CurrentPosition);

		// If we get here, and we are moving the stick, then hooray
		if (!AccelFromAnalogStick.IsZero())
		{
			bIsUsingAnalogCursor = true;
			FSlateApplication::Get().SetCursorRadius(Settings->GetAnalogCursorRadius()*DPIScale);
		}
	}
}

bool FVirtualAnalogCursor::IsWidgetInteractable(const TSharedPtr<SWidget> Widget) const
{
	return Widget.IsValid() && Widget->IsInteractable();
}

FVector2D FVirtualAnalogCursor::GetAnalogCursorAccelerationValue(const FVector2D& InAnalogValues, float DPIScale)
{
	const UUEVirtualCursorSettings* Settings = GET_SETTINGS;

	FVector2D RetValue = FVector2D::ZeroVector;
	if ( const FRichCurve* AccelerationCurve = Settings->GetAnalogCursorAccelerationCurve() )
	{
		const float DeadZoneSize = Settings->GetAnalogCursorDeadZone();
		const float AnalogValSize = InAnalogValues.Size();
		if (AnalogValSize > DeadZoneSize)
		{
			RetValue = AccelerationCurve->Eval(AnalogValSize) * InAnalogValues.GetSafeNormal() * DPIScale;
			RetValue *= Settings->GetAnalogCursorAccelerationMultiplier()*DPIScale;
		}
	}
	return RetValue;
}

