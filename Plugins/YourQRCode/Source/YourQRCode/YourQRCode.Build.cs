// Copyright 2019 Chen Wei Hong. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class YourQRCode : ModuleRules
{

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/")); }
    }

    public YourQRCode(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;
        bEnableUndefinedIdentifierWarnings = false;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "YourQRCode/Public",
                Path.Combine(ThirdPartyPath, "QRCodeGenerator/")
                 //Path.Combine(ThirdPartyPath, "include")
				// ... add public include paths required here ...
			}
			);

        // Add Library Path 
        //PublicLibraryPaths.Add(Path.Combine(ThirdPartyPath, "QRCodeGenerator/Includes"));


        PrivateIncludePaths.AddRange(
			new string[] 
            {
                "YourQRCode/Private"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "QRCodeGenerator",
                "Projects"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "QRCodeGenerator"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{
        //    PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "qrencode_64.lib"));

        //}
        //else if (Target.Platform == UnrealTargetPlatform.Win32)
        //{
        //    PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "qrencode_32.lib"));
        //}
    }
}
