// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGAutoRepairNormals.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGAutoRepairNormals)

#define LOCTEXT_NAMESPACE "PCGAutoRepairNormalsElement"

namespace PCGAutoRepairNormals
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGAutoRepairNormalsSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Auto Repair Normals"));
}

FText UPCGAutoRepairNormalsSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Auto Repair Normals");
}

FText UPCGAutoRepairNormalsSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Auto repair normals on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGAutoRepairNormalsSettings::CreateElement() const
{
	return MakeShared<FPCGAutoRepairNormalsElement>();
}

TArray<FPCGPinProperties> UPCGAutoRepairNormalsSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGAutoRepairNormals::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGAutoRepairNormalsElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGAutoRepairNormalsElement::Execute);

	check(InContext);

	const UPCGAutoRepairNormalsSettings* Settings = InContext->GetInputSettings<UPCGAutoRepairNormalsSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGAutoRepairNormals::InputAPinLabel);

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

		UGeometryScriptLibrary_MeshNormalsFunctions::AutoRepairNormals(
			CurrentOutputMeshData->GetMutableDynamicMesh(),
			nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
