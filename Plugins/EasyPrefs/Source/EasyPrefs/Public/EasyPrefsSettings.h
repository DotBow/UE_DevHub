// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "EasyPrefsCVars.h"
#include "Engine/DeveloperSettings.h"
#include "EasyPrefsSettings.generated.h"


UCLASS(Config=Game, DefaultConfig)
class UEasyPrefsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UEasyPrefsSettings();

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

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

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif

private:
	UPROPERTY(meta=(BaseStruct="/Script/EasyPrefs.EasyPrefCVar", ExcludeBaseStruct))
	TArray<FInstancedStruct> CVars;

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

	const TArray<FInstancedStruct>& GetCVars() const
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
};
