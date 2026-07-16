// Copyright (C) Dreamer's Tail

#pragma once
#include "BaseMeshKit.generated.h"


UENUM(NotBlueprintable, NotBlueprintType)
enum EBaseMeshKitType : uint8
{
	Male,
	Female
};


UCLASS(Placeable, HideCategories=(HLOD, Replication, Collision,
	Physics, Networking, Input, Actor, LevelInstance),
	meta=(ShortTooltip="Spawns a base mesh of a character to use as a reference in level"))
class PROTOKIT_API ABaseMeshKit : public AActor
{
	GENERATED_BODY()

public:
	ABaseMeshKit();

	UPROPERTY()
	TObjectPtr<class USceneComponent> CustomRoot;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBaseMeshKitType> Type;

	UPROPERTY(EditAnywhere, meta=(Units="cm"))
	float HalfHeight;

	UPROPERTY(EditAnywhere, meta=(Units="cm"))
	float Radius;

protected:
	void UpdateComponents() const;
	void UpdateType() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(
		struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
