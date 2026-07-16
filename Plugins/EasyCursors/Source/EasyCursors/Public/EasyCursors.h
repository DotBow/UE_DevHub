// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleInterface.h"


class FEasyCursorsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
