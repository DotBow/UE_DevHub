// Copyright (C) Dreamer's Tail

#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "EasyPrefsSubsystem.generated.h"


UCLASS()
class EASYPREFS_API UEasyPrefsSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(
		FSubsystemCollectionBase& Collection) override;

	static void ApplyAllSettigns();
};
