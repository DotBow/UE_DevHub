// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGUVTransformRandom.h"
#include "PCGContext.h"
#include "UDynamicMesh.h"
#include "Components/BaseDynamicMeshSceneProxy.h"
#include "Data/PCGDynamicMeshData.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Selections/MeshConnectedComponents.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGUVTransformRandom)

#define LOCTEXT_NAMESPACE "PCGUVTransformRandomElement"

namespace PCGUVTransformRandom
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGUVTransformRandomSettings::GetDefaultNodeName() const
{
	return FName(TEXT("UV Transform Random"));
}

FText UPCGUVTransformRandomSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "UV Transform Random");
}

FText UPCGUVTransformRandomSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "UV Transform operation on dynamic meshes.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGUVTransformRandomSettings::CreateElement() const
{
	return MakeShared<FPCGUVTransformRandomElement>();
}

TArray<FPCGPinProperties> UPCGUVTransformRandomSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGUVTransformRandom::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGUVTransformRandomElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGUVTransformRandomElement::Execute);

	check(InContext);

	const UPCGUVTransformRandomSettings* Settings = InContext->GetInputSettings<UPCGUVTransformRandomSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = InContext->InputData.GetInputsByPin(PCGUVTransformRandom::InputAPinLabel);

	if (Inputs.IsEmpty())
	{
		return true;
	}

	const int32 UVSetIndex = Settings->UVSetIndex;
	const float RotationAngleDegMin = Settings->RotationAngleDegMin;
	const float RotationAngleDegMax = Settings->RotationAngleDegMax;
	const FVector2D ScaleMin = Settings->ScaleMin;
	const FVector2D ScaleMax = Settings->ScaleMax;
	const FVector2D TranslateMin = Settings->TranslateMin;
	const FVector2D TranslateMax = Settings->TranslateMax;

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

		SetMeshUVElementPosition(CurrentOutputMeshData->GetMutableDynamicMesh(),
			UVSetIndex, RotationAngleDegMin, RotationAngleDegMax,
			FVector2f(ScaleMin), FVector2f(ScaleMax),
			FVector2f(TranslateMin), FVector2f(TranslateMax));
	}

	return true;
}

UDynamicMesh* FPCGUVTransformRandomElement::SetMeshUVElementPosition(
	UDynamicMesh* TargetMesh,
	const int32 UVSetIndex,
	const float RotationAngleDegMin,
	const float RotationAngleDegMax,
	const FVector2f& ScaleMin,
	const FVector2f& ScaleMax,
	const FVector2f& TranslateMin,
	const FVector2f& TranslateMax)
{
	if (TargetMesh)
	{
		TargetMesh->EditMesh([&](FDynamicMesh3& Mesh)
		{
			FDynamicMeshUVOverlay* UVOverlay = Mesh.Attributes()->GetUVLayer(UVSetIndex);

			if (!UVOverlay)
				return;

			UE::Geometry::FMeshConnectedComponents UVComponents(&Mesh);

			UVComponents.FindConnectedTriangles(
			[&](const int32 Tri0, const int32 Tri1)
			{
				return UVOverlay->AreTrianglesConnected(Tri0, Tri1);
			});

			for (const UE::Geometry::FMeshConnectedComponents::FComponent& Component : UVComponents.Components)
			{
				// Component.Indices contains the triangle IDs in this UV shell
				TSet<int32> UVElements;

				for (const int32 TreeIdx : Component.Indices)
				{
					UE::Geometry::FIndex3i UVTri = UVOverlay->GetTriangle(TreeIdx);

					UVElements.Add(UVTri.A);
					UVElements.Add(UVTri.B);
					UVElements.Add(UVTri.C);
				}

				uint32 Seed = 31337;

				for (const int32 ElemID : UVElements)
					Seed = HashCombine(Seed, GetTypeHash(ElemID));

				FRandomStream RandStream(Seed);

				const float Angle = RandStream.FRandRange(RotationAngleDegMin, RotationAngleDegMax);

				const float OffsetX = RandStream.FRandRange(TranslateMin.X, TranslateMax.X);
				const float OffsetY = RandStream.FRandRange(TranslateMin.Y, TranslateMax.Y);

				const float ScaleX = RandStream.FRandRange(ScaleMin.X, ScaleMax.X);
				const float ScaleY = RandStream.FRandRange(ScaleMin.Y, ScaleMax.Y);

				// Compute center
				FVector2f Center(0, 0);

				for (const int32 ElemID : UVElements)
					Center += UVOverlay->GetElement(ElemID);

				Center /= static_cast<float>(UVElements.Num());

				// Apply random transform
				for (const int32 ElemID : UVElements)
				{
					FVector2f UV = UVOverlay->GetElement(ElemID);

					const FVector2f Translate(OffsetX, OffsetY);
					const FVector2f Scale(ScaleX, ScaleY);

					UE::Geometry::FMatrix2f RotationMatrix = 
						UE::Geometry::FMatrix2f::RotationDeg(Angle);
					FVector2f UVScaleRotate = (RotationMatrix * UV) * Scale;
					UV = UVScaleRotate + Center + Translate;
 
					UVOverlay->SetElement(ElemID, UV);
				}
			}
		},
		EDynamicMeshChangeType::GeneralEdit,
		EDynamicMeshAttributeChangeFlags::UVs);
	}

	return TargetMesh;
}

#undef LOCTEXT_NAMESPACE
