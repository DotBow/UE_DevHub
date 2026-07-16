// Copyright (C) Dreamer's Tail

#pragma once

#include "AnimatickData.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "AnimatickComponent.generated.h"


UCLASS(ClassGroup=(Animation), meta=(BlueprintSpawnableComponent))
class ANIMATICK_API UAnimatickComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAnimatickComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta=(BaseStruct="/Script/Animatick.AnimatickData"), Category=Animation)
	FInstancedStruct AnimatickData;

public:
	virtual void BeginPlay() override;

	FInstancedStruct& GetAnimatickDataRef();
	FAnimatickData* GetAnimatickDataPtr();

	UFUNCTION(BlueprintCallable, Category=Animatick)
	void PlayCurveAnimation(
		const EAnimatickPlayMode InPlayMode = EAnimatickPlayMode::FromStart);

	UPROPERTY(BlueprintAssignable)
	FAnimatickCallback AnimationCallback;

	UPROPERTY(BlueprintAssignable)
	FAnimatickStartCallback StartCallback;

	UPROPERTY(BlueprintAssignable)
	FAnimatickEndCallback EndCallback;
};
