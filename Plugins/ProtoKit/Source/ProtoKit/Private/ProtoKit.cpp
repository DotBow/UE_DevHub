// Copyright (C) Dreamer's Tail

#include "ProtoKit.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ProtoKitSettings.h"
#endif


#define LOCTEXT_NAMESPACE "ProtoKitModule"

void FProtoKitModule::StartupModule()
{
#if WITH_EDITOR
	/* Register settings */
	if (ISettingsModule* SettingsModule =
		FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		const TSharedPtr<ISettingsSection> SettingsSection =
			SettingsModule->RegisterSettings(
			"Project", "Plugins", "ProtoKit",
			LOCTEXT("PluginSettingsName", "ProtoKit"),
			LOCTEXT("PluginSettingsDescription", "ProtoKit Settings"),
			GetMutableDefault<UProtoKitSettings>());

		SettingsSection->OnResetDefaults().BindStatic(
			&FProtoKitModule::OnResetDefaultsSettingsSection);
		SettingsSection->OnSave().BindStatic(
			&FProtoKitModule::OnSaveSettingsSection);
		SettingsSection->OnSaveDefaults().BindStatic(
			&FProtoKitModule::OnSaveSettingsSection);
	}
	/* ~ Register settings */
#endif
}

void FProtoKitModule::ShutdownModule()
{
}

#if WITH_EDITOR
bool FProtoKitModule::OnResetDefaultsSettingsSection()
{
	if (FString ConfigFilePath;
		UProtoKitSettings::GetConfigPath(ConfigFilePath))
	{
		if (UProtoKitSettings* Settings =
			GetMutableDefault<UProtoKitSettings>())
		{
			Settings->LoadConfig(
				UProtoKitSettings::StaticClass(),
				*ConfigFilePath);
			return true;
		}
	}

	return false;
}

bool FProtoKitModule::OnSaveSettingsSection()
{
	FString ConfigFilePath;
	UProtoKitSettings::GetConfigPath(ConfigFilePath);

	if (UProtoKitSettings* Settings =
		GetMutableDefault<UProtoKitSettings>())
	{
		Settings->TryUpdateDefaultConfigFile(*ConfigFilePath);
		return true;
	}

	return false;
}
#endif

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProtoKitModule, ProtoKit)
