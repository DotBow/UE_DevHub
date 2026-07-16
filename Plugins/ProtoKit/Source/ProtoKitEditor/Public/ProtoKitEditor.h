// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleInterface.h"


class FProtoKitEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
