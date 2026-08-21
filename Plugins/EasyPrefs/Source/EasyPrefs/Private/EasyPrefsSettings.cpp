// Copyright Epic Games, Inc. All Rights Reserved.
#include "EasyPrefsSettings.h"

#if WITH_EDITOR
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"
#endif


UEasyPrefsSettings::UEasyPrefsSettings()
{
	CVars =
	{
		MaxFPS,

		Panini,

		ScreenPercentage,
		UpscaleQuality,
		UpscaleSharpeningQuality,

		AntiAliasingMethod,
		MSAACount
	};
}

#if WITH_EDITOR
FText UEasyPrefsSettings::GetSectionText() const
{
	return NSLOCTEXT("UEasyPrefsSettings", "SectionText", "Easy Prefs");
}

void UEasyPrefsSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
