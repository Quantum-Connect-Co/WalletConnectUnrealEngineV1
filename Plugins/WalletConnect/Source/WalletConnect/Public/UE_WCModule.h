// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include "openssl/evp.h"
THIRD_PARTY_INCLUDES_END
#undef UI

#include <openssl/evp.h>
#include <openssl/aes.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <random>
#include <iostream>
#include "../WCEngine/Core/JSON/json_lib.hpp"


//url encode
#include <iomanip>
#include <sstream>

#include <WebSocketsModule.h>
#include <IWebSocket.h>
#include <IWebSocketsManager.h>

#include <Sockets.h>
#include "SocketSubsystem.h"



#include "UObject/NoExportTypes.h"
#include "UE_WCModule.generated.h"



using JSON = nlohmann::ordered_json;
using namespace std;

struct dbg_sharp
{
	std::string qr_code_uri;
	std::string wss_url;
	std::string http_url;
	std::string client_id;
	std::string handshake_topic;
	std::string key_hex;
	char bridge;
	std::string iv;
	std::string hmac;
	std::string data;
};

struct wc_protocol_data
{
	std::vector<uint8_t> _kewVector;
	std::vector<uint8_t> _iv;
	std::string _keyHex;
};

struct JsProtocolBridge
{
	std::string UUID;
};




UCLASS()
class WALLETCONNECT_API UUE_WCModule : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

public:

	void CreateNewSession() noexcept;
	void WebSocketInit();
	void Subscribe();
	void Publish();
	void Connect();
	void Close();

public:
	void InstantBridgeSocket();
private:

	char Bridge;

	UPROPERTY(VisibleAnywhere)
	FString BridgeHTTP;

	UPROPERTY(VisibleAnywhere)
	FString HandshakeTopic;

	UPROPERTY(VisibleAnywhere)
	FString ClientID;

public:
	UFUNCTION()
	void OnWebSocketGetMessage(const FString& MessageString);


public:

	std::string std_str_clientID;
	std::string std_str_handshakeTopic;

private:
	ISocketSubsystem* SocketSubsystem;
public:
	TSharedPtr<IWebSocket> WebSocket;
	
public:
	void SendMessage(std::string msg);
	FString GetQrCodeURI();

private:
	char GetRandomBridge();
	int  GenerateRandomInteger(int between);


public:
	std::string GetWssURL(char wc_bridge);
	std::string GetHttpsURL(char wc_bridge);
	std::string GetGuid();
	std::vector<uint8_t> CreateCryptoString(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, std::string str);
	std::string GenerateCryptoJson();
	std::string CreateStringFromVectorUCharHMAC(const std::vector<uint8_t>& key, std::vector<uint8_t>& data);

private:
	std::vector<uint8_t> GenerateKey();
	std::vector<uint8_t> GenerateIV();
	std::string ConvertKeyToHex(std::vector<uint8_t>& key);
	std::string ConvertStringToHex(const std::string& input);
	vector<uint8_t> CovertHexStringToVectorUChar(const std::string& data);
	std::string UrlEncode(std::string value);
	
private:


public:
	std::string GetJsonRpcRequest();


private:
	std::string url;
	std::string description;
	std::vector<std::string> icons;
	std::string name;

	
public:
	wc_protocol_data protocol_data;
	JsProtocolBridge JsProtocol;
	dbg_sharp sharp;


public:
	void DebugFromCharpModule();
	void ReadFileDebugJSON();
	void TestMigration();
};



