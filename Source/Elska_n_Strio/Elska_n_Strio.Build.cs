// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Elska_n_Strio : ModuleRules
{
	public Elska_n_Strio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "GameplayAbilities", "GameplayTags", "GameplayTasks", "AIModule", "UMG", "SlateCore" });
    }
}
