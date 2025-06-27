// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ipvmulti : ModuleRules
{
	public ipvmulti(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"GameplayTasks"
		});
	}
}
