// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGComputeTangents.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGComputeTangents)

#define LOCTEXT_NAMESPACE "PCGComputeTangentsElement"

namespace PCGComputeTangents
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGComputeTangentsSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Compute Tangents"));
}

FText UPCGComputeTangentsSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Compute Tangents");
}

FText UPCGComputeTangentsSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Compute tangents on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGComputeTangentsSettings::CreateElement() const
{
	return MakeShared<FPCGComputeTangentsElement>();
}

TArray<FPCGPinProperties> UPCGComputeTangentsSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGComputeTangents::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGComputeTangentsElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGComputeTangentsElement::Execute);

	check(InContext);

	const UPCGComputeTangentsSettings* Settings = InContext->GetInputSettings<UPCGComputeTangentsSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGComputeTangents::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const FGeometryScriptTangentsOptions Options = Settings->Options;

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

		UGeometryScriptLibrary_MeshNormalsFunctions::ComputeTangents(
			CurrentOutputMeshData->GetMutableDynamicMesh(), Options, nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
