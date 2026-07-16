// Copyright (C) Dreamer's Tail

#pragma once

#include "ProtoKitSettings.generated.h"


USTRUCT()
struct FProtoKitPlaceableMeshDesc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(AllowedClasses="/Script/Engine.StaticMesh"))
	FSoftObjectPath StaticMesh;

	UPROPERTY(EditAnywhere)
	FString DisplayName;

	UPROPERTY(EditAnywhere)
	FName ThumbnailBrush;

	UPROPERTY(EditAnywhere)
	FName IconBrush;
};


UCLASS()
class UProtoKitSettingsAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Meshes", DisplayName="InvalidObject",
		meta=(AllowedClasses="/Script/Engine.StaticMesh"))
	FSoftObjectPath InvalidObjectMesh;

	UPROPERTY(EditAnywhere, Category="Meshes", DisplayName="BaseMesh",
		meta=(AllowedClasses="/Script/Engine.StaticMesh"))
	FSoftObjectPath BaseMesh;

	UPROPERTY(EditAnywhere, Category="Textures", DisplayName="InvalidImage",
		meta=(AllowedClasses="/Script/Engine.Texture"))
	FSoftObjectPath InvalidImageTexture;

	UPROPERTY(EditAnywhere, Category="Placeable Items",
		meta=(ShowOnlyInnerProperties))
	TArray<FProtoKitPlaceableMeshDesc> PlaceableMeshes;

public:
	PROTOKIT_API const FSoftObjectPath& GetInvalidObjectMeshPath() const;
	PROTOKIT_API UStaticMesh* GetInvalidObjectMesh() const;

	PROTOKIT_API const FSoftObjectPath& GetBaseMeshPath() const;
	PROTOKIT_API UStaticMesh* GetBaseMesh() const;

	PROTOKIT_API const FSoftObjectPath& GetInvalidImageTexturePath() const;
	PROTOKIT_API UTexture2D* GetInvalidImageTexture() const;

	const TArray<FProtoKitPlaceableMeshDesc>& GetPlaceableMeshes() const
	{
		return PlaceableMeshes;
	}
};


UCLASS(Config=ProtoKitSettings, DisplayName="ProtoKit Settings")
class UProtoKitSettings : public UObject
{
	GENERATED_BODY()

public:
	UProtoKitSettings();

	PROTOKIT_API static const UProtoKitSettingsAsset* Get()
	{
		return GetSingleton()->GetAsset();
	}

	static bool GetConfigPath(
		FString& OutConfigFilePath);

private:
	virtual void PostInitProperties() override;

	static const UProtoKitSettings* GetSingleton()
	{
		return GetDefault<UProtoKitSettings>();
	}

	const UProtoKitSettingsAsset* GetAsset() const
	{
		if (!Asset.IsNull())
			return Cast<UProtoKitSettingsAsset>(Asset.TryLoad());

		return GetDefault<UProtoKitSettingsAsset>();
	}

	UPROPERTY(EditAnywhere, Config,
		meta=(AllowedClasses="/Script/ProtoKit.ProtoKitSettingsAsset"))
	FSoftObjectPath Asset;
};
