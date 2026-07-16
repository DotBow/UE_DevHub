// Copyright (C) Dreamer's Tail

#pragma once
#include "UObject/Object.h"
#include "UObject/SoftObjectPath.h"
#include "GamePulseSettings.generated.h"

class UGamePulseWidget;


UCLASS(Config=GamePulse, MinimalAPI, meta=(DisplayName="Game Pulse"))
class UGamePulseSettings : public UObject
{
	GENERATED_BODY()

public:
	UGamePulseSettings();

	static const ThisClass& Get()
	{
		return *GetDefault<ThisClass>();
	}

	static bool GetConfigPath(
		FString& OutConfigFilePath);

private:
	UPROPERTY(EditAnywhere, Config,
		meta=(AllowedClasses="/Script/UMGEditor.WidgetBlueprint"))
	FSoftObjectPath GamePulseWidget;

public:
	UClass* GetGamePulseWidgetClass() const;
};
