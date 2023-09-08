// Copyright 2020 by Nicholas Helish. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/AnalogCursor.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVirtualAnalogCursor, Log, All);

DECLARE_EVENT_OneParam(FVirtualAnalogCursor, FHoverStateChanged, FName);

/**
 * 
 */
class UEVIRTUALCURSOR_API FVirtualAnalogCursor : public FAnalogCursor
{
public:
	
	/** Ctor */
	FVirtualAnalogCursor(ULocalPlayer* InLocalPlayer, UWorld* InWorld, float InRadius);
	FVirtualAnalogCursor(APlayerController* PC, float InRadius);

	/** Dtor */
	virtual ~FVirtualAnalogCursor()
	{}

	/** So incase somebody wants to extend this to support multiple "cursors" for multiple players :) */
	virtual int32 GetOwnerUserIndex() const override;

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	/** Uses different logic than base cursor */
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	bool IsWidgetInteractable(const TSharedPtr<SWidget> Widget) const;
	
	/** Getter */
	FORCEINLINE FName GetHoveredWidgetName() const
	{
		return HoveredWidgetName;
	}

	/** Getter */
	FORCEINLINE bool IsHovered() const
	{
		return HoveredWidgetName != NAME_None;
	}

	/** Getter */
	FORCEINLINE FVector2D GetCurrentPosition() const
	{
		return CurrentPosition;
	}

	/** Getter */
	FORCEINLINE FVector2D GetVelocity() const
	{
		return Velocity;
	}

	/** Getter */
	FORCEINLINE bool GetIsUsingAnalogCursor() const
	{
		return bIsUsingAnalogCursor;
	}

	/** Getter */
	FORCEINLINE FVector2D GetLastCursorDirection() const
	{
		return LastCursorDirection;
	}

	/** Getter */
	FORCEINLINE float GetRadius() const
	{
		return Radius;
	}	

	uint8 bDebugging : 1;

	uint8 bAnalogDebug : 1;

	FHoverStateChanged OnHoverStateChanged;

private:

	/** Helper Function. Takes in values from the analog stick, returns a vector that represents acceleration */
	FVector2D GetAnalogCursorAccelerationValue(const FVector2D& InAnalogValues, float DPIScale);

	/** Current velocity of the cursor */
	FVector2D Velocity;

	/** Current position of the cursor. Stored outside of ICursor's position to avoid float->int32 truncation */
	FVector2D CurrentPosition;

	/** Unit vector derived from Velocity */
	FVector2D LastCursorDirection;

	/** The name of the hovered widget */
	FName HoveredWidgetName;

	/** Is this thing even active right now? */
	uint8 bIsUsingAnalogCursor : 1;

	/** The radius of the analog cursor */
	float Radius;

	/** The Player's Context */
	FLocalPlayerContext PlayerContext;

	TSet<FKey> PressedKeys;
};
