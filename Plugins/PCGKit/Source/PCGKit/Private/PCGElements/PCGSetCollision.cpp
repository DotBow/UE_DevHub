// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGSetCollision.h"
#include "PCGContext.h"
#include "Components/DynamicMeshComponent.h"
#include "Data/PCGDynamicMeshData.h"
#include "Helpers/PCGHelpers.h"
#include "Resources/PCGDynamicMeshManagedComponent.h"
#include "Utils/PCGLogErrors.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGSetCollision)

#define LOCTEXT_NAMESPACE "PCGSetCollisionElement"

namespace PCGSetCollision
{
	static const FName InputAPinLabel = TEXT("DynamicMesh");
}

#if WITH_EDITOR
FName UPCGSetCollisionSettings::GetDefaultNodeName() const
{
	return FName(TEXT("Set Collision"));
}

FText UPCGSetCollisionSettings::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Set Collision");
}

FText UPCGSetCollisionSettings::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Generate Simple Collision shapes for a dynamic meshes based on the input collision.");
}
#endif // WITH_EDITOR

FPCGElementPtr UPCGSetCollisionSettings::CreateElement() const
{
	return MakeShared<FPCGSetCollisionElement>();
}

TArray<FPCGPinProperties> UPCGSetCollisionSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> Properties;
	Properties.Emplace_GetRef(PCGSetCollision::InputAPinLabel, EPCGDataType::DynamicMesh).SetRequiredPin();
	return Properties;
}


bool FPCGSetCollisionElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGSetCollisionElement::Execute);

	check(InContext);

	const UPCGSetCollisionSettings* Settings = InContext->GetInputSettings<UPCGSetCollisionSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> MeshInputs = InContext->InputData.GetInputsByPin(PCGSetCollision::InputAPinLabel);

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

	const ECollisionTraceFlag CollisionType = Settings->CollisionType;
	const ECollisionEnabled::Type CollisionEnabled = Settings->CollisionEnabled;
	const FCollisionProfileName CollisionProfile = Settings->CollisionProfile;
	const bool bEnableComplexCollision = Settings->bEnableComplexCollision;
	const bool bVisibility = Settings->bVisibility;

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

		PCGHelpers::ExecuteOnGameThread(TEXT("UpdateDynamicMeshComponentCollisionSettings"),
		[InContext, Settings, InputMesh, TargetActor, &CollisionProfile,
		CollisionType, CollisionEnabled, bEnableComplexCollision, bVisibility]()
		{
			const UPCGDynamicMeshManagedComponent* ManagedComponent =
				PCGDynamicMeshManagedComponent::GetOrCreateDynamicMeshManagedComponent(
				InContext, Settings, InputMesh, TargetActor);
			UDynamicMeshComponent* Component =
				ManagedComponent ? ManagedComponent->GetComponent() : nullptr;

			if (Component)
			{
				Component->CollisionType = CollisionType;
				Component->SetCollisionEnabled(CollisionEnabled);
				Component->SetCollisionProfileName(CollisionProfile.Name, false);
				Component->SetComplexAsSimpleCollisionEnabled(bEnableComplexCollision, true);
				Component->SetVisibility(bVisibility);
			}
		});
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
