// Copyright (C) Dreamer's Tail

#include "GamePulseWidget.h"


TMap<EGamePulseAnchors, FAnchors> UGamePulseWidget::Anchors =
{
	{ EGamePulseAnchors::TopLeft,      FAnchors(0,   0,   0,   0) },
	{ EGamePulseAnchors::TopCenter,    FAnchors(0.5, 0,   0.5, 0) },
	{ EGamePulseAnchors::TopRight,     FAnchors(1,   0,   1,   0) },
	{ EGamePulseAnchors::CenterLeft,   FAnchors(0,   0.5, 0,   0.5) },
	{ EGamePulseAnchors::CenterCenter, FAnchors(0.5, 0.5, 0.5, 0.5) },
	{ EGamePulseAnchors::CenterRight,  FAnchors(1,   0.5, 1,   0.5) },
	{ EGamePulseAnchors::BottomLeft,   FAnchors(0,   1,   0,   1) },
	{ EGamePulseAnchors::BottomCenter, FAnchors(0.5, 1,   0.5, 1) },
	{ EGamePulseAnchors::BottomRight,  FAnchors(1,   1,   1,   1) },
};


float UGamePulseWidget::GetDeltaTime() const
{
	if (OnGetDeltaTime.IsBound())
		return OnGetDeltaTime.Execute();

	return -1.f;
}

void UGamePulseWidget::SetAnchors(
	const EGamePulseAnchors& InAnchors)
{
	if (Anchors.Contains(InAnchors))
		SetAnchorsInViewport(Anchors[InAnchors]);
}

void UGamePulseWidget::SetAlignment(
	const EGamePulseAnchors& InAnchors)
{
	if (Anchors.Contains(InAnchors))
	{
		const FAnchors NewAnchors = Anchors[InAnchors];
		const FVector2D NewAlignment = FVector2D(
			NewAnchors.IsStretchedHorizontal() ? 0.0f : NewAnchors.Minimum.X,
			NewAnchors.IsStretchedVertical() ? 0.0f : NewAnchors.Minimum.Y);
		SetAlignmentInViewport(NewAlignment);
	}
}
