// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGSetDynamicMeshCustomDataFloat.h"
#include "PCGContext.h"
#include "Components/DynamicMeshComponent.h"
#include "Data/PCGDynamicMeshData.h"
#include "Helpers/PCGHelpers.h"
#include "Resources/PCGDynamicMeshManagedComponent.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGSetDynamicMeshCustomDataFloat)

#define LOCTEXT_NAMESPACE "PCGSetDynamicMeshCustomDataFloatElement"

namespace PCGSetDynamicMeshCustomDataFloat
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGSetDynamicMeshCustomDataFloatSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Set Custom Data Float"));
}

FText UPCGSetDynamicMeshCustomDataFloatSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Set Custom Data Float");
}

FText UPCGSetDynamicMeshCustomDataFloatSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Set default custom primitive data, an array floats at once, from index DataIndex to index DataIndex + Values.Num(), and marks the render state dirty");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGSetDynamicMeshCustomDataFloatSettings::CreateElement() const
{
	return MakeShared<FPCGSetDynamicMeshCustomDataFloatElement>();
}

TArray<FPCGPinProperties> UPCGSetDynamicMeshCustomDataFloatSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGSetDynamicMeshCustomDataFloat::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGSetDynamicMeshCustomDataFloatElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGSetDynamicMeshCustomDataFloatElement::Execute);

	check(InContext);

	const UPCGSetDynamicMeshCustomDataFloatSettings* Settings = InContext->GetInputSettings<UPCGSetDynamicMeshCustomDataFloatSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> MeshInputs = InContext->InputData.GetInputsByPin(PCGSetDynamicMeshCustomDataFloat::InputAPinLabel);

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

	const int32 DataIndex = Settings->DataIndex;
	const TArray<float> Values = Settings->Values;

	if (Values.IsEmpty())
		return true;

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

		PCGHelpers::ExecuteOnGameThread(TEXT("SetDefaultCustomPrimitiveDataFloatArray"),
		[InContext, Settings, InputMesh, TargetActor, DataIndex, Values]()
		{
			const UPCGDynamicMeshManagedComponent* ManagedComponent =
				PCGDynamicMeshManagedComponent::GetOrCreateDynamicMeshManagedComponent(
				InContext, Settings, InputMesh, TargetActor);
			UDynamicMeshComponent* Component =
				ManagedComponent ? ManagedComponent->GetComponent() : nullptr;

			if (Component)
			{
				Component->SetDefaultCustomPrimitiveDataFloatArray(DataIndex, Values);
			}
		});
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
