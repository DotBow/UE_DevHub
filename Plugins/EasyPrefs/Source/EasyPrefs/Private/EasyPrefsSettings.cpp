// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyPrefsSettings.h"


UEasyPrefsSettings::UEasyPrefsSettings()
{
	CVars =
	{
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, MaxFPS),
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, Panini),
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, ScreenPercentage),
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, UpscaleQuality),
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, UpscaleSharpeningQuality),
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, AntiAliasingMethod),
		GET_MEMBER_NAME_CHECKED(UEasyPrefsSettings, MSAACount)
	};
}
