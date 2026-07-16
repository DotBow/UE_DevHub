// Copyright (C) Dreamer's Tail

#include "GamePulseGraphWidget.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"


void UGamePulseGraphWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

TSharedRef<SWidget> UGamePulseGraphWidget::RebuildWidget()
{
	return
	SNew(SGamePulseGraphWidget)
	.Size(Size)
	.MaxNumFrames(MaxNumFrames)
	.OnGetFrameTimes_UObject(this,
		&UGamePulseGraphWidget::OnGetFrameTimes);
}

TArray<float> UGamePulseGraphWidget::OnGetFrameTimes() const
{
	return FrameTimes;
}

void UGamePulseGraphWidget::AddFrameTime(
	const float& InFrameTime)
{
	if (FrameTimes.Num() > MaxNumFrames)
		FrameTimes.RemoveAt(0);

	FrameTimes.Add(InFrameTime);
}


void SGamePulseGraphWidget::Construct(const FArguments& InArgs)
{
	Size = InArgs._Size;
	MaxNumFrames = InArgs._MaxNumFrames;
	OnGetFrameTimes = InArgs._OnGetFrameTimes;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(Size.X)
		.HeightOverride(Size.Y)
	];
}

int32 SGamePulseGraphWidget::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	constexpr FLinearColor BgColor(0.025f, 0.025f, 0.1f, 0.25f);
	const FSlateBrush* BgImage =
		FAppStyle::GetBrush("StatusBar.ProgressOverlay");

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		BgImage,
		ESlateDrawEffect::None,
		BgColor);

	LayerId++;

	if (OnGetFrameTimes.IsBound())
	{
		constexpr FLinearColor FillColor(1, 1, 1, 1);

		TArray<FVector2D> Points;
		const TArray<float> FrameTimes =
			OnGetFrameTimes.Execute();

		for (int32 Idx = 0; Idx <= FrameTimes.Num() - 1; ++Idx)
		{
			const float FrameTime = FrameTimes[Idx];

			const float Step = Size.X / MaxNumFrames;
			float X = Idx * Step;

			if (Idx == 0)
				X += 1.f;

			if (Idx == FrameTimes.Num() - 1)
				X -= 1.f;

			const float Y = FMath::Clamp(
				Size.Y - FrameTime * Size.Y * 10.f * 2.f,
				2.f, Size.Y);

			Points.Append({
				FVector2D(X, Y) });
		}

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			FillColor,
			true);
	}

	LayerId++;

	const FSlateFontInfo FontInfo =
		FCoreStyle::Get().GetFontStyle("SmallFont");
	constexpr FLinearColor FontColor(1.f, 1.f, 1.f, 0.25f);

	FVector2D TextPosition1 = FVector2D(
		Size.X - 32,
		Size.Y - 0.033f * Size.Y * 10.f * 2.f);

	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(
			FSlateLayoutTransform(TextPosition1)),
		TEXT("33 ms"),
		FontInfo,
		ESlateDrawEffect::None,
		FontColor);

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		{
			FVector2D(1.f, TextPosition1.Y),
			FVector2D(Size.X - 1.f, TextPosition1.Y),
		},
		ESlateDrawEffect::NoBlending,
		FontColor,
		true);

	LayerId++;

	FVector2D TextPosition2 = FVector2D(
		Size.X - 32,
		Size.Y - 0.016f * Size.Y * 10.f * 2.f);

	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(
			FSlateLayoutTransform(TextPosition2)),
		TEXT("16 ms"),
		FontInfo,
		ESlateDrawEffect::None,
		FontColor);

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		{
			FVector2D(1.f, TextPosition2.Y),
			FVector2D(Size.X - 1.f, TextPosition2.Y),
		},
		ESlateDrawEffect::NoBlending,
		FontColor,
		true);

	LayerId++;

	return SCompoundWidget::OnPaint(
		Args,
		AllottedGeometry,
		MyCullingRect,
		OutDrawElements,
		LayerId,
		InWidgetStyle,
		bParentEnabled);
}
