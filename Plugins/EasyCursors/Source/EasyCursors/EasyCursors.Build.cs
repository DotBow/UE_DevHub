// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class EasyCursors : ModuleRules
{
	public EasyCursors(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"PropertyEditor",
			"DeveloperSettings"
		});

		DynamicallyLoadedModuleNames.AddRange(
		new string[]
		{
		});
	}
}
