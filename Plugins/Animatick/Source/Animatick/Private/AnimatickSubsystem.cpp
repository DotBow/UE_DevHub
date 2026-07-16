// Copyright (C) Dreamer's Tail

#include "AnimatickSubsystem.h"
#include "Animatick.h"


void UAnimatickSubsystem::Initialize(
	FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AnimatickSubsystem = this;
}

void UAnimatickSubsystem::Deinitialize()
{
	AnimatickSubsystem = nullptr;

	Super::Deinitialize();
}

TStatId UAnimatickSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(
		AnimatickSubsystem, STATGROUP_Tickables);
}

void UAnimatickSubsystem::Tick(
	const float DeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(AnimatickSubsystemTick)

	for (int32 AnimationIdx = CurveAnimations.Num() - 1;
		AnimationIdx >= 0; --AnimationIdx)
	{
		FAnimatickData* AnimatickData = CurveAnimations[AnimationIdx].GetMutablePtr<FAnimatickData>();
		AnimatickData->AnimationTick(DeltaTime);

		if (AnimatickData->IsFinished())
		{
			AnimatickData->EndCallback.Broadcast();
			CurveAnimations.RemoveAtSwap(AnimationIdx);
		}
	}

	Super::Tick(DeltaTime);
}

bool UAnimatickSubsystem::ContainsCurveAnimation(
	const FAnimatickData* InAnimationData)
{
	for (const FInstancedStruct& Struct : CurveAnimations)
	{
		if (const FAnimatickData* AnimatickData = Struct.GetPtr<FAnimatickData>())
		{
			if (AnimatickData == InAnimationData)
				return true;
		}
	}

	return false;
}

void UAnimatickSubsystem::PlayCurveAnimation(
	FInstancedStruct& InAnimationData,
	const EAnimatickPlayMode InPlayMode)
{
	FAnimatickData* AnimatickData = InAnimationData.GetMutablePtr<FAnimatickData>();

	if (AnimatickData->IsValid())
	{
		AnimatickData->Init(InPlayMode);

		if (!ContainsCurveAnimation(AnimatickData))
		{
			CurveAnimations.Add(InAnimationData);
			AnimatickData->StartCallback.Broadcast();
		}
	}
}
