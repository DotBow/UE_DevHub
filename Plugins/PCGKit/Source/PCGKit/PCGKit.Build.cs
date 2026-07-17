// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class PCGKit : ModuleRules
{
	public PCGKit(ReadOnlyTargetRules Target) : base(Target)
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
			"PCG",
			"PCGGeometryScriptInterop",
			"GeometryScriptingCore",
			"PhysicsCore",
			"GeometryFramework",
			"GeometryCore"
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
