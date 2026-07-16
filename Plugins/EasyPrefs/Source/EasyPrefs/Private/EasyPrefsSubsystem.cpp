// Copyright (C) Dreamer's Tail

#include "EasyPrefsSubsystem.h"

#include "EasyPrefsCVars.h"


void UEasyPrefsSubsystem::Initialize(
	FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ApplyAllSettigns();
}

void UEasyPrefsSubsystem::ApplyAllSettigns()
{
	for (FEasyPrefCVar CVar : EasyPrefsCVars::AllVars)
	{
		CVar.ApplyCustomCVar();
	}
}
