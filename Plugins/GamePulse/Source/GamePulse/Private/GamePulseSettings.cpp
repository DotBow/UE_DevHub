// Copyright (C) Dreamer's Tail

#include "GamePulseSettings.h"

#include "Engine/Blueprint.h"
#include "Misc/Paths.h"


UGamePulseSettings::UGamePulseSettings()
{
}

bool UGamePulseSettings::GetConfigPath(
	FString& OutConfigFilePath)
{
	FString PluginConfigDir = FPaths::Combine(
		FPaths::ProjectPluginsDir(), TEXT("GamePulse/Config"));
	OutConfigFilePath = FPaths::ConvertRelativePathToFull(
		FPaths::Combine(PluginConfigDir,
			TEXT("GamePulseSettings.ini")));

	if (FPaths::FileExists(OutConfigFilePath))
		return true;

	return false;
}

UClass* UGamePulseSettings::GetGamePulseWidgetClass() const
{
	if (!GamePulseWidget.IsNull())
	{
		if (UObject* LoadedObject = GamePulseWidget.TryLoad())
		{
			if (const UBlueprint* BlueprintAsset = Cast<UBlueprint>(LoadedObject))
				return BlueprintAsset->GeneratedClass;

			if (UClass* Class = Cast<UClass>(LoadedObject))
				return Class;
		}
	}

	return nullptr;
}
