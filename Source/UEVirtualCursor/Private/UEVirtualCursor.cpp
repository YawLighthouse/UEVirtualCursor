// Copyright 2020 by Nicholas Helish. All Rights Reserved.

#include "UEVirtualCursor.h"

DEFINE_LOG_CATEGORY(LogUEVirtualCursorModule);

#define LOCTEXT_NAMESPACE "UEVirtualCursorModule"

class FUEVirtualCursorModule : public IUEVirtualCursorModule
{
public:

	virtual void StartupModule() override
	{
		UE_LOG(LogUEVirtualCursorModule, Display, TEXT("UE Virtual Cursor Module Started"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogUEVirtualCursorModule, Display, TEXT("UE Virtual Cursor Module Shutdown"));
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}
	
};

IMPLEMENT_MODULE(FUEVirtualCursorModule, UEVirtualCursor)
	
#undef LOCTEXT_NAMESPACE