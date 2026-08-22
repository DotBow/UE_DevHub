// Copyright (C) Dreamer's Tail

#include "EasyPrefsSubsystem.h"

#include "EasyPrefsCVars.h"
#include "EasyPrefsSettings.h"


void UEasyPrefsSubsystem::Initialize(
	FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ApplyAllSettigns();
}

void UEasyPrefsSubsystem::ApplyAllSettigns()
{
	if (UEasyPrefsSettings::Get())
	{
		/*for (const FInstancedStruct& CVar : UEasyPrefsSettings::Get()->GetCVars())
		{
			if (const FEasyPrefsCVar* CVarPtr = CVar.GetPtr<FEasyPrefsCVar>())
				CVarPtr->Apply();
		}*/
	}
}
