// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGSetNumUVSets.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGSetNumUVSets)

#define LOCTEXT_NAMESPACE "PCGSetNumUVSetsElement"

namespace PCGSetNumUVSets
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGSetNumUVSetsSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Set Num UV Sets"));
}

FText UPCGSetNumUVSetsSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Set Num UV Sets");
}

FText UPCGSetNumUVSetsSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Set the number of UV Channels on the dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGSetNumUVSetsSettings::CreateElement() const
{
	return MakeShared<FPCGSetNumUVSetsElement>();
}

TArray<FPCGPinProperties> UPCGSetNumUVSetsSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGSetNumUVSets::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGSetNumUVSetsElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGSetNumUVSetsElement::Execute);

	check(InContext);

	const UPCGSetNumUVSetsSettings* Settings = InContext->GetInputSettings<UPCGSetNumUVSetsSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGSetNumUVSets::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const int32 NumUVSets = Settings->NumUVSets;

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

		UGeometryScriptLibrary_MeshUVFunctions::SetNumUVSets(
			CurrentOutputMeshData->GetMutableDynamicMesh(),
			NumUVSets, nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
