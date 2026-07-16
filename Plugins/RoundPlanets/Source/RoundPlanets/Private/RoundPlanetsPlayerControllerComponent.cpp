// Copyright (C) Dreamer's Tail

#include "RoundPlanetsPlayerControllerComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


URoundPlanetsPlayerControllerComponent::URoundPlanetsPlayerControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URoundPlanetsPlayerControllerComponent::UpdateRotationImpl(
	APlayerController* Controller,
	float DeltaTime)
{
	FVector GravityDirection = FVector::DownVector;

	if (const ACharacter* PlayerCharacter = Cast<ACharacter>(Controller->GetPawn()))
	{
		if (const UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}
	else
	{
		if (Controller->GetPawn())
		{
			GravityDirection = Controller->GetPawn()->GetActorUpVector() * -1.0f;
		}
	}

	// Get the current control rotation in world space
	FRotator ViewRotation = Controller->GetControlRotation();

	// Add any rotation from the gravity changes, if any happened.
	// Delete this code block if you don't want the camera to automatically compensate for gravity rotation.
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, GravityDirection);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);

		ViewRotation = WarpedCameraRotation.Rotator();
	}

	LastFrameGravity = GravityDirection;

	// Convert the view rotation from world space to gravity relative space.
	// Now we can work with the rotation as if no custom gravity was affecting it.
	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);

	// Calculate Delta to be applied on ViewRotation
	FRotator DeltaRot(Controller->RotationInput);

	if (Controller->PlayerCameraManager)
	{
		Controller->PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);

		// Zero the roll of the camera as we always want it horizontal in relation to the gravity.
		ViewRotation.Roll = 0;

		// Convert the rotation back to world space, and set it as the current control rotation.
		Controller->SetControlRotation(GetGravityWorldRotation(ViewRotation, GravityDirection));
	}

	if (APawn* const Pawn = Controller->GetPawnOrSpectator())
		Pawn->FaceRotation(ViewRotation, DeltaTime);
}

FRotator URoundPlanetsPlayerControllerComponent::GetGravityRelativeRotation(
	const FRotator& Rotation,
	const FVector& GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		const FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

FRotator URoundPlanetsPlayerControllerComponent::GetGravityWorldRotation(
	const FRotator& Rotation,
	const FVector& GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		const FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}
