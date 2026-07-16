// Copyright (C) Dreamer's Tail

#pragma once

#include "HAL/IConsoleManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "GamePulseSubsystem.generated.h"


class UGamePulseWidget;
extern class GAMEPULSE_API UGamePulseSubsystem* GamePulse;


UCLASS()
class GAMEPULSE_API UGamePulseSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(
		FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual TStatId GetStatId() const override;
	virtual void Tick(
		float DeltaTime) override;

	static TAutoConsoleVariable<bool> CVarGamePulseShow;
	static TAutoConsoleVariable<int32> CVarGamePulseAnchors;

	virtual bool IsTickableInEditor() const override
	{
		return false;
	}

	virtual bool IsTickable() const override;

private:
	virtual void OnWorldBeginPlay(
		UWorld& InWorld) override;

	static void OnSetGamePulseShow(
		IConsoleVariable* InVariable);

	static void OnSetGamePulseAnchors(
		IConsoleVariable* InVariable);

	UPROPERTY()
	UGamePulseWidget* GamePulseWidget;

	void ShowGamePulse();
	void SetGamePulseAnchors() const;
	void AddGamePulseToViewport();
	void RemoveGamePulseFromViewport();

	FDelegateHandle GamePulseShowHandle;
	FDelegateHandle GamePulseAnchorsHandle;

	float FPSTimerDelay = 0.25f;
	float StatTimerDelay = 2.f;
	float LastDeltaTime;

	FTimerHandle FPSTimerHandle;
	FTimerHandle StatTimerHandle;

	float OnGetDeltaTime() const
	{
		return LastDeltaTime;
	}
};
