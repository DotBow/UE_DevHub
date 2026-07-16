// Copyright (C) Dreamer's Tail

#pragma once

#include "PCGSettings.h"
#include "PCGTranslateAlongAxis.generated.h"


UENUM()
enum class EPCGTranslateAlongAxis : uint8
{
	PositiveX UMETA(DisplayName="+X", ToolTip="Orients axis in the positive X direction"),
	PositiveY UMETA(DisplayName="+Y", ToolTip="Orients axis in the positive Y direction"),
	PositiveZ UMETA(DisplayName="+Z", ToolTip="Orients axis in the positive Z direction"),
	NegativeX UMETA(DisplayName="-X", ToolTip="Orients axis in the negative X direction"),
	NegativeY UMETA(DisplayName="-Y", ToolTip="Orients axis in the negative Y direction"),
	NegativeZ UMETA(DisplayName="-Z", ToolTip="Orients axis in the negative Z direction"),
};


UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGTranslateAlongAxisSettings : public UPCGSettings
{
	GENERATED_BODY()

public:
	//~Begin UPCGSettings interface
#if WITH_EDITOR
	virtual FName GetDefaultNodeName() const override
	{
		return FName(TEXT("TranslateAlongAxis"));
	}

	virtual FText GetDefaultNodeTitle() const override
	{
		return NSLOCTEXT("PCGTranslateAlongAxisSettings", "NodeTitle", "Translate Along Axis");
	}

	virtual EPCGSettingsType GetType() const override
	{
		return EPCGSettingsType::PointOps;
	}

	virtual bool DisplayExecuteOnGPUSetting() const override
	{
		return false;
	}
#endif

	virtual bool UseSeed() const override
	{
		return false;
	}

protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override
	{
		return Super::DefaultPointInputPinProperties();
	}

	virtual TArray<FPCGPinProperties> OutputPinProperties() const override
	{
		return Super::DefaultPointOutputPinProperties();
	}

	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings interface

public:
	EPCGPointNativeProperties GetPropertiesToAllocate() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	EPCGTranslateAlongAxis Axis;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	FName AttributeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_OverridableCPUAndGPU))
	float Offset = 100.f;
};


class FPCGTranslateAlongAxisElement : public IPCGElement
{
protected:
	virtual bool ExecuteInternal(FPCGContext* Context) const override;

	virtual EPCGElementExecutionLoopMode ExecutionLoopMode(const UPCGSettings* Settings) const override
	{
		return EPCGElementExecutionLoopMode::SinglePrimaryPin;
	}

	virtual bool SupportsBasePointDataInputs(FPCGContext* InContext) const override
	{
		return true;
	}
};
