// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "EasyPrefsCVars.h"
#include "Engine/DeveloperSettings.h"
#include "EasyPrefsSettings.generated.h"


UCLASS(Config=Engine, DefaultConfig, MinimalAPI)
class UEasyPrefsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UEasyPrefsSettings();

	static const UEasyPrefsSettings* Get()
	{
		if (!IsClassLoaded<UEasyPrefsSettings>())
			return nullptr;

		return GetDefault<UEasyPrefsSettings>();
	}

	static UEasyPrefsSettings* GetMutable()
	{
		if (!IsClassLoaded<UEasyPrefsSettings>())
			return nullptr;

		return GetMutableDefault<UEasyPrefsSettings>();
	}

private:
	UPROPERTY()
	TArray<FName> CVars;

public:
	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct MaxFPS = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Max FPS", "t.MaxFPS", 60, 1, 999));

	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct Panini = FInstancedStruct::Make(
		FEasyPrefsCVarFloat("Panini D", "r.LensDistortion.Panini.D", 0.1f, 0.f, 1.f));

	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct ScreenPercentage = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Screen Percentage", "r.ScreenPercentage", 100, 1, 100));

	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct UpscaleQuality = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Upscale Quality", "r.Upscale.Quality", 1, 0, 5));

	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct UpscaleSharpeningQuality = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Upscale Sharpening Quality", "r.Upscale.SharpeningQuality", 1, 0, 2));

	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct AntiAliasingMethod = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Anti Aliasing Method", "r.AntiAliasingMethod", 3, 0, 5));

	UPROPERTY(Config, EditAnywhere)
	FInstancedStruct MSAACount = FInstancedStruct::Make(
		FEasyPrefsCVarInt("MSAA Count", "r.MSAACount", 4));

	const TArray<FName>& GetCVars() const
	{
		return CVars;
	}

	int32 GetNumCVars() const
	{
		return CVars.Num();
	}

	template<typename T>
	const T* GetCVarPtr(const int32 InCVarIdx) const
	{
		if (const FName PropertyName = GetCVarPropertyName(InCVarIdx); PropertyName != NAME_None)
		{
			if (const FProperty* Property = FindFieldChecked<FProperty>(StaticClass(), PropertyName))
			{
				if (const FInstancedStruct* InstancedStruct = Property->ContainerPtrToValuePtr<FInstancedStruct>(this))
					return InstancedStruct->GetPtr<T>();
			}
		}

		return nullptr;
	}

	template<typename T>
	T* GetCVarMutablePtr(const int32 InCVarIdx)
	{
		if (const FName PropertyName = GetCVarPropertyName(InCVarIdx); PropertyName != NAME_None)
		{
			if (const FProperty* Property = FindFieldChecked<FProperty>(StaticClass(), PropertyName))
			{
				if (FInstancedStruct* InstancedStruct = Property->ContainerPtrToValuePtr<FInstancedStruct>(this))
					return InstancedStruct->GetMutablePtr<T>();
			}
		}

		return nullptr;
	}

	bool IsCVarChildOf(const int32 InCVarIdx, const UScriptStruct* InScriptStruct) const
	{
		if (const FName PropertyName = GetCVarPropertyName(InCVarIdx); PropertyName != NAME_None)
		{
			if (const FProperty* Property = FindFieldChecked<FProperty>(StaticClass(), PropertyName))
			{
				if (const FInstancedStruct* InstancedStruct = Property->ContainerPtrToValuePtr<FInstancedStruct>(this))
					return InstancedStruct->GetScriptStruct() == InScriptStruct;
			}
		}

		return false;
	}

	void SaveCVarConfig(const int32 InCVarIdx)
	{
		if (const FName PropertyName = GetCVarPropertyName(InCVarIdx); PropertyName != NAME_None)
		{
			if (const FProperty* Property = FindFieldChecked<FProperty>(StaticClass(), PropertyName))
				UpdateSinglePropertyInConfigFile(Property, GetDefaultConfigFilename());
		}
	}

private:
	FName GetCVarPropertyName(const int32 InCVarIdx) const
	{
		if (InCVarIdx >= 0 && InCVarIdx < CVars.Num())
			return CVars[InCVarIdx];

		return NAME_None;
	}
};
