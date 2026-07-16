// Copyright (C) Dreamer's Tail

#include "ProtoKitPlacement.h"

#include "BaseMeshKit.h"
#include "IPlacementModeModule.h"
#include "ProtoKitSettings.h"
#include "ActorFactories/ActorFactoryStaticMesh.h"


#define LOCTEXT_NAMESPACE "FDataChartsEditorModule"

void FProtoKitPlacement::RegisterPlacement()
{
	IPlacementModeModule& PlacementMode = IPlacementModeModule::Get();

	PlacementMode.RegisterPlacementCategory(
		FPlacementCategoryInfo(
			LOCTEXT("ProtoKit", "ProtoKit"),
			FSlateIcon(FName("ProtoKitEditorStyle"), "ProtoKit"),
			"ProtoKit",
			TEXT("ProtoKit"),
			TNumericLimits<int32>::Lowest(),
			false
		));

	PlacementMode.RegisterPlaceableItem(
		"ProtoKit", MakeShared<FPlaceableItem>(
		*ABaseMeshKit::StaticClass(),
		FAssetData(ABaseMeshKit::StaticClass()),
		FName("ClassIcon.BaseMeshKit"),
		FName("ClassIcon.BaseMeshKit"),
		TOptional<FLinearColor>(),
		TOptional<int32>(),
		NSLOCTEXT("PlacementMode", "BaseMeshKit", "Base Mesh")
	));

	for (const FProtoKitPlaceableMeshDesc& PlaceableMeshDesc :
		UProtoKitSettings::Get()->GetPlaceableMeshes())
	{
		const TSharedPtr<FPlaceableItem> Item =
			MakeShared<FPlaceableItem>(*UActorFactoryStaticMesh::StaticClass(),
			FAssetData(LoadObject<UStaticMesh>(nullptr,
			PlaceableMeshDesc.StaticMesh.GetAssetPath().ToString())),
			PlaceableMeshDesc.ThumbnailBrush, PlaceableMeshDesc.IconBrush,
			TOptional<FLinearColor>(), TOptional<int32>(),
			FText::FromString(PlaceableMeshDesc.DisplayName));
		PlacementMode.RegisterPlaceableItem("ProtoKit", Item.ToSharedRef());
	}
}

void FProtoKitPlacement::UnregisterPlacement()
{
	if (IPlacementModeModule::IsAvailable())
	{
		IPlacementModeModule::Get().UnregisterPlacementCategory(TEXT("ProtoKit"));
	}
}

#undef LOCTEXT_NAMESPACE
