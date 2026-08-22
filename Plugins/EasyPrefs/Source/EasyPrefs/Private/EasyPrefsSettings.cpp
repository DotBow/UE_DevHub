// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyPrefsSettings.h"


UEasyPrefsSettings::UEasyPrefsSettings()
{
	CVars =
	{
		MaxFPSCVar,
		PaniniCVar,
		ScreenPercentageCVar,
		UpscaleQualityCVar,
		UpscaleSharpeningQualityCVar,
		AntiAliasingMethodCVar,
		MSAACountCVar
	};
}


TMap<FName, int32> UEasyPrefsSettings::AntiAliasingMethodEnum =
{
	{ TEXT("Disabled"), 0 },
	{ TEXT("FXAA"),     1 },
	{ TEXT("TAA"),      2 },
	{ TEXT("MSAA"),     3 },
	{ TEXT("TSR"),      4 },
	{ TEXT("SMAA"),     5 }
};


TMap<FName, int32> UEasyPrefsSettings::MSAACountEnum =
{
	{ TEXT("Disabled (TAA)"), 0 },
	{ TEXT("Disabled"),       1 },
	{ TEXT("2x MSAA"),        2 },
	{ TEXT("4x MSAA"),        4 },
	{ TEXT("8x MSAA"),        8 }
};
