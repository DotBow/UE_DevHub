// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "EasyPrefsCVars.h"
#include "Engine/DeveloperSettings.h"
#include "StructUtils/InstancedStruct.h"
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
	TArray<FInstancedStruct> CVars;

public:
	UPROPERTY(Config, VisibleAnywhere)
	int32 MaxFPS = 60;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct MaxFPSCVar = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Max FPS", "t.MaxFPS", "MaxFPS", 1, 999));

	UPROPERTY(Config, VisibleAnywhere)
	float Panini = 0.1f;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct PaniniCVar = FInstancedStruct::Make(
		FEasyPrefsCVarFloat("Panini D", "r.LensDistortion.Panini.D", "Panini", 0.f, 1.f));

	UPROPERTY(Config, VisibleAnywhere)
	int32 ScreenPercentage = 100;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct ScreenPercentageCVar = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Screen Percentage", "r.ScreenPercentage", "ScreenPercentage", 1, 100));

	UPROPERTY(Config, VisibleAnywhere)
	int32 UpscaleQuality = 1;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct UpscaleQualityCVar = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Upscale Quality", "r.Upscale.Quality", "UpscaleQuality", 0, 5));

	UPROPERTY(Config, VisibleAnywhere)
	int32 UpscaleSharpeningQuality = 1;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct UpscaleSharpeningQualityCVar = FInstancedStruct::Make(
		FEasyPrefsCVarInt("Upscale Sharpening Quality", "r.Upscale.SharpeningQuality", "UpscaleSharpeningQuality", 0, 2));

	UPROPERTY(Config, VisibleAnywhere)
	int32 AntiAliasingMethod = 3;

	static TMap<FName, int32> AntiAliasingMethodEnum;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct AntiAliasingMethodCVar = FInstancedStruct::Make(
		FEasyPrefsCVarEnum("Anti Aliasing Method", "r.AntiAliasingMethod", "AntiAliasingMethod", AntiAliasingMethodEnum));

	UPROPERTY(Config, VisibleAnywhere)
	int32 MSAACount = 4;

	static TMap<FName, int32> MSAACountEnum;

	UPROPERTY(VisibleAnywhere)
	FInstancedStruct MSAACountCVar = FInstancedStruct::Make(
		FEasyPrefsCVarEnum("MSAA Count", "r.MSAACount", "MSAACount", MSAACountEnum));

	int32 GetNumCVars() const
	{
		return CVars.Num();
	}

	template<typename T>
	const T* GetCVarPtr(const int32 InCVarIdx) const
	{
		if (InCVarIdx >= 0 && InCVarIdx < CVars.Num())
			return CVars[InCVarIdx].GetPtr<T>();

		return nullptr;
	}

	template<typename T>
	T* GetCVarMutablePtr(const int32 InCVarIdx)
	{
		if (InCVarIdx >= 0 && InCVarIdx < CVars.Num())
			return CVars[InCVarIdx].GetMutablePtr<T>();

		return nullptr;
	}

	bool IsCVarChildOf(const int32 InCVarIdx, const UScriptStruct* InScriptStruct) const
	{
		if (InCVarIdx >= 0 && InCVarIdx < CVars.Num())
			return CVars[InCVarIdx].GetScriptStruct() == InScriptStruct;

		return false;
	}

	void SaveCVarConfig(const int32 InCVarIdx)
	{
		if (InCVarIdx >= 0 && InCVarIdx < CVars.Num())
		{
			if (const FEasyPrefsCVar* CVar = CVars[InCVarIdx].GetPtr<FEasyPrefsCVar>())
			{
				if (const FName PropertyName = CVar->GetPropertyName(); PropertyName != NAME_None)
				{
					if (const FProperty* Property = FindFieldChecked<FProperty>(StaticClass(), PropertyName))
						UpdateSinglePropertyInConfigFile(Property, GetDefaultConfigFilename());
				}
			}
		}
	}

	const TArray<FInstancedStruct>& GetCVars() const
	{
		return CVars;
	}
};
