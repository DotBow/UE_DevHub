// Copyright (C) Dreamer's Tail

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamePulseBlueprintLibrary.generated.h"


UCLASS()
class GAMEPULSE_API UGamePulseBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* Returns used video memory in megabytes */
	UFUNCTION(BlueprintCallable, Category = "StatProvider", BlueprintPure)
	static float GetUsedVideoMemory();

	/* Returns memory used by system in megabytes */
	UFUNCTION(BlueprintCallable, Category = "StatProvider", BlueprintPure)
	static float GetUsedMemory();

	/* Returns memory used bu Unreal process in megabytes */
	UFUNCTION(BlueprintCallable, Category = "StatProvider", BlueprintPure)
	static float GetProcessMemoryUsage();

	/* Returns CPU usage in 0-1 range */
	UFUNCTION(BlueprintCallable, Category = "StatProvider", BlueprintPure)
	static double GetCPULoad();

	/* Returns GPU usage in 0-1 range */
	UFUNCTION(BlueprintCallable, Category = "StatProvider", BlueprintPure)
	static int GetGPULoad();
};
