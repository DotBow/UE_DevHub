// Copyright (C) Dreamer's Tail

#pragma once

#include "RoundPlanetAttractorComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "RoundPlanetsSubsystem.generated.h"

class FRoundPlanetsAsyncCallback;
class UCharacterMovementComponent;
struct FGravityAttractorData; 


UCLASS()
class ROUNDPLANETS_API URoundPlanetsSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	// UTickableWorldSubsystem overrides
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;
	void UpdateCMCGravities();

	virtual void Tick(float DeltaTime) override;

	// Register async callback with physics system.
	virtual void RegisterAsyncCallback(); 
	virtual bool IsAsyncCallbackRegistered() const;
	void AddGravityAttractorData(const FGravityAttractorData& InputData) const;
	FRoundPlanetsAsyncCallback* AsyncCallback = nullptr;

	// Keep track of any attractors (optional)
	void AddAttractor(URoundPlanetAttractorComponent* GravityAttractorComponent);
	void RemoveAttractor(URoundPlanetAttractorComponent* GravityAttractorComponent);

protected:
	// List of existing gravity attractors in the world
	TArray<TWeakObjectPtr<URoundPlanetAttractorComponent>> Attractors;

	// Track the CMC components
	void AddActorToTrackedCharacters(AActor* Actor);
	void RemoveActorFromTrackedCharacters(AActor* Actor);

	FDelegateHandle ActorSpawnedHandle;
	FDelegateHandle ActorDestroyedHandle;

	TArray<TWeakObjectPtr<UCharacterMovementComponent>> TrackedCharacterMovementComponents;
};
