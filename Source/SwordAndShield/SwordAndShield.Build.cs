// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SwordAndShield : ModuleRules
{
	public SwordAndShield(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",  "OnlineSubsystem", "OnlineSubsystemEOS", "OnlineSubsystemUtils"  });

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			var manifestFile = Path.Combine(ModuleDirectory, "AndroidSanitizePermission_UPL.xml");
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", manifestFile);
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
