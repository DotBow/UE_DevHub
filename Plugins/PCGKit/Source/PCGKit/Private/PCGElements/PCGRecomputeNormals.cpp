// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGRecomputeNormals.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGRecomputeNormals)

#define LOCTEXT_NAMESPACE "PCGRecomputeNormalsElement"

namespace PCGRecomputeNormals
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGRecomputeNormalsSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Recompute Normals"));
}

FText UPCGRecomputeNormalsSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Recompute Normals");
}

FText UPCGRecomputeNormalsSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Recompute normals on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGRecomputeNormalsSettings::CreateElement() const
{
	return MakeShared<FPCGRecomputeNormalsElement>();
}

TArray<FPCGPinProperties> UPCGRecomputeNormalsSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGRecomputeNormals::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGRecomputeNormalsElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGRecomputeNormalsElement::Execute);

	check(InContext);

	const UPCGRecomputeNormalsSettings* Settings = InContext->GetInputSettings<UPCGRecomputeNormalsSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGRecomputeNormals::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const FGeometryScriptCalculateNormalsOptions CalculateOptions = Settings->CalculateOptions;

	UPCGDynamicMeshData* CurrentOutputMeshData = nullptr;
	FPCGTaggedData* CurrentTaggedOutputData = nullptr;

	for (int32 i = 0; i < Inputs.Num(); ++i)
	{
		const FPCGTaggedData& Input = Inputs[i];
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

		UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(
			CurrentOutputMeshData->GetMutableDynamicMesh(),
			CalculateOptions,
			false,
			nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
