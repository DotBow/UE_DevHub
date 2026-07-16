// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class EasyPrefs : ModuleRules
{
	public EasyPrefs(ReadOnlyTargetRules Target) : base(Target)
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
			"CoreUObject",
			"Engine",
			"DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
		});

		DynamicallyLoadedModuleNames.AddRange(
		new string[]
		{
		});
	}
}
