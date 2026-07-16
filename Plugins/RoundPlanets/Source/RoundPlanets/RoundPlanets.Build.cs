// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class RoundPlanets : ModuleRules
{
	public RoundPlanets(ReadOnlyTargetRules Target) : base(Target)
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
			"Chaos",
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
