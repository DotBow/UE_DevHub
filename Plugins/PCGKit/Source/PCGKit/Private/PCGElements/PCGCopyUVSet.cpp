// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGCopyUVSet.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGCopyUVSet)

#define LOCTEXT_NAMESPACE "PCGCopyUVSetElement"

namespace PCGCopyUVSet
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGCopyUVSetSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Copy UV Set"));
}

FText UPCGCopyUVSetSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Copy UV Set");
}

FText UPCGCopyUVSetSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Copy the data in one UV Channel to another UV Channel on the same dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGCopyUVSetSettings::CreateElement() const
{
	return MakeShared<FPCGCopyUVSetElement>();
}

TArray<FPCGPinProperties> UPCGCopyUVSetSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGCopyUVSet::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGCopyUVSetElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGCopyUVSetElement::Execute);

	check(InContext);

	const UPCGCopyUVSetSettings* Settings = InContext->GetInputSettings<UPCGCopyUVSetSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGCopyUVSet::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const int32 FromUVSet = Settings->FromUVSet;
	const int32 ToUVSet = Settings->ToUVSet;

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

		UGeometryScriptLibrary_MeshUVFunctions::CopyUVSet(
			CurrentOutputMeshData->GetMutableDynamicMesh(),
			FromUVSet, ToUVSet, nullptr);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
