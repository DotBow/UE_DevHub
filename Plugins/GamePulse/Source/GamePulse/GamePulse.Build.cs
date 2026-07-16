// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class GamePulse : ModuleRules
{
	public GamePulse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			});

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			});

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"RHI",
				"UMG",
				"DeveloperSettings",
				// ... add other public dependencies that you statically link with here ...
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"NVAPI",
				// ... add private dependencies that you statically link with here ...	
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			});

		if (Target.IsInPlatformGroup(UnrealPlatformGroup.Windows))
		{
			// Uses DXGI to query GPU hardware
			// This is what will allow us to get GPU usage statistics at runtime
			PublicSystemLibraries.Add("DXGI.lib");
		}

		AddEngineThirdPartyPrivateStaticDependencies(Target, "NVAPI");
	}
}
