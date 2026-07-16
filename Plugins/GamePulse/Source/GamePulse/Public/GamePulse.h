// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleManager.h"


DECLARE_LOG_CATEGORY_EXTERN(LogGamePulse, Log, All);


class FGamePulseModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static bool OnResetDefaultsSettingsSection();
	static bool OnSaveSettingsSection();
};
