// Copyright (C) Dreamer's Tail

#pragma once
#include "EasyPrefsCVars.generated.h"


USTRUCT()
struct FEasyPrefCVar
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FString CustomCVarName;

	UPROPERTY()
	TArray<FString> EngineCVarNames;

public:
	FEasyPrefCVar(
		const FString& InCustomCVarName = TEXT(""),
		const TArray<FString>& InEngineCVarNames = {})
		: CustomCVarName(InCustomCVarName)
		, EngineCVarNames(InEngineCVarNames)
	{}

	void ApplyCustomCVar();
};


namespace EasyPrefsCVars
{
	extern TAutoConsoleVariable<int32> CVarEasyPrefsMaxFPS;
	extern TAutoConsoleVariable<float> CVarEasyPrefsPanini;

	extern TArray<FEasyPrefCVar> AllVars;
}
