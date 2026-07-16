// Copyright (C) Dreamer's Tail

#include "RoundPlanetAttractorComponent.h"
#include "RoundPlanetsAsyncCallback.h"
#include "RoundPlanetsSubsystem.h"


URoundPlanetAttractorComponent::URoundPlanetAttractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics; // Important, we need to be ticked before the Physics Thread to send the attractor location!
}

void URoundPlanetAttractorComponent::BeginPlay()
{
	Super::BeginPlay();
}

FGravityAttractorData URoundPlanetAttractorComponent::GetGravityAttractorData() const
{
	FGravityAttractorData GravityAttractorData;
	GravityAttractorData.Location = GetComponentLocation();

	if (bUseGravityAtRadius)
	{
		GravityAttractorData.MassDotG = Gravity*Radius*Radius;
	}
	else
	{
		GravityAttractorData.MassDotG = Mass * 6.67430E-5 ; // G = 6.67430E-11 m³kg⁻¹s⁻², because 1m equals 100 UE Units, we have to multiply by a 100³ factor, so E-11 goes E-5
	}

	return GravityAttractorData;
}

void URoundPlanetAttractorComponent::BuildAsyncInput()
{
	if (ApplyGravity) // Publish attractor data only if active
	{
		if (const UWorld* World = GetWorld())
		{
			if (const URoundPlanetsSubsystem* GravitySubsystem = World->GetSubsystem<URoundPlanetsSubsystem>())
			{
				GravitySubsystem->AddGravityAttractorData(GetGravityAttractorData());
			}
		}
	}
}

// Called every frame
void URoundPlanetAttractorComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BuildAsyncInput();
}

void URoundPlanetAttractorComponent::OnRegister()
{
	Super::OnRegister();

	if (const UWorld* World = GetWorld())
	{
		if (URoundPlanetsSubsystem* GravitySubsystem = World->GetSubsystem<URoundPlanetsSubsystem>())
		{
			GravitySubsystem->AddAttractor(this);
		}
	}
}

void URoundPlanetAttractorComponent::OnUnregister()
{
	if (const UWorld* World = GetWorld())
	{
		if (URoundPlanetsSubsystem* GravitySubsystem = World->GetSubsystem<URoundPlanetsSubsystem>())
		{
			GravitySubsystem->RemoveAttractor(this);
		}
	}

	Super::OnUnregister();
}
