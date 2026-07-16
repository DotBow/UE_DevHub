// Copyright (C) Dreamer's Tail

#include "ProtoKitSettings.h"


const FSoftObjectPath& UProtoKitSettingsAsset::GetInvalidObjectMeshPath() const
{
	return InvalidObjectMesh;
}

UStaticMesh* UProtoKitSettingsAsset::GetInvalidObjectMesh() const
{
	if (!InvalidObjectMesh.IsNull())
		return Cast<UStaticMesh>(InvalidObjectMesh.TryLoad());

	return nullptr;
}

const FSoftObjectPath& UProtoKitSettingsAsset::GetBaseMeshPath() const
{
	return BaseMesh;
}

UStaticMesh* UProtoKitSettingsAsset::GetBaseMesh() const
{
	if (!BaseMesh.IsNull())
		return Cast<UStaticMesh>(BaseMesh.TryLoad());

	return nullptr;
}

const FSoftObjectPath& UProtoKitSettingsAsset::GetInvalidImageTexturePath() const
{
	return InvalidImageTexture;
}

UTexture2D* UProtoKitSettingsAsset::GetInvalidImageTexture() const
{
	if (!InvalidImageTexture.IsNull())
		return Cast<UTexture2D>(InvalidImageTexture.TryLoad());

	return nullptr;
}


UProtoKitSettings::UProtoKitSettings()
{
}

bool UProtoKitSettings::GetConfigPath(
	FString& OutConfigFilePath)
{
	FString PluginConfigDir = FPaths::Combine(
		FPaths::ProjectPluginsDir(), TEXT("ProtoKit/Config"));
	OutConfigFilePath = FPaths::ConvertRelativePathToFull(
		FPaths::Combine(PluginConfigDir,
			TEXT("ProtoKitSettings.ini")));

	if (FPaths::FileExists(OutConfigFilePath))
		return true;

	return false;
}

void UProtoKitSettings::PostInitProperties()
{
	Super::PostInitProperties();

	if (FString ConfigFilePath;
		GetConfigPath(ConfigFilePath))
		LoadConfig(StaticClass(), *ConfigFilePath);
}
