// Copyright 2019 Chen Wei Hong. All Rights Reserved.
#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Engine/Texture2D.h"
#include "Core/Public/Math/Color.h"
//#include "Color.h"

#include "YourTools.generated.h"

/**
 * 
 */
UCLASS()
class YOURQRCODE_API UYourTools : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "QRCode")
	static UTexture2D * GenerateQRCodeImageSimple(FString code);

	UFUNCTION(BlueprintCallable, Category = "QRCode")
	static UTexture2D * GenerateQRCodeImage(FString code, int QRCodeVersion, int PixelNum, FColor Backcolor, FColor QRPixelColor);

	
};
