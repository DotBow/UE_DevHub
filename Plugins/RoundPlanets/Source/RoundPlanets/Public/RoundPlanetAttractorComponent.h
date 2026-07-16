// Copyright (C) Dreamer's Tail

#pragma once

#include "Components/SceneComponent.h"
#include "RoundPlanetAttractorComponent.generated.h"

struct FGravityAttractorData;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUNDPLANETS_API URoundPlanetAttractorComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	URoundPlanetAttractorComponent();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

	FGravityAttractorData GetGravityAttractorData() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attractor")
	bool bUseGravityAtRadius = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attractor",
		meta = (ForceUnits="Kg", ClampMin = "1", EditConditionHides, EditCondition = "!bUseGravityAtRadius"))
	double Mass = 5.9722E24;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attractor",
		meta = (EditConditionHides, EditCondition = "bUseGravityAtRadius"))
	double Gravity = 981.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attractor",
		meta = (EditConditionHides, EditCondition = "bUseGravityAtRadius"))
	double Radius = 5000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attractor")
	bool ApplyGravity = true;

protected:
	virtual void BeginPlay() override;

private:
	// Physics Thread Communications
	// Construct the Data to be sent to the PT
	virtual void BuildAsyncInput();
};
