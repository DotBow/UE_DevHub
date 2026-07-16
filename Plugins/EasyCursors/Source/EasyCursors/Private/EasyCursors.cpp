// Copyright (C) Dreamer's Tail

#include "EasyCursors.h"

#include "EasyCursorsSettings.h"
#include "EasyCursorsSettingsCustomization.h"


#define LOCTEXT_NAMESPACE "EasyCursorsModule"

void FEasyCursorsModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule =
		FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		UEasyCursorsSettings::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(
			&FEasyCursorsSettingsCustomization::MakeInstance));
}

void FEasyCursorsModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule =
		FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(
		UEasyCursorsSettings::StaticClass()->GetFName());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEasyCursorsModule, EasyCursors)
