// Copyright (C) Dreamer's Tail

#pragma once

#include "Blueprint/UserWidget.h"
#include "GamePulseWidget.generated.h"


DECLARE_DELEGATE_RetVal(float, FGetDeltaTime);


UENUM()
enum class EGamePulseAnchors
{
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	CenterCenter,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};


UCLASS()
class GAMEPULSE_API UGamePulseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	float GetDeltaTime() const;

	FGetDeltaTime OnGetDeltaTime;

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateFPS();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateStats();

	static TMap<EGamePulseAnchors, FAnchors> Anchors;

	void SetAnchors(
		const EGamePulseAnchors& InAnchors);
	void SetAlignment(
		const EGamePulseAnchors& InAnchors);
};
