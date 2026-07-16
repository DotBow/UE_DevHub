// Copyright (C) Dreamer's Tail

#include "PCGElements/PCGTranslateAlongAxis.h"

#include "PCGContext.h"
#include "Compute/PCGKernelHelpers.h"
#include "Data/PCGSpatialData.h"
#include "Data/PCGBasePointData.h"
#include "Helpers/PCGAsync.h"
#include "Helpers/PCGHelpers.h"
#include "Metadata/Accessors/PCGAttributeAccessorHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGTranslateAlongAxis)

#define LOCTEXT_NAMESPACE "PCGTranslateAlongAxisElement"

FPCGElementPtr UPCGTranslateAlongAxisSettings::CreateElement() const
{
	return MakeShared<FPCGTranslateAlongAxisElement>();
}

EPCGPointNativeProperties UPCGTranslateAlongAxisSettings::GetPropertiesToAllocate() const
{
	return EPCGPointNativeProperties::Transform;
}

bool FPCGTranslateAlongAxisElement::ExecuteInternal(FPCGContext* Context) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGTranslateAlongAxisElement::Execute);

	const UPCGTranslateAlongAxisSettings* Settings = Context->GetInputSettings<UPCGTranslateAlongAxisSettings>();
	check(Settings);

	const TArray<FPCGTaggedData> Inputs = Context->InputData.GetInputsByPin(PCGPinConstants::DefaultInputLabel);
	TArray<FPCGTaggedData>& Outputs = Context->OutputData.TaggedData;

	const FName AttributeName = Settings->AttributeName;
	const EPCGTranslateAlongAxis Axis = Settings->Axis;

	// Use implicit capture, since we capture a lot
	//ProcessPoints(Context, Inputs, Outputs, [&](const FPCGPoint& InPoint, FPCGPoint& OutPoint)
	for (const FPCGTaggedData& Input : Inputs)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(FPCGTranslateAlongAxisElement::Execute::InputLoop);
		FPCGTaggedData& Output = Outputs.Add_GetRef(Input);

		const UPCGSpatialData* SpatialData = Cast<UPCGSpatialData>(Input.Data);

		if (!SpatialData)
		{
			PCGE_LOG(Error, GraphAndLog, LOCTEXT("InputMissingSpatialData", "Unable to get Spatial data from input"));
			continue;
		}

		const UPCGBasePointData* PointData = SpatialData->ToBasePointData(Context);

		if (!PointData)
		{
			PCGE_LOG(Error, GraphAndLog, LOCTEXT("InputMissingPointData", "Unable to get Point data from input"));
			continue;
		}

		UPCGBasePointData* OutputData = FPCGContext::NewPointData_AnyThread(Context);

		OutputData->InitializeFromData(PointData);
		OutputData->SetNumPoints(PointData->GetNumPoints(), /*bInitializeValues=*/false);

		const EPCGPointNativeProperties PropertiesToAllocate = Settings->GetPropertiesToAllocate();

		if (OutputData->HasSpatialDataParent())
		{
			OutputData->AllocateProperties(PropertiesToAllocate);
		}
		else
		{
			OutputData->AllocateProperties(PointData->GetAllocatedProperties() | PropertiesToAllocate);
		}

		Output.Data = OutputData;

		TArray<float> TargetOffsets;
		TargetOffsets.Init(Settings->Offset, PointData->GetNumPoints());

		if (AttributeName != NAME_None)
		{
			FPCGAttributePropertyInputSelector OffsetAttributeSelector;
			OffsetAttributeSelector.SetAttributeName(AttributeName);
			const TUniquePtr<const IPCGAttributeAccessor> OffsetAttributeAccessor =
				PCGAttributeAccessorHelpers::CreateConstAccessor(PointData, OffsetAttributeSelector);
			const TUniquePtr<const IPCGAttributeAccessorKeys> OffsetAttributeKeys =
				PCGAttributeAccessorHelpers::CreateConstKeys(PointData, OffsetAttributeSelector);

			if (!OffsetAttributeAccessor || !OffsetAttributeKeys)
			{
				PCGLog::Metadata::LogFailToCreateAccessorError(OffsetAttributeSelector, Context);
				continue;
			}

			PCGAttributeAccessorHelpers::ExtractAllValues(
				OffsetAttributeAccessor.Get(), OffsetAttributeKeys.Get(), TargetOffsets);
		}

		auto ProcessRangeFunc = [&](int32 StartReadIndex, int32 StartWriteIndex, int32 Count)
		{
			int32 NumWritten = 0;

			// Copy all properties except the ones we are going to modify (if we are not inheriting)
			if (!OutputData->HasSpatialDataParent())
			{
				PointData->CopyPropertiesTo(OutputData, StartReadIndex, StartWriteIndex, Count, EPCGPointNativeProperties::All & ~PropertiesToAllocate);
			}

			const TConstPCGValueRange<FTransform> ReadTransformRange = PointData->GetConstTransformValueRange();
			const TPCGValueRange<FTransform> WriteTransformRange = OutputData->GetTransformValueRange(/*bAllocate=*/false);

			for (int32 ReadIndex = StartReadIndex; ReadIndex < StartReadIndex + Count; ++ReadIndex)
			{
				const int32 WriteIndex = StartWriteIndex + NumWritten;
				const float TargetOffset = TargetOffsets[ReadIndex];

				FTransform SourceTransform = ReadTransformRange[ReadIndex];
				FVector Direction = FVector::ZeroVector;

				switch (Axis)
				{
					case EPCGTranslateAlongAxis::PositiveX:
						Direction =  SourceTransform.GetRotation().GetAxisX();
						break;
					case EPCGTranslateAlongAxis::PositiveY:
						Direction =  SourceTransform.GetRotation().GetAxisY();
						break;
					case EPCGTranslateAlongAxis::PositiveZ:
						Direction =  SourceTransform.GetRotation().GetAxisZ();
						break;
					case EPCGTranslateAlongAxis::NegativeX:
						Direction = -SourceTransform.GetRotation().GetAxisX();
						break;
					case EPCGTranslateAlongAxis::NegativeY:
						Direction = -SourceTransform.GetRotation().GetAxisY();
						break;
					case EPCGTranslateAlongAxis::NegativeZ:
						Direction = -SourceTransform.GetRotation().GetAxisZ();
						break;
				}

				SourceTransform.SetLocation(SourceTransform.GetLocation() +
					Direction * TargetOffset);
				WriteTransformRange[WriteIndex] = SourceTransform;

				++NumWritten;
			}

			check(NumWritten == Count);
			return NumWritten;
		};

		FPCGAsync::AsyncProcessingOneToOneRangeEx(
			&Context->AsyncState,
			PointData->GetNumPoints(),
			/*InitializeFunc=*/[](){},
			ProcessRangeFunc,
			/*bTimeSliceEnabled=*/false);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
