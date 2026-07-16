// Copyright (C) Dreamer's Tail

#pragma once

#include "GameFramework/PlayerController.h"
#include "RoundPlanetsPlayerController.generated.h"


UCLASS()
class ROUNDPLANETS_API ARoundPlanetsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARoundPlanetsPlayerController();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class URoundPlanetsPlayerControllerComponent> CustomGravityPlayerControllerComponent;

	virtual void UpdateRotation(
		float DeltaTime) override;
};
