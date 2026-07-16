// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGComputeSplitNormals.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGComputeSplitNormals)

#define LOCTEXT_NAMESPACE "PCGComputeSplitNormalsElement"

namespace PCGComputeSplitNormals
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGComputeSplitNormalsSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Compute Split Normals"));
}

FText UPCGComputeSplitNormalsSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Compute Split Normals");
}

FText UPCGComputeSplitNormalsSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Compute split normals on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGComputeSplitNormalsSettings::CreateElement() const
{
	return MakeShared<FPCGComputeSplitNormalsElement>();
}

TArray<FPCGPinProperties> UPCGComputeSplitNormalsSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGComputeSplitNormals::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGComputeSplitNormalsElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGComputeSplitNormalsElement::Execute);

	check(InContext);

	const UPCGComputeSplitNormalsSettings* Settings = InContext->GetInputSettings<UPCGComputeSplitNormalsSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGComputeSplitNormals::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const FGeometryScriptSplitNormalsOptions SplitOptions = Settings->SplitOptions;
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

		UGeometryScriptLibrary_MeshNormalsFunctions::ComputeSplitNormals(
			CurrentOutputMeshData->GetMutableDynamicMesh(),
			SplitOptions, CalculateOptions, nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
