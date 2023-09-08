// Copyright 2020 by Nicholas Helish. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UEVirtualCursor : ModuleRules
{
	public UEVirtualCursor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		
		// PublicIncludePaths.AddRange(
		// 	new string[] {
		// 	});
		
		// PrivateIncludePaths.AddRange(
		// 	new string[] {
		// 	});
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputCore",
				"UMG",
				"Slate",
				"SlateCore",
			});
		
		// DynamicallyLoadedModuleNames.AddRange(
		// 	new string[]
		// 	{
		// 	});
		
	}
}
