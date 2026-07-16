// Copyright (C) Dreamer's Tail

#pragma once
#include "IDetailCustomization.h"


class FEasyCursorsSettingsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(
		IDetailLayoutBuilder& DetailLayout) override;

private:
	FString ObjectPath(
		const TSharedRef<IPropertyHandle> ElementHandle) const;
	void OnObjectChanged(
		const FAssetData& AssetData,
		const TSharedRef<IPropertyHandle> ElementHandle);
};
