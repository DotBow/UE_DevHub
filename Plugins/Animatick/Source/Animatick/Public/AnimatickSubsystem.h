// Copyright (C) Dreamer's Tail

#pragma once

#include "AnimatickData.h"
#include "StructUtils/InstancedStruct.h"
#include "Subsystems/WorldSubsystem.h"
#include "AnimatickSubsystem.generated.h"


UCLASS(Category=Animatick)
class ANIMATICK_API UAnimatickSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY(meta=(BaseStruct="/Script/Animatick.AnimatickData"))
	TArray<FInstancedStruct> CurveAnimations;

	bool ContainsCurveAnimation(
		const FAnimatickData* InAnimationData);

public:
	virtual void Initialize(
		FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual TStatId GetStatId() const override;
	virtual void Tick(
		float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Animatick)
	void PlayCurveAnimation(
		UPARAM(ref) FInstancedStruct& InAnimationData,
		const EAnimatickPlayMode InPlayMode = EAnimatickPlayMode::FromStart);
};
