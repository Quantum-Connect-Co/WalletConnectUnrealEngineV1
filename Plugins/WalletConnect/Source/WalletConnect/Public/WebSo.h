// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WebSo.generated.h"


USTRUCT(BlueprintType)
struct FClientMeta
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Client meta")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Client meta")
	FString URL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Client meta")
	TArray<FString> Icons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Client meta")
	FString Name;
};

USTRUCT(BlueprintType)
struct FSession
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Session")
	TArray<FString> Accounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Session")
	bool Approved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Session")
	uint8 ChainId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Session")
	FString PeerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalletConnect|Session")
	FClientMeta ClientMeta;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWalletConnectSignature, FSession, WalletConnectSession);

UCLASS()
class AWebSo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWebSo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};