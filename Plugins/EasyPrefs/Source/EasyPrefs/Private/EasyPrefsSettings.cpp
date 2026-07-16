// Copyright (C) Dreamer's Tail

#include "EasyPrefsSettings.h"
#include "Misc/ConfigUtilities.h"


UEasyPrefsSettings::UEasyPrefsSettings()
{
	CategoryName = TEXT("Game");
}

void UEasyPrefsSettings::PostInitProperties()
{
	Super::PostInitProperties();

	UE::ConfigUtilities::ApplyCVarSettingsFromIni(
		TEXT("/Script/EasyPrefs.EasyPrefsSettings"),
		*GEngineIni, ECVF_SetByProjectSetting);
}

#if WITH_EDITOR
void UEasyPrefsSettings::PostEditChangeProperty(
	FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.ChangeType != EErrorReportMode::Interactive)
	{
		ExportValuesToConsoleVariables(PropertyChangedEvent.Property);
	}
}
#endif
