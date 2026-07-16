// Copyright (C) Dreamer's Tail

#include "BaseMeshKit.h"

#include "ProtoKitSettings.h"
#include "Components/CapsuleComponent.h"


ABaseMeshKit::ABaseMeshKit()
	: Type(Male)
	, HalfHeight(88.f)
	, Radius(34.f)
{
	PrimaryActorTick.bCanEverTick = false;

	CustomRoot = CreateDefaultSubobject<USceneComponent>("CustomRoot");
	CustomRoot->SetMobility(EComponentMobility::Static);
	SetRootComponent(CustomRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CustomRoot);
	MeshComponent->SetStaticMesh(UProtoKitSettings::Get()->GetBaseMesh());

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(CustomRoot);

	MeshComponent->SetDefaultCustomPrimitiveDataFloat(0, 0.f);

	UpdateComponents();
}

void ABaseMeshKit::UpdateComponents() const
{
	MeshComponent->SetWorldScale3D(FVector::One() * HalfHeight * 0.02f);

	CapsuleComponent->SetRelativeLocation(FVector::UpVector * HalfHeight);
	CapsuleComponent->SetCapsuleHalfHeight(HalfHeight);
	CapsuleComponent->SetCapsuleRadius(Radius);
}

void ABaseMeshKit::UpdateType() const
{
	MeshComponent->SetCustomPrimitiveDataFloat(0,
		Type == Male ? 0 : 1);
}

#if WITH_EDITOR
void ABaseMeshKit::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABaseMeshKit, HalfHeight) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ABaseMeshKit, Radius))
	{
		UpdateComponents();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABaseMeshKit, Type))
	{
		UpdateType();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
