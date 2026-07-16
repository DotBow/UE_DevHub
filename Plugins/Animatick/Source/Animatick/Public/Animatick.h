// Copyright (C) Dreamer's Tail

#pragma once
#include "Modules/ModuleInterface.h"


extern ANIMATICK_API class UAnimatickSubsystem* AnimatickSubsystem;

class FAnimatickModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
