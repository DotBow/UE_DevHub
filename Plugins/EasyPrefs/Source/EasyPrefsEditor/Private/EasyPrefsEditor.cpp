// Copyright (C) Dreamer's Tail

#include "EasyPrefsEditor.h"

#include "EasyPrefsCVarsWidgets.h"


#define LOCTEXT_NAMESPACE "EasyPrefsEditorModule"

class FStructOnScopeStructureDataProvider;

void FEasyPrefsEditorModule::StartupModule()
{
	RegisterMenuExtensions();
}

void FEasyPrefsEditorModule::ShutdownModule()
{
	UToolMenus::UnregisterOwner(this);
}

void FEasyPrefsEditorModule::RegisterMenuExtensions()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* CustomToolBar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
		{
			FToolMenuSection& Section = CustomToolBar->AddSection("EasyPrefs");
			Section.AddSeparator("EasyPrefsSeperator");
			{
				FToolMenuEntry SettingsEntry = FToolMenuEntry::InitComboButton(
					"EasyPrefsMenus",
					FUIAction(),
					FOnGetContent::CreateStatic(&FEasyPrefsEditorModule::GenerateEasyPrefsOptionsMenu),
					LOCTEXT("EasyPrefsButton", ""),
					LOCTEXT("EasyPrefsButton_Tooltip", ""),
					FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.ExportAll"),
					false);

				SettingsEntry.StyleNameOverride = "CalloutToolbar";
				Section.AddEntry(SettingsEntry);
			}
		}
	}
}

TSharedRef<SWidget> FEasyPrefsEditorModule::GenerateEasyPrefsOptionsMenu()
{
	return SNew(SEasyPrefsCVars);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEasyPrefsEditorModule, EasyPrefsEditor)
