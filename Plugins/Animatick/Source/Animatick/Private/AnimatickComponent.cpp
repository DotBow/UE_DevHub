// Copyright (C) Dreamer's Tail

#include "AnimatickComponent.h"

#include "Animatick.h"
#include "AnimatickData.h"
#include "AnimatickSubsystem.h"


UAnimatickComponent::UAnimatickComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AnimatickData = FInstancedStruct::Make(FAnimatickData());
}

void UAnimatickComponent::BeginPlay()
{
	Super::BeginPlay();

	GetAnimatickDataPtr()->AnimationCallback = AnimationCallback;
	GetAnimatickDataPtr()->StartCallback = StartCallback;
	GetAnimatickDataPtr()->EndCallback = EndCallback;
}

FInstancedStruct& UAnimatickComponent::GetAnimatickDataRef()
{
	return AnimatickData;
}

FAnimatickData* UAnimatickComponent::GetAnimatickDataPtr()
{
	return AnimatickData.GetMutablePtr<FAnimatickData>();
}

void UAnimatickComponent::PlayCurveAnimation(const EAnimatickPlayMode InPlayMode)
{
	AnimatickSubsystem->PlayCurveAnimation(GetAnimatickDataRef());
}
