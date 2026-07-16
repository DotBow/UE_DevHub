// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGUVTransform.h"
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGUVTransform)

#define LOCTEXT_NAMESPACE "PCGUVTransformElement"

namespace PCGUVTransform
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGUVTransformSettings::GetDefaultNodeName() const
{
	return FName(TEXT("UV Transform Operation"));
}

FText UPCGUVTransformSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "UV Transform Operation");
}

FText UPCGUVTransformSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "UV Transform operation on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGUVTransformSettings::CreateElement() const
{
	return MakeShared<FPCGUVTransformElement>();
}

TArray<FPCGPinProperties> UPCGUVTransformSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGUVTransform::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGUVTransformElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGUVTransformElement::Execute);

	check(InContext);

	const UPCGUVTransformSettings* Settings = InContext->GetInputSettings<UPCGUVTransformSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGUVTransform::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const int32 UVSetIndex = Settings->UVSetIndex;
	const FVector2D Translate = Settings->Translate;
	const FVector2D ScaleOrigin = Settings->ScaleOrigin;
	const FVector2D Scale = Settings->Scale;
	const FVector2D RotationOrigin = Settings->RotationOrigin;
	const float RotationAngle = Settings->RotationAngle;

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

		if (!Translate.IsZero())
		{
			UGeometryScriptLibrary_MeshUVFunctions::TranslateMeshUVs(
				CurrentOutputMeshData->GetMutableDynamicMesh(),
				UVSetIndex, Translate, FGeometryScriptMeshSelection(), nullptr);
		}

		if (Scale != FVector2D(1.f, 1.f))
		{
			UGeometryScriptLibrary_MeshUVFunctions::ScaleMeshUVs(
				CurrentOutputMeshData->GetMutableDynamicMesh(),
				UVSetIndex, Scale, ScaleOrigin, FGeometryScriptMeshSelection(), nullptr);
		}

		if (RotationAngle != 0)
		{
			UGeometryScriptLibrary_MeshUVFunctions::RotateMeshUVs(
				CurrentOutputMeshData->GetMutableDynamicMesh(),
				UVSetIndex, RotationAngle, RotationOrigin, FGeometryScriptMeshSelection(), nullptr);
		}
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
