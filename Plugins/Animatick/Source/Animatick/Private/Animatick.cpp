// Copyright (C) Dreamer's Tail

#include "Animatick.h"


#define LOCTEXT_NAMESPACE "AnimatickModule"

UAnimatickSubsystem* AnimatickSubsystem;

void FAnimatickModule::StartupModule()
{
}

void FAnimatickModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAnimatickModule, Animatick)
