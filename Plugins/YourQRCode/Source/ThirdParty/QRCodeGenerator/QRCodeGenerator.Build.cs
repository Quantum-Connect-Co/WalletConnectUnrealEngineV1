// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class QRCodeGenerator : ModuleRules
{
	public QRCodeGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
        Type = ModuleType.External;

        bEnableExceptions = true;
        bEnableUndefinedIdentifierWarnings = false;

        string LibraryPath = ModuleDirectory + "Classes/";

        PublicIncludePaths.Add(LibraryPath);
    }
}
