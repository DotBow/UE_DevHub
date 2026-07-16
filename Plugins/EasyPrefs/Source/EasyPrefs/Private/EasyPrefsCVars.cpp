// Copyright (C) Dreamer's Tail

#include "EasyPrefsCVars.h"


void FEasyPrefCVar::ApplyCustomCVar()
{
	if (const IConsoleVariable* CustomCVar =
		IConsoleManager::Get().FindConsoleVariable(*CustomCVarName))
	{
		const FString OutValue = CustomCVar->GetString();

		for (const FString EngineCVarName : EngineCVarNames)
		{
			IConsoleVariable* EngineCVar =
				IConsoleManager::Get().FindConsoleVariable(*EngineCVarName);

			if (EngineCVar && (EngineCVar->GetFlags() & ECVF_ReadOnly) == 0)
				EngineCVar->Set(*OutValue, ECVF_SetByProjectSetting);
		}
	}
}


namespace EasyPrefsCVars
{
	TAutoConsoleVariable<int32> CVarEasyPrefsMaxFPS(
		TEXT("EasyPrefs.MaxFPS"),
		60,
		TEXT(""),
		ECVF_RenderThreadSafe);

	FEasyPrefCVar EasyPrefsMaxFPS(
		TEXT("EasyPrefs.MaxFPS"),
		{ TEXT("t.MaxFPS") });

	TAutoConsoleVariable<float> CVarEasyPrefsPanini(
		TEXT("EasyPrefs.Panini"),
		0.1f,
		TEXT(""),
		ECVF_RenderThreadSafe);

	FEasyPrefCVar EasyPrefsPanini(
		TEXT("EasyPrefs.Panini"),
		{ TEXT("r.Upscale.Panini.D") });

	TArray<FEasyPrefCVar> AllVars =
	{
		EasyPrefsMaxFPS,
		EasyPrefsPanini
	};
}
