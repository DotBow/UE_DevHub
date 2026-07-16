// Copyright (C) Dreamer's Tail

#pragma once
#include "EasyPrefsSettings.generated.h"


UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="Easy Prefs"))
class EASYPREFS_API UEasyPrefsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UEasyPrefsSettings();

	UPROPERTY(Config, EditAnywhere, Category="Scalability",
		meta=(ConsoleVariable="r.EasyPrefs.Test"))
	float Test;

private:
	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(
		FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	static const ThisClass& GetThis()
	{
		return *GetDefault<ThisClass>();
	}
};
