// Copyright (C) Dreamer's Tail

#pragma once

#include "Blueprint/UserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "GamePulseGraphWidget.generated.h"


UCLASS()
class GAMEPULSE_API UGamePulseGraphWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<float> FrameTimes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D Size = FVector2D(256, 128);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxNumFrames = 64;

	UFUNCTION(BlueprintCallable)
	void AddFrameTime(const float& InFrameTime);

	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	TArray<float> OnGetFrameTimes() const;
};


DECLARE_DELEGATE_RetVal(TArray<float>, FGetFrameTimes);

class GAMEPULSE_API SGamePulseGraphWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamePulseGraphWidget) {}
		SLATE_ARGUMENT(FVector2D, Size)
		SLATE_ARGUMENT(int32, MaxNumFrames)
		SLATE_EVENT(FGetFrameTimes, OnGetFrameTimes)
	SLATE_END_ARGS()

	void Construct(
		const FArguments& InArgs);

private:
	FVector2D Size = FVector2D(256, 128);
	int32 MaxNumFrames = 64;

	FGetFrameTimes OnGetFrameTimes;

	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
};
