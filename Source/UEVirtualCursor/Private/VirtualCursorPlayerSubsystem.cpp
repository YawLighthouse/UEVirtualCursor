// Copyright 2020 by Nicholas Helish. All Rights Reserved.


#include "VirtualCursorPlayerSubsystem.h"

#include "Engine/Engine.h"
#include "UEVirtualCursorSettings.h"
#include "VirtualAnalogCursor.h"
#include "Framework/Application/SlateApplication.h"

DEFINE_LOG_CATEGORY(LogVirtualCursorPlayerSubsystem);

// Helper macro for getting the settings without being able to edit the settings
#define GET_SETTINGS GetDefault<UUEVirtualCursorSettings>()
// Helper macro for getting the settings while being able to edit the settings
#define GET_SETTINGS_MUTABLE GetMutableDefault<UUEVirtualCursorSettings>()

void UVirtualCursorPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UVirtualCursorPlayerSubsystem::Deinitialize()
{
	// When the local player is ending, cleanup the analog cursor and reset the shared ptr variable
	DisableAnalogCursor();
	Cursor.Reset();
}

void UVirtualCursorPlayerSubsystem::EnableAnalogCursor()
{	
	// Ensure that slate and the world is valid
	if (FSlateApplication::IsInitialized() && GetWorld())
	{
		const float CursorRadius = GET_SETTINGS->GetAnalogCursorRadius();		

		// If the shared ptr isnt tied to a valid obj then create one and connect the two
		if (!IsCursorValid())
		{
			Cursor = MakeShareable(new FVirtualAnalogCursor(GetLocalPlayer(), GetWorld(), CursorRadius));
			Cursor->OnHoverStateChanged.AddUObject(this, &UVirtualCursorPlayerSubsystem::CursorStateChanged);
		}

		// Check that we're not re-adding it(which counts as a duplicate)
		if (!ContainsGamepadCursorInputProcessor())
		{
			FSlateApplication::Get().RegisterInputPreProcessor(Cursor);
		}
		FSlateApplication::Get().SetCursorRadius(CursorRadius);
	}
}

void UVirtualCursorPlayerSubsystem::DisableAnalogCursor()
{
	if (FSlateApplication::IsInitialized())
	{
		// Dont try to remove it if we already removed it, you may say overkill I say ensuring safeguards
		if (ContainsGamepadCursorInputProcessor())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(Cursor);
		}
		FSlateApplication::Get().SetCursorRadius(0.0f);
	}
}

void UVirtualCursorPlayerSubsystem::ToggleCursorDebug()
{
	if (IsCursorValid())
	{
		Cursor->bDebugging = !Cursor->bDebugging;
		const FString BoolResult = ((Cursor->bDebugging) ? "true" : "false");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, "Cursor Debug: " + BoolResult);
	}
}

void UVirtualCursorPlayerSubsystem::ToggleAnalogDebug()
{
	if (IsCursorValid())
	{
		Cursor->bAnalogDebug = !Cursor->bAnalogDebug;
		const FString BoolResult = ((Cursor->bAnalogDebug) ? "true" : "false");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, "Analog Debug: " + BoolResult);
	}
}

bool UVirtualCursorPlayerSubsystem::IsCursorDebugActive() const
{
	if (IsCursorValid())
	{
		return Cursor->bDebugging;
	}
	return false;
}

bool UVirtualCursorPlayerSubsystem::IsAnalogDebugActive() const
{
	if (IsCursorValid())
	{
		return Cursor->bAnalogDebug;
	}
	return false;
}

bool UVirtualCursorPlayerSubsystem::IsCursorOverInteractableWidget() const
{
	if (IsCursorValid())
	{
		return Cursor->IsHovered();
	}
	return false;
}

bool UVirtualCursorPlayerSubsystem::IsCursorValid() const
{
	return Cursor.IsValid();
}

bool UVirtualCursorPlayerSubsystem::ContainsGamepadCursorInputProcessor() const
{
	if (FSlateApplication::IsInitialized())
	{
		// Continue if we're using a valid cursor
		if (IsCursorValid())
		{
			// Returns true if index is greater than -1, false if less than/equal to -1
			const int32 FoundIndex = FSlateApplication::Get().FindInputPreProcessor(Cursor);
			return (FoundIndex > -1);
		}
	}
	return false;
}

void UVirtualCursorPlayerSubsystem::CursorStateChanged(FName HoveredWidgetName)
{
	OnCursorHoverStateChanged.Broadcast(HoveredWidgetName != NAME_None);
}
