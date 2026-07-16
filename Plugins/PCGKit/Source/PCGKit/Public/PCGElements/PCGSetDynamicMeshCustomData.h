// Copyright (C) Dreamer's Tail

#pragma once

#include "Elements/PCGDynamicMeshBaseElement.h"
#include "Engine/CollisionProfile.h"
#include "Math/TransformCalculus2D.h"
#include "PCGSetDynamicMeshCustomData.generated.h"


enum ECollisionTraceFlag : int;

UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGSetDynamicMeshCustomDataSettings : public UPCGDynamicMeshBaseSettings
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
};


class FPCGSetDynamicMeshCustomDataElement : public IPCGDynamicMeshBaseElement
{
protected:
	virtual bool ExecuteInternal(FPCGContext* InContext) const override;

	virtual bool CanExecuteOnlyOnMainThread(FPCGContext* Context) const override
	{
		return true;
	}

	virtual bool IsCacheable(const UPCGSettings* InSettings) const override
	{
		return false;
	}
};
