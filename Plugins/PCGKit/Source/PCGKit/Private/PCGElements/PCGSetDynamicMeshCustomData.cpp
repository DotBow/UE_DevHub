// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGSetDynamicMeshCustomData.h"
#include "PCGContext.h"
#include "Components/DynamicMeshComponent.h"
#include "Data/PCGDynamicMeshData.h"
#include "Helpers/PCGHelpers.h"
#include "Resources/PCGDynamicMeshManagedComponent.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGSetDynamicMeshCustomData)

#define LOCTEXT_NAMESPACE "PCGSetDynamicMeshCustomDataElement"

namespace PCGSetDynamicMeshCustomData
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGSetDynamicMeshCustomDataSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Set Custom Data"));
}

FText UPCGSetDynamicMeshCustomDataSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Set Custom Data");
}

FText UPCGSetDynamicMeshCustomDataSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Setup the Default Custom Primitive Data based on the assigned materials. Wipes any existing data.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGSetDynamicMeshCustomDataSettings::CreateElement() const
{
	return MakeShared<FPCGSetDynamicMeshCustomDataElement>();
}

TArray<FPCGPinProperties> UPCGSetDynamicMeshCustomDataSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGSetDynamicMeshCustomData::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGSetDynamicMeshCustomDataElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGSetDynamicMeshCustomDataElement::Execute);

	check(InContext);

	const UPCGSetDynamicMeshCustomDataSettings* Settings = InContext->GetInputSettings<UPCGSetDynamicMeshCustomDataSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> MeshInputs = InContext->InputData.GetInputsByPin(PCGSetDynamicMeshCustomData::InputAPinLabel);

	if (MeshInputs.IsEmpty() )
	{
		return true;
	}

	AActor* TargetActor = InContext->GetTypedExecutionTarget<AActor>();

	if (!TargetActor)
	{
		PCGLog::LogErrorOnGraph(LOCTEXT("InvalidTargetActor", "Invalid target actor."), InContext);
		return true;
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

		PCGHelpers::ExecuteOnGameThread(TEXT("SetupDefaultCustomPrimitiveDataFromMaterials"),
		[InContext, Settings, InputMesh, TargetActor]()
		{
			const UPCGDynamicMeshManagedComponent* ManagedComponent =
				PCGDynamicMeshManagedComponent::GetOrCreateDynamicMeshManagedComponent(
				InContext, Settings, InputMesh, TargetActor);
			UDynamicMeshComponent* Component =
				ManagedComponent ? ManagedComponent->GetComponent() : nullptr;

			if (Component)
			{
				Component->SetupDefaultCustomPrimitiveDataFromMaterials();
			}
		});
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
