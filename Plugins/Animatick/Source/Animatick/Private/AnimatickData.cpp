// Copyright (C) Dreamer's Tail

#include "AnimatickData.h"


FAnimatickData::FAnimatickData()
{
	Guid = FGuid::NewGuid();

	PlayRate = 1.f;
	PlayMode = EAnimatickPlayMode::FromStart;
	PlayState = EAnimatickPlayState::MinTime;
	PlayDirection = EAnimatickPlayDirection::Forward;
}

bool FAnimatickData::IsValid() const
{
	return PlayRate != 0.f && AnimationCallback.IsBound();
}

void FAnimatickData::UpdatePlayState()
{
	if (CurrentTime >= MaxTime)
		PlayState = EAnimatickPlayState::MaxTime;
	else if (CurrentTime <= MinTime)
		PlayState = EAnimatickPlayState::MinTime;
	else
		PlayState = EAnimatickPlayState::Playing;
}

void FAnimatickData::Init(
	const EAnimatickPlayMode InPlayMode)
{
	PlayMode = InPlayMode;
	Curve.GetRichCurveConst()->GetTimeRange(MinTime, MaxTime);

	switch (PlayMode)
	{
		case EAnimatickPlayMode::FromStart:
			CurrentTime = MinTime;
			PlayDirection = EAnimatickPlayDirection::Forward;
			break;
		case EAnimatickPlayMode::FromEnd:
			CurrentTime = MaxTime;
			PlayDirection = EAnimatickPlayDirection::Backward;
			break;
		case EAnimatickPlayMode::PingPong:
			switch (PlayState)
			{
				case EAnimatickPlayState::MinTime:
					PlayDirection = EAnimatickPlayDirection::Forward;
					break;
				case EAnimatickPlayState::Playing:
					if (PlayDirection == EAnimatickPlayDirection::Forward)
						PlayDirection = EAnimatickPlayDirection::Backward;
					else
						PlayDirection = EAnimatickPlayDirection::Forward;

					break;
				case EAnimatickPlayState::MaxTime:
					PlayDirection = EAnimatickPlayDirection::Backward;
					break;
			}

			break;
	}

	UpdatePlayState();
}

void FAnimatickData::AnimationTick(
	const float DeltaTime)
{
	const float Step = DeltaTime * PlayRate;

	switch (PlayDirection)
	{
		case EAnimatickPlayDirection::Forward:
			CurrentTime += Step;
			break;
		case EAnimatickPlayDirection::Backward:
			CurrentTime -= Step;
			break;
	}

	CurrentTime = FMath::Clamp(CurrentTime, MinTime, MaxTime);
	const float CurrentValue = Curve.GetRichCurveConst()->Eval(CurrentTime);

	UpdatePlayState();

	AnimationCallback.Broadcast(CurrentValue);
}

bool FAnimatickData::IsFinished() const
{
	return PlayState != EAnimatickPlayState::Playing;
}
