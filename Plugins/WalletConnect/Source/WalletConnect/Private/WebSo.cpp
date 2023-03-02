// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSo.h"
#include <random>
#include "../Public/UE_WCModule.h"
#include <WalletConnect/Public/WalletConnectModule.h>

DEFINE_LOG_CATEGORY_STATIC(LogWebSo, All, All);

AWebSo::AWebSo()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWebSo::BeginPlay()
{
	AActor::BeginPlay();

}

void AWebSo::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AActor::EndPlay(EndPlayReason);	
}

void AWebSo::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}
