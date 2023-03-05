// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class WalletConnect : ModuleRules
{
	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "WCEngine/QRCodeGenerator/")); }
	}

	public WalletConnect(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;
        bEnableUndefinedIdentifierWarnings = false;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
		new string[] {
				Path.Combine(ModuleDirectory, "WCEngine"),
				Path.Combine(ModuleDirectory, "Public"),
				ThirdPartyPath

			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Private"),
				ThirdPartyPath

            }
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"OpenSSL",
				"Core",
				"WebSockets",
                //"QRCodeGenerator",
                "Projects"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"OpenSSL",
                //"QRCodeGenerator"

				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
