// Copyright (C) Dreamer's Tail

#include "EasyPrefsCVars.h"


void FEasyPrefsCVar::Apply() const
{
	IConsoleVariable* EngineCVar =
	IConsoleManager::Get().FindConsoleVariable(*Name);

	if (EngineCVar && (EngineCVar->GetFlags() & ECVF_ReadOnly) == 0)
		EngineCVar->Set(*GetValue(), ECVF_SetByProjectSetting);
}
