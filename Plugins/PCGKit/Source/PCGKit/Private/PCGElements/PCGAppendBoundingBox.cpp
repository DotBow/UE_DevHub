// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGAppendBoundingBox.h"
#include "PCGContext.h"
#include "Data/PCGBasePointData.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Metadata/Accessors/PCGAttributeAccessorHelpers.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGAppendBoundingBox)

#define LOCTEXT_NAMESPACE "PCGAppendBoundingBoxElement"

namespace PCGAppendBoundingBox
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
	static const FName InputBPinLabel = TEXT("Points");
}

#if WITH_EDITOR
FName UPCGAppendBoundingBoxSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Append Bounding Box"));
}

FText UPCGAppendBoundingBoxSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Append Bounding Box");
}

FText UPCGAppendBoundingBoxSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Appends a 3D box to the dynamic meshes dimensions and origin taken from the points data.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGAppendBoundingBoxSettings::CreateElement() const
{
	return MakeShared<FPCGAppendBoundingBoxElement>();
}

TArray<FPCGPinProperties> UPCGAppendBoundingBoxSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGAppendBoundingBox::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	Properties.Emplace_GetRef(PCGAppendBoundingBox::InputBPinLabel, EPCGDataType::Point).SetRequiredPin();
	return Properties;
}


bool FPCGAppendBoundingBoxElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGAppendBoundingBoxElement::Execute);

	check(InContext);

	const UPCGAppendBoundingBoxSettings* Settings = InContext->GetInputSettings<UPCGAppendBoundingBoxSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> MeshInputs = InContext->InputData.GetInputsByPin(PCGAppendBoundingBox::InputAPinLabel);
	const TArray<FPCGTaggedData> PointInput = InContext->InputData.GetInputsByPin(PCGAppendBoundingBox::InputBPinLabel);

	if (MeshInputs.IsEmpty())
	{
		return true;
	}

	const int32 StepsX = Settings->StepsX;
	const int32 StepsY = Settings->StepsY;
	const int32 StepsZ = Settings->StepsZ;

	TArray<FTransform> Transforms;
	TArray<FVector> BoundsMin;
	TArray<FVector> BoundsMax;

	for (const FPCGTaggedData& Points : PointInput)
	{
		const UPCGSpatialData* SpatialData = Cast<UPCGSpatialData>(Points.Data);

		if (!SpatialData)
		{
			PCGLog::InputOutput::LogInvalidInputDataError(InContext);
			continue;
		}

		const UPCGBasePointData* PointData = SpatialData->ToBasePointData(InContext);

		if (!PointData)
		{
			PCGLog::InputOutput::LogInvalidInputDataError(InContext);
			continue;
		}

		Transforms.Append(PointData->GetTransformsCopy());

		TArray<FVector> OutBoundsMin;
		TArray<FVector> OutBoundsMax;
		const TConstPCGValueRange<FVector> BoundsMinRange = PointData->GetConstBoundsMinValueRange();
		const TConstPCGValueRange<FVector> BoundsMaxRange = PointData->GetConstBoundsMaxValueRange();

		for (int32 i = 0; i < PointData->GetNumPoints(); ++i)
		{
			OutBoundsMin.Add(BoundsMinRange[i]);
			OutBoundsMax.Add(BoundsMaxRange[i]);
		}

		BoundsMin.Append(OutBoundsMin);
		BoundsMax.Append(OutBoundsMax);
	}

	UPCGDynamicMeshData* CurrentOutputMeshData = nullptr;
	FPCGTaggedData* CurrentTaggedOutputData = nullptr;

	for (int32 i = 0; i < MeshInputs.Num(); ++i)
	{
		const FPCGTaggedData& Input = MeshInputs[i];
		const UPCGDynamicMeshData* InputMesh = Cast<const UPCGDynamicMeshData>(Input.Data);

		if (!InputMesh)
		{
			PCGLog::InputOutput::LogInvalidInputDataError(InContext);
			continue;
		}

		CurrentOutputMeshData = CopyOrSteal(Input, InContext);
		CurrentTaggedOutputData = &InContext->OutputData.TaggedData.Emplace_GetRef(Input);
		CurrentTaggedOutputData->Data = CurrentOutputMeshData;

		check(CurrentOutputMeshData);
		check(CurrentTaggedOutputData);

		for (int32 DataId = 0; DataId < Transforms.Num(); ++DataId)
		{
			FGeometryScriptPrimitiveOptions PrimitiveOptions;
			UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBoundingBox(
				CurrentOutputMeshData->GetMutableDynamicMesh(), PrimitiveOptions,
				Transforms[DataId],
				FBox(BoundsMin[DataId], BoundsMax[DataId]),
				StepsX, StepsY, StepsZ, nullptr);
		}
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
