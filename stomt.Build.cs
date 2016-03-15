// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class stomt : ModuleRules
{
	public stomt(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP"});

        PrivateDependencyModuleNames.AddRange(new string[] { "VaRestPlugin" });
	}
}
