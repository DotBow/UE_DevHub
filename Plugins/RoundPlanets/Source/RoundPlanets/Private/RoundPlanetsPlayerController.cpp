// Copyright (C) Dreamer's Tail

#include "RoundPlanetsPlayerController.h"
#include "RoundPlanetsPlayerControllerComponent.h"


ARoundPlanetsPlayerController::ARoundPlanetsPlayerController()
{
	CustomGravityPlayerControllerComponent = CreateDefaultSubobject<
		URoundPlanetsPlayerControllerComponent>("CustomGravityPlayerControllerComponent");
}

void ARoundPlanetsPlayerController::UpdateRotation(
	float DeltaTime)
{
	CustomGravityPlayerControllerComponent->UpdateRotationImpl(this, DeltaTime);
}
