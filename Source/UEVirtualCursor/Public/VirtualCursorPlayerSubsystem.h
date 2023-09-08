// Copyright 2020 by Nicholas Helish. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "VirtualCursorPlayerSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVirtualCursorPlayerSubsystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCursorHoverStateChanged, const bool, NewState);

class FVirtualAnalogCursor;

/**
 * 
 */
UCLASS(BlueprintType)
class UEVIRTUALCURSOR_API UVirtualCursorPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:

	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
	virtual void Deinitialize() override;
	// ~USubsystem interface

	FORCEINLINE TSharedPtr<FVirtualAnalogCursor> GetCursor() const { return Cursor; }

	UFUNCTION(BlueprintCallable, Category = "Cursor")
	void EnableAnalogCursor();

	UFUNCTION(BlueprintCallable, Category = "Cursor")
	void DisableAnalogCursor();

	UFUNCTION(BlueprintCallable, Category = "Cursor")
	void ToggleCursorDebug();

	UFUNCTION(BlueprintCallable, Category = "Cursor")
	void ToggleAnalogDebug();

	/** 
	* Returns true if the analog cursor input preprocessor debug flag is on/off 
	* Prints an onscreen debug message when a key/button is pressed
	*/
	UFUNCTION(BlueprintPure, Category = "Cursor")
	bool IsCursorDebugActive() const;

	/** 
	* Returns true if the analog cursor debug flag is on/off 
	* Prints an onscreen debug message when an analog input occurs(for example: thumbstick on x/y)
	*/
	UFUNCTION(BlueprintPure, Category = "Cursor")
	bool IsAnalogDebugActive() const;

	UFUNCTION(BlueprintPure, Category = "Cursor", meta = (Keywords = "Hover"))
	bool IsCursorOverInteractableWidget() const;

	UFUNCTION(BlueprintPure, Category = "Cursor")
	bool IsCursorValid() const;

	/** Returns true if this cursor is actively used. */
	UFUNCTION(BlueprintPure, Category = "Cursor")
	bool ContainsGamepadCursorInputProcessor() const;

	UPROPERTY(BlueprintAssignable, Category = "Cursor")
	FCursorHoverStateChanged OnCursorHoverStateChanged;
	
protected:
	
	UFUNCTION()
	virtual void CursorStateChanged(FName HoveredWidgetName);

private:

	TSharedPtr<FVirtualAnalogCursor> Cursor;
};
