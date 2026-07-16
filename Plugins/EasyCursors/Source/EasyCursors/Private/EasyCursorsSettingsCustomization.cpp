// Copyright (C) Dreamer's Tail

#include "EasyCursorsSettingsCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EasyCursorsSettings.h"
#include "PropertyCustomizationHelpers.h"
#include "ThumbnailRendering/ThumbnailManager.h"


TSharedRef<IDetailCustomization> FEasyCursorsSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FEasyCursorsSettingsCustomization);
}

void FEasyCursorsSettingsCustomization::CustomizeDetails(
	IDetailLayoutBuilder& DetailLayout)
{
	const TSharedRef<IPropertyHandle> HardwareCursorsProperty =
		DetailLayout.GetProperty(UEasyCursorsSettings::GetHardwareCursorsPropertyName());

	if (!HardwareCursorsProperty->IsValidHandle())
		return;

	const TSharedPtr<IPropertyHandleMap> HardwareCursorsPropertyMap =
		HardwareCursorsProperty->AsMap();

	if (!HardwareCursorsPropertyMap.IsValid())
		return;

	uint32 NumElements;
	HardwareCursorsPropertyMap->GetNumElements(NumElements);

	IDetailCategoryBuilder& Category =
		DetailLayout.EditCategory("EasyCursorsSettings");

	for (uint32 ElementIdx = 0; ElementIdx < NumElements; ElementIdx++)
	{
		const TSharedRef<IPropertyHandle> ElementHandle =
			HardwareCursorsPropertyMap->GetElement(ElementIdx);

		if (!ElementHandle->IsValidHandle())
			continue;

		const TSharedPtr<IPropertyHandle> KeyHandle =
			ElementHandle->GetKeyHandle();

		if (!KeyHandle.IsValid())
			return;

		uint8 MouseCursorValue;
		KeyHandle->GetValue(MouseCursorValue);
		const EMouseCursor::Type MouseCursorType =
			static_cast<EMouseCursor::Type>(MouseCursorValue);
		FString MouseCursorTypeName =
			UEasyCursorsSettings::GetMouseCursorTypeName(MouseCursorType);

		Category.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(MouseCursorTypeName))
		]
		.ValueContent()
		[
			SNew(SObjectPropertyEntryBox)
			.AllowClear(true)
			.AllowCreate(false)
			.AllowedClass(UMaterialInterface::StaticClass())
			.DisplayBrowse(true)
			.DisplayThumbnail(true)
			.DisplayUseSelected(true)
			.DisplayCompactSize(false)
			.ThumbnailPool(UThumbnailManager::Get().GetSharedThumbnailPool())
			.ObjectPath(this, &FEasyCursorsSettingsCustomization::ObjectPath, ElementHandle)
			.OnObjectChanged(this, &FEasyCursorsSettingsCustomization::OnObjectChanged, ElementHandle)
		];
	}

	DetailLayout.HideProperty(HardwareCursorsProperty);
}

FString FEasyCursorsSettingsCustomization::ObjectPath(
	const TSharedRef<IPropertyHandle> ElementHandle) const
{
	if (ElementHandle->IsValidHandle())
	{
		FString OutValue;
		ElementHandle->GetValueAsDisplayString(OutValue);
		return OutValue;
	}

	return FString("");
}

void FEasyCursorsSettingsCustomization::OnObjectChanged(
	const FAssetData& AssetData,
	const TSharedRef<IPropertyHandle> ElementHandle)
{
	if (AssetData.IsValid())
	{
		ElementHandle->SetValueFromFormattedString(
			AssetData.GetSoftObjectPath().ToString());
	}
}
