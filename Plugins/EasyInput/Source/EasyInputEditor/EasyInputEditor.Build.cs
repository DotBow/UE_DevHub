// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class EasyInputEditor : ModuleRules
{
	public EasyInputEditor(ReadOnlyTargetRules Target) : base(Target)
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
			"EasyInput",
			"CommonUI"
		});

		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"SlateCore",
			"Slate",
			"Projects",
			"PropertyEditor",
			"InputCore",
			"GameplayTags"
		});

		DynamicallyLoadedModuleNames.AddRange(
		new string[]
		{
		});
	}
}
