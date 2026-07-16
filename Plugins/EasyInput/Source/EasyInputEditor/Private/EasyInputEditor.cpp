// Copyright (C) Dreamer's Tail

#include "EasyInputEditor.h"

#include "AssetToolsModule.h"
#include "EasyInputEditorStyle.h"
#include "IAssetTools.h"
#include "EasyInputBindingsAsset.h"
#include "EasyInputBindingsCustomization.h"
#include "PropertyEditorModule.h"


#define LOCTEXT_NAMESPACE "EasyInputEditorModule"

void FEasyInputEditorModule::StartupModule()
{
	FEasyInputEditorStyle::Get();

	/* Register assets */
	IAssetTools& AssetTools =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	const EAssetTypeCategories::Type EasyInputCategoryBit =
		AssetTools.RegisterAdvancedAssetCategory(
			FName(TEXT("Easy Input")),
			LOCTEXT("EasyInputCategory", "Easy Input"));

	EasyInputBindingsActions = MakeShareable(new FEasyInputBindingsActions(EasyInputCategoryBit));
	AssetTools.RegisterAssetTypeActions(EasyInputBindingsActions.ToSharedRef());

	/* Register customization */
	FPropertyEditorModule& PropertyModule =
		FModuleManager::LoadModuleChecked
		<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FEasyInputActionKey::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FEasyInputActionKeyCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(
		UEasyInputBindings::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(
			&FEasyInputBindingsCustomization::MakeInstance));
}

void FEasyInputEditorModule::ShutdownModule()
{
	/* Unregister assets */
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools =
			FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		AssetTools.UnregisterAssetTypeActions(EasyInputBindingsActions.ToSharedRef());
	}

	/* Unregister customization */
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule =
			FModuleManager::LoadModuleChecked
			<FPropertyEditorModule>("PropertyEditor");

		PropertyModule.UnregisterCustomPropertyTypeLayout(
			FEasyInputActionKey::StaticStruct()->GetFName());

		PropertyModule.UnregisterCustomClassLayout(
			UEasyInputBindings::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEasyInputEditorModule, EasyInputEditor)
