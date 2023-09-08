// Copyright 2020 by Nicholas Helish. All Rights Reserved.


#include "UEVirtualCursorLibrary.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "VirtualCursorPlayerSubsystem.h"

DEFINE_LOG_CATEGORY(LogVirtualCursorLibrary);


void UUEVirtualCursorLibrary::Global_EnableVirtualCursor(APlayerController* PC)
{
	if (PC)
	{
		if (PC->GetLocalPlayer())
		{
			PC->GetLocalPlayer()->GetSubsystem<UVirtualCursorPlayerSubsystem>()->EnableAnalogCursor();
		}
	}	
}

void UUEVirtualCursorLibrary::Global_DisableVirtualCursor(APlayerController* PC)
{
	if (PC)
	{
		if (PC->GetLocalPlayer())
		{
			PC->GetLocalPlayer()->GetSubsystem<UVirtualCursorPlayerSubsystem>()->DisableAnalogCursor();
		}
	}
}

bool UUEVirtualCursorLibrary::Global_IsCursorOverInteractableWidget(APlayerController* PC)
{
	if (PC)
	{
		if (PC->GetLocalPlayer())
		{
			return PC->GetLocalPlayer()->GetSubsystem<UVirtualCursorPlayerSubsystem>()->IsCursorOverInteractableWidget();
		}
	}
	
	return false;
}
