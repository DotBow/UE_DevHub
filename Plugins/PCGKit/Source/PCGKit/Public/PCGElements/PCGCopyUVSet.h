// Copyright (C) Dreamer's Tail

#pragma once

#include "Elements/PCGDynamicMeshBaseElement.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "Math/TransformCalculus2D.h"
#include "PCGCopyUVSet.generated.h"


/**
* Do a boolean operation between dynamic meshes.
*/
UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGCopyUVSetSettings : public UPCGDynamicMeshBaseSettings
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
	int32 FromUVSet = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	int32 ToUVSet = 1;
};


class FPCGCopyUVSetElement : public IPCGDynamicMeshBaseElement
{
protected:
	virtual bool ExecuteInternal(FPCGContext* InContext) const override;

	virtual bool IsCacheable(const UPCGSettings* InSettings) const override
	{
		return false;
	}
};
