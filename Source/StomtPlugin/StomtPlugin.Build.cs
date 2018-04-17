// Copyright 2018 STOMT GmbH. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class StomtPlugin : ModuleRules
{
    public StomtPlugin(ReadOnlyTargetRules ROTargetRules) : base(ROTargetRules)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(new string[] { "StomtPlugin/Private" });
        PublicIncludePaths.AddRange(new string[] { "StomtPlugin/Public" });
 
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP", "Json", "StomtPlugin", "UMG", "Slate", "SlateCore", "ImageWrapper" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
    }
}
