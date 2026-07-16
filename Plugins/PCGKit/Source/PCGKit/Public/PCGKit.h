// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleInterface.h"


class FPCGKitModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
