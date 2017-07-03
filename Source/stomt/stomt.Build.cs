// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class stomt : ModuleRules
{
	public stomt(ReadOnlyTargetRules ROTargetRules) : base(ROTargetRules)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP", "Json", "StomtPlugin", "UMG", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });

	   // PublicIncludePaths.AddRange(new string[] { "StomtPlugin/Public", "StomtPlugin/Classes" });

		//PrivateDependencyModuleNames.AddRange(new string[] { "VaRestPlugin" });
	}
}
