// Copyright (C) Dreamer's Tail

#include "GamePulse.h"

#include "GamePulseSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"


DEFINE_LOG_CATEGORY(LogGamePulse);


#define LOCTEXT_NAMESPACE "FGamePulseModule"

void FGamePulseModule::StartupModule()
{
	/* Register settings */
	if (ISettingsModule* SettingsModule =
		FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		const TSharedPtr<ISettingsSection> SettingsSection =
			SettingsModule->RegisterSettings(
			"Editor", "Plugins", "Game Pulse",
			LOCTEXT("PluginSettingsName", "Game Pulse"),
			LOCTEXT("PluginSettingsDescription", "Game Pulse Settings"),
			GetMutableDefault<UGamePulseSettings>());

		SettingsSection->OnResetDefaults().BindStatic(
			&FGamePulseModule::OnResetDefaultsSettingsSection);
		SettingsSection->OnSave().BindStatic(
			&FGamePulseModule::OnSaveSettingsSection);
		SettingsSection->OnSaveDefaults().BindStatic(
			&FGamePulseModule::OnSaveSettingsSection);
	}
	/* ~ Register settings */
}

void FGamePulseModule::ShutdownModule()
{
}

bool FGamePulseModule::OnResetDefaultsSettingsSection()
{
	if (FString ConfigFilePath;
		UGamePulseSettings::GetConfigPath(ConfigFilePath))
	{
		if (UGamePulseSettings* Settings =
			GetMutableDefault<UGamePulseSettings>())
		{
			Settings->LoadConfig(
				UGamePulseSettings::StaticClass(),
				*ConfigFilePath);
			return true;
		}
	}

	return false;
}

bool FGamePulseModule::OnSaveSettingsSection()
{
	FString ConfigFilePath;
	UGamePulseSettings::GetConfigPath(ConfigFilePath);

	if (UGamePulseSettings* Settings =
		GetMutableDefault<UGamePulseSettings>())
	{
		Settings->TryUpdateDefaultConfigFile(*ConfigFilePath);
		return true;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamePulseModule, GamePulse)
