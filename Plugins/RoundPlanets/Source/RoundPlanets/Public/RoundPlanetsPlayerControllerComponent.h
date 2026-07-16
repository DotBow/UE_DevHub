// Copyright (C) Dreamer's Tail

#pragma once

#include "Components/ActorComponent.h"
#include "RoundPlanetsPlayerControllerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUNDPLANETS_API URoundPlanetsPlayerControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URoundPlanetsPlayerControllerComponent();

	void UpdateRotationImpl(
		APlayerController* Controller,
		float DeltaTime);

	// Converts a rotation from world space to gravity relative space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(
		const FRotator& Rotation,
		const FVector& GravityDirection);

	// Converts a rotation from gravity relative space to world space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(
		const FRotator& Rotation,
		const FVector& GravityDirection);

private:
	FVector LastFrameGravity = FVector::ZeroVector;
};
