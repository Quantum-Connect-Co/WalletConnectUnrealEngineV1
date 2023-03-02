// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <WalletConnect/WCEngine/Core/Utils/WCEngine.hpp>
#include "WClient.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WALLETCONNECT_API UWClient : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "WalletConnect|Create new session")
	FString CreateNewSession(FClientMeta ClientMeta, FString Adress);

	UFUNCTION(BlueprintPure, Category = "WalletConnect|Get Session Data")
	FSession GetSessionData();

	UFUNCTION(BlueprintCallable, Category = "WalletConnect")
	void SendEthTransaction(FString _From, FString To, FString Value, FString Data);

public:

	UPROPERTY(BlueprintAssignable, Category = "Wallet Connect Events")
	FWalletConnectSignature GetWalletSessionData;

private:

	UFUNCTION()
	void OnWebSocketConnected();

	UFUNCTION()
	void OnWebSocketGetMessage(const FString& MessageString);

	UFUNCTION()
	void OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	UFUNCTION()
	void OnWebSocketError(const FString& Error);

private:

	FClientMeta ClientMeta;

	FSession Session;

	TSharedPtr<IWebSocket> WebSocket;

public:
	
	UWClient();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void WebSocketClose();

	void WebSocketInit(FString Adress);

	void Deallocate();

private:
	

	session_controller* SessionHandler;
};
