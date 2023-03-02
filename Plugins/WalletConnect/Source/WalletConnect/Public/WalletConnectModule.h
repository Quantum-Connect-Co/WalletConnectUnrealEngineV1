// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <WebSocketsModule.h>
#include <IWebSocket.h>
#include <IWebSocketsManager.h>

#include "WalletConnectModule.generated.h"


UENUM(BlueprintType)
enum class Status : uint8
{
	Disconnected,
	Connected
};




static Status StatusWalletConnection;

//static TSharedPtr<IWebSocket> WebSocket;


UCLASS()
class WALLETCONNECT_API UWebSocketAdapter final : public UObject
{
	GENERATED_BODY()

public:
	
	TSharedPtr<IWebSocket> WebSocket;
	
};



UCLASS()
class WALLETCONNECT_API UWalletConnectModule final : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "WalletConnect|Init Wallet Connect")
	static void CreateNewSession(UWebSocketAdapter* UWebSocketAdapter);

	UFUNCTION(BlueprintCallable, Category = "WalletConnect|QR Code")
	static FString GetQRCodeURI();

	UFUNCTION(BlueprintCallable, Category = "WalletConnect|Web Socket Init")
	static void WebSocketInit();

	UFUNCTION(BlueprintCallable, Category = "WalletConnect|Get Connect Status")
	static Status GetConnectionStatus();

	UFUNCTION(BlueprintCallable, Category = "WalletConnect|Call When End Play")
	static void CallWhenEndPlay();

	UFUNCTION()
	static void OnMessageEvent(const FString& MessageString);
};
