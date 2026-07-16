// Copyright (C) Dreamer's Tail

#pragma once

#include "Curves/CurveFloat.h"
#include "StructUtils/InstancedStruct.h"
#include "AnimatickData.generated.h"


UENUM()
enum class EAnimatickPlayMode : uint8
{
	FromStart,
	FromEnd,
	PingPong
};


UENUM()
enum class EAnimatickPlayState : uint8
{
	MinTime,
	Playing,
	MaxTime
};


UENUM()
enum class EAnimatickPlayDirection : uint8
{
	Forward,
	Backward
};


UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FAnimatickCallback, float, OutValue);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimatickStartCallback);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimatickEndCallback);


USTRUCT(BlueprintType, Blueprintable)
struct ANIMATICK_API FAnimatickData
{
	GENERATED_BODY()

	FAnimatickData();

	UPROPERTY()
	FGuid Guid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve Curve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimatickCallback AnimationCallback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimatickStartCallback StartCallback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimatickEndCallback EndCallback;

	bool IsValid() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	EAnimatickPlayMode PlayMode;

	UPROPERTY(BlueprintReadOnly)
	EAnimatickPlayState PlayState;

	UPROPERTY(BlueprintReadOnly)
	EAnimatickPlayDirection PlayDirection;

	void UpdatePlayState();

	/* Current time along curve */
	UPROPERTY()
	float CurrentTime = 0.0f;

	/* Cached min time */
	float MinTime = 0.0f;

	/* Cached max time */
	float MaxTime = 0.0f;

public:
	void Init(
		const EAnimatickPlayMode InPlayMode = EAnimatickPlayMode::FromStart);

	void AnimationTick(
		const float DeltaTime);
	bool IsFinished() const;

	bool operator==(const FAnimatickData& InA) const
	{
		return InA.Guid == Guid;
	}
};
