// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleInterface.h"


class FProtoKitModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#if WITH_EDITOR
	static bool OnResetDefaultsSettingsSection();
	static bool OnSaveSettingsSection();
#endif
};
