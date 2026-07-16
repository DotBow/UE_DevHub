// Copyright (C) Dreamer's Tail

using UnrealBuildTool;

public class ProtoKitEditor : ModuleRules
{
	public ProtoKitEditor(ReadOnlyTargetRules Target) : base(Target)
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
			"ProtoKit",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
			"UnrealEd",
			"PlacementMode",
			"Projects"
		});

		DynamicallyLoadedModuleNames.AddRange(
		new string[]
		{
		});
	}
}
