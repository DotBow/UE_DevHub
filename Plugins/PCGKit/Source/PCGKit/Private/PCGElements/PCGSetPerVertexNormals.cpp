// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGSetPerVertexNormals.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGSetPerVertexNormals)

#define LOCTEXT_NAMESPACE "PCGSetPerVertexNormalsElement"

namespace PCGSetPerVertexNormals
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGSetPerVertexNormalsSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Set Per Vertex Normals"));
}

FText UPCGSetPerVertexNormalsSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Set Per Vertex Normals");
}

FText UPCGSetPerVertexNormalsSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Set per vertex normals on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGSetPerVertexNormalsSettings::CreateElement() const
{
	return MakeShared<FPCGSetPerVertexNormalsElement>();
}

TArray<FPCGPinProperties> UPCGSetPerVertexNormalsSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGSetPerVertexNormals::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGSetPerVertexNormalsElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGSetPerVertexNormalsElement::Execute);

	check(InContext);

	const UPCGSetPerVertexNormalsSettings* Settings = InContext->GetInputSettings<UPCGSetPerVertexNormalsSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGSetPerVertexNormals::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

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

		UGeometryScriptLibrary_MeshNormalsFunctions::SetPerVertexNormals(
			CurrentOutputMeshData->GetMutableDynamicMesh(),
			nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
