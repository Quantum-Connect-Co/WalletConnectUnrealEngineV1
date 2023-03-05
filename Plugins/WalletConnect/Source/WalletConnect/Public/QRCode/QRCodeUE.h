// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Engine/Texture2D.h"
#include "Core/Public/Math/Color.h"
#include "QRCodeUE.generated.h"

/**
 * 
 */
UCLASS()
class WALLETCONNECT_API UQRCodeUE : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "QRCode")
		static UTexture2D* GenerateQRCodeImageSimple(FString code);

	UFUNCTION(BlueprintCallable, Category = "QRCode")
		static UTexture2D* GenerateQRCodeImage(FString code, int QRCodeVersion, int PixelNum, FColor Backcolor, FColor QRPixelColor);


};
