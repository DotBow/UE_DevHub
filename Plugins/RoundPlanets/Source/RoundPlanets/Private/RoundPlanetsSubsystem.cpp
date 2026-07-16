// Copyright (C) Dreamer's Tail

#include "RoundPlanetsSubsystem.h"

#include "RoundPlanetsAsyncCallback.h"
#include "EngineUtils.h"
#include "PBDRigidsSolver.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "GameFramework/CharacterMovementComponent.h"


/// Base Class overrides
void URoundPlanetsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UWorld* World = GetWorld())
	{
		// Register delegates to track the ones created/destroyed on the fly
		ActorSpawnedHandle = World->AddOnActorSpawnedHandler(
			FOnActorSpawned::FDelegate::CreateUObject(this, &URoundPlanetsSubsystem::AddActorToTrackedCharacters));
		ActorDestroyedHandle = World->AddOnActorDestroyedHandler(
			FOnActorDestroyed::FDelegate::CreateUObject(this, &URoundPlanetsSubsystem::RemoveActorFromTrackedCharacters));
	}
}

void URoundPlanetsSubsystem::Deinitialize()
{
	if (const UWorld* World = GetWorld())
	{
		World->RemoveOnActorSpawnedHandler(ActorSpawnedHandle);
		World->RemoveOnActorDestroyedHandler(ActorDestroyedHandle);
	}

	TrackedCharacterMovementComponents.Empty();

	Super::Deinitialize();
}

void URoundPlanetsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, *FString::Printf(TEXT("%d Attractors in the World"), Attractors.Num()));

	if (!IsAsyncCallbackRegistered())
	{
		RegisterAsyncCallback();
	}

	// Find the existing Actors with a Character Movement Component
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AddActorToTrackedCharacters(*ActorItr);
	}
}

TStatId URoundPlanetsSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCustomGravityWorldSubSystem, STATGROUP_Tickables);
}

void URoundPlanetsSubsystem::UpdateCMCGravities()
{
	//for (int i = 0; i < TrackedCharacterMovementComponents.Num(); i++)
	for (const auto CMComponent : TrackedCharacterMovementComponents)
	{
		// Compute the 
		FVector AdditionalAcceleration = FVector::ZeroVector; 
		for (const auto& GravityAttractor : Attractors)
		{
			if (GravityAttractor->ApplyGravity)
			{
				FGravityAttractorData GravityAttractorData = GravityAttractor->GetGravityAttractorData();

				// Direction
				FVector Direction(GravityAttractorData.Location - CMComponent->GetActorLocation());
				const double SquaredDistance = FVector::DotProduct(Direction, Direction); // We'll be using UE units here, no meters... 
				Direction.Normalize();

				// Intensity
				const double Intensity = GravityAttractorData.MassDotG / SquaredDistance;

				// Add the new acceleration to the force field.
				AdditionalAcceleration += Intensity * Direction;
			}
		}

		//DrawDebugDirectionalArrow(GetWorld(), CMComponent->GetActorLocation(), CMComponent->GetActorLocation() + AdditionalAcceleration, 1.0, FColor::Red, 0, false, 1.0f  );
		//DrawDebugString(GetWorld(), CMComponent->GetActorLocation(), * FString::Printf(TEXT("%.2f"), AdditionalAcceleration.Length()), nullptr, FColor::Red, 0, false, 1.0f  );

		CMComponent->AddForce(AdditionalAcceleration * CMComponent->Mass);
		CMComponent->SetGravityDirection(AdditionalAcceleration.GetSafeNormal());
	}
}

void URoundPlanetsSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCMCGravities();
}

void URoundPlanetsSubsystem::RegisterAsyncCallback()
{
	if (const UWorld* World = GetWorld())
	{
		if (const FPhysScene* PhysScene = World->GetPhysicsScene())
		{
			AsyncCallback = PhysScene->GetSolver()->CreateAndRegisterSimCallbackObject_External<FRoundPlanetsAsyncCallback>();
		}
	}
}

bool URoundPlanetsSubsystem::IsAsyncCallbackRegistered() const
{
	return AsyncCallback != nullptr;
}

void URoundPlanetsSubsystem::AddGravityAttractorData(const FGravityAttractorData& InputData) const
{
	if (IsAsyncCallbackRegistered())
	{
		FCustomGravityAsyncInput* Input = AsyncCallback->GetProducerInputData_External();
		Input->GravityAttractorsData.Add(InputData);
	}
}

void URoundPlanetsSubsystem::AddAttractor(URoundPlanetAttractorComponent* GravityAttractorComponent)
{
	Attractors.Add(GravityAttractorComponent);
}

void URoundPlanetsSubsystem::RemoveAttractor(URoundPlanetAttractorComponent* GravityAttractorComponent)
{
	Attractors.Remove(GravityAttractorComponent);
}

void URoundPlanetsSubsystem::AddActorToTrackedCharacters(AActor* Actor)
{
	if (!Actor)
		return;

	Actor->ForEachComponent<UCharacterMovementComponent>(
		true, [this](UCharacterMovementComponent* CMComponent)
	{
		if (CMComponent)
			TrackedCharacterMovementComponents.Add(CMComponent);
	});
}

void URoundPlanetsSubsystem::RemoveActorFromTrackedCharacters(AActor* Actor)
{
	if (!Actor)
		return;

	Actor->ForEachComponent<UCharacterMovementComponent>(
		true, [this](UCharacterMovementComponent* CMComponent)
	{
		if (CMComponent)
			TrackedCharacterMovementComponents.Remove(CMComponent);
	});
}
