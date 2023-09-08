// Copyright 2020 by Nicholas Helish. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UEVirtualCursorLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVirtualCursorLibrary, Log, All);

/**
 * 
 */
UCLASS()
class UEVIRTUALCURSOR_API UUEVirtualCursorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="Game", meta = (DisplayName = "(Global) Enable Virtual Cursor"))
	static void Global_EnableVirtualCursor(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (DisplayName = "(Global) Disable Virtual Cursor"))
	static void Global_DisableVirtualCursor(APlayerController* PC);

	UFUNCTION(BlueprintPure, Category="Game", meta = (DisplayName = "(Global) Is Cursor Over Interactable Widget"))
	static bool Global_IsCursorOverInteractableWidget(APlayerController* PC);
	
};
