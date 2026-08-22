// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class EasyPrefsEditor : ModuleRules
{
	public EasyPrefsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
		new string[] {
		});

		PrivateIncludePaths.AddRange(
		new string[] {
		});

		PublicDependencyModuleNames.AddRange(
		new string[]
		{
			"Core",
			"EasyPrefs",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"SlateCore",
			"Slate",
			"ToolMenus"
		});

		DynamicallyLoadedModuleNames.AddRange(
		new string[]
		{
		});
	}
}
