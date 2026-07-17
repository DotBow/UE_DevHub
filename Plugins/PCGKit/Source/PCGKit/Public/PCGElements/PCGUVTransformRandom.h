// Copyright (C) Dreamer's Tail

#pragma once

#include "Elements/PCGDynamicMeshBaseElement.h"
#include "Math/TransformCalculus2D.h"
#include "PCGUVTransformRandom.generated.h"


/**
* Do a boolean operation between dynamic meshes.
*/
UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGUVTransformRandomSettings : public UPCGDynamicMeshBaseSettings
{
	GENERATED_BODY()

public:
	//~Begin UPCGSettings interface
#if WITH_EDITOR
	virtual FName GetDefaultNodeName() const override;
	virtual FText GetDefaultNodeTitle() const override;
	virtual FText GetNodeTooltipText() const override;
#endif

protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override;
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings interface

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	int32 UVSetIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	FVector2D TranslateMin = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	FVector2D TranslateMax = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	FVector2D ScaleMin = FVector2D(1.f, 1.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	FVector2D ScaleMax = FVector2D(1.f, 1.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	float RotationAngleDegMin = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	float RotationAngleDegMax = 0.f;
};


class FPCGUVTransformRandomElement : public IPCGDynamicMeshBaseElement
{
protected:
	virtual bool ExecuteInternal(FPCGContext* InContext) const override;

	static class UDynamicMesh* SetMeshUVElementPosition(
		UDynamicMesh* TargetMesh,
		const int32 UVSetIndex,
		const float RotationAngleDegMin,
		const float RotationAngleDegMax,
		const FVector2f& ScaleMin,
		const FVector2f& ScaleMax,
		const FVector2f& TranslateMin,
		const FVector2f& TranslateMax);
};
