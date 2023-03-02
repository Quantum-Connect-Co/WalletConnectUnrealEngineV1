// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_WCModule.h"

#include <iostream>
#include <openssl/hmac.h>
#include <openssl/rand.h>



#define PRINTSTD(msg, stdstr) TEXT("%s%s"), msg,  *FString(std::string(stdstr).c_str())

DEFINE_LOG_CATEGORY_STATIC(LogWalletConnect, All, All);

void UUE_WCModule::CreateNewSession() noexcept
{

	//UUE_WCModule::ReadFileDebugJSON();

	description = "An example that showcases how to use the WalletConnectSharp library";
	url = "https://app.warriders.com/";
	icons.push_back("https://app.warriders.com/favicon.ico");
	name = "WalletConnectUnreal";

	std_str_clientID = GetGuid();
	std_str_handshakeTopic = GetGuid();

	//FIX LATER
	Bridge = GetRandomBridge();
	

	protocol_data._iv = GenerateIV();
	protocol_data._kewVector = GenerateKey();
	//protocol_data._keyHex = hmac::to_hex(string(protocol_data._kewVector.begin(), protocol_data._kewVector.end()));

	UE_LOG(LogWalletConnect, Error, TEXT("%s%s"), *FString("Init[_keyHex]: "), *FString(protocol_data._keyHex.c_str()));
}

int UUE_WCModule::GenerateRandomInteger(int between)
{
	std::random_device rd;
	// Generates pseudo-random numbers, seeded by random_device
	std::default_random_engine generator{ rd() };
	// Transforms output of generator into desired range
	// In this case, between 1 and 10, inclusive
	std::uniform_int_distribution<int> dist{ 1, between };
	// Get number
	return 1;
}

void UUE_WCModule::OnWebSocketGetMessage(const FString& MessageString)
{
	auto yes = std::string(TCHAR_TO_UTF8(*FString(MessageString)));

	if (yes == "Hello Dolbaeb") {
		Subscribe();
		Publish();
	}

	if(GEngine != nullptr) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, MessageString);
}

char UUE_WCModule::GetRandomBridge()
{
	std::string alphaNumeric = "abcdefghijklmnopqrstuvwxyz0123456789";
	return alphaNumeric[GenerateRandomInteger(alphaNumeric.length())];
}

std::string UUE_WCModule::GetHttpsURL(char wc_bridge)
{
	std::string local_https = "https://";
	std::string local_url = ".bridge.walletconnect.org";

	return std::string(local_https + wc_bridge + local_url).c_str();
}

std::string UUE_WCModule::GetGuid()
{
	FGuid guid = FGuid::NewGuid();
	UE_LOG(LogWalletConnect, Warning, TEXT("%s"), *guid.ToString(EGuidFormats::DigitsWithHyphensLower));
	return std::string(TCHAR_TO_UTF8(*guid.ToString(EGuidFormats::DigitsWithHyphensLower)));
};

std::string UUE_WCModule::GetWssURL(char wc_bridge)
{
	std::string local_wss = "wss://";
	std::string local_url = ".bridge.walletconnect.org";
	UE_LOG(LogWalletConnect, Warning, TEXT("%s"), *FString(std::string(local_wss + wc_bridge + local_url).c_str()));

	return std::string(local_wss + wc_bridge + local_url).c_str();
}

FString UUE_WCModule::GetQrCodeURI()
{
	auto versionEncode = UrlEncode("1");
	auto bridgeUrlEncode = UrlEncode(GetHttpsURL(Bridge));
	auto keyEncoded = UrlEncode(protocol_data._keyHex);


	auto part1 = std::string("wc:" + std_str_handshakeTopic);

	auto part2 = std::string("@" + versionEncode + "?bridge=" + bridgeUrlEncode + "&key=" + keyEncoded);

	auto result = std::string(part1 + part2);

	UE_LOG(LogWalletConnect, Display, TEXT("%s"), *FString(part1.c_str()));
	UE_LOG(LogWalletConnect, Display, TEXT("%s"), *FString(part2.c_str()));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString(part1.c_str()));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString(part2.c_str()));

	FString QRCodeResult = FString(result.c_str());

	return QRCodeResult;
}

std::string UUE_WCModule::UrlEncode(std::string value)
{
	ostringstream escaped;
	escaped.fill('0');
	escaped << hex;

	for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << uppercase;
		escaped << '%' << setw(2) << int((unsigned char)c);
		escaped << nouppercase;
	}
	return escaped.str();
}

std::string UUE_WCModule::GetJsonRpcRequest()
{
	JSON request;

	request["params"] =
	{
		{{ "peerId", std_str_clientID.c_str() },
		{ "peerMeta", {
			{ "description", description.c_str() },
			{ "url", url.c_str() },
			{ "icons", {icons[0].c_str()} },
			{ "name", name.c_str() }}
		},
		{ "chainId", 1}}
	};

	int extra = GenerateRandomInteger(999999);
	
	request["id"] = int64(FDateTime::UtcNow().ToUnixTimestamp() * 1000000 + extra);
	request["jsonrpc"] = "2.0";
	request["method"] = "wc_sessionRequest";

	return request.dump();
}

void UUE_WCModule::WebSocketInit()
{
	auto real_url = GetWssURL(Bridge);

	auto cons = real_url;


	WebSocket = FWebSocketsModule::Get().CreateWebSocket(FString("ws://localhost:8080"));

	if (WebSocket != nullptr) 
	{
		WebSocket->OnConnected().AddLambda([]()
		{
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Succsess Connect!");
		});


		WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString("Code: " + Reason + " [WasClean: " + (bWasClean ? "true ]" : "false ]")));
		});

		WebSocket->OnMessage().AddUFunction(this, FName("OnWebSocketGetMessage"));

		//WebSocket->OnMessage().AddLambda([](const FString& MessageString)
		//{

		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, MessageString);
		//});

		UE_LOG(LogWalletConnect, Warning, TEXT("%s"), *FString(real_url.c_str()));

		WebSocket->Connect();

		InstantBridgeSocket();
	}

}

void UUE_WCModule::Subscribe()
{
	JSON subscribe =
	{
		  {"topic", std_str_clientID.c_str()},
		  {"type", "sub"},
		  {"payload", ""},
		  {"silent", true}
	};

	UE_LOG(LogWalletConnect, Warning, TEXT("%s"), *FString(subscribe.dump().c_str()));

	SendMessage(subscribe.dump());
}

void UUE_WCModule::Publish()
{
	JSON publish =
	{
		  {"topic", std_str_handshakeTopic.c_str()},
		  {"type", "pub"},
		  {"payload", GenerateCryptoJson().c_str()},
		  {"silent", true}
	};

	UE_LOG(LogWalletConnect, Warning, TEXT("Publish: %s"), *FString(publish.dump().c_str()));

	SendMessage(publish.dump());
}

void UUE_WCModule::Connect()
{
	WebSocketInit();
}

void UUE_WCModule::SendMessage(std::string msg)
{
	//Debug
	if (WebSocket != nullptr)
	{
		UE_LOG(LogWalletConnect, Display, TEXT("%s%s"), *FString("Sent message: "), *FString(msg.c_str()));
		WebSocket->Send(msg.c_str());
	}
}

std::vector<uint8_t> UUE_WCModule::GenerateIV()
{
	int size = 16;
	std::vector<uint8_t> iv;
	iv.resize(size);

	for (size_t index = 0;index < size; index++)
		iv[index] = GenerateRandomInteger(255);

	return iv;
}

std::vector<uint8_t> UUE_WCModule::GenerateKey()
{
	int size = 32;
	vector<uint8_t> key;
	key.resize(size);

	if (!RAND_bytes(&key[0], size)) {
		UE_LOG(LogWalletConnect, Error, TEXT("OpenSSL reports a failure, act accordingly"));
	}

	return key;
}

std::string UUE_WCModule::ConvertKeyToHex(std::vector<uint8_t>& key)
{
	return ConvertStringToHex(std::string(key.begin(), key.end()));
}

std::string UUE_WCModule::ConvertStringToHex(const std::string& input)
{
	static const char hex_digits[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(input.length() * 2);
	for (unsigned char c : input)
	{
		output.push_back(hex_digits[c >> 4]);
		output.push_back(hex_digits[c & 15]);
	}
	transform(output.begin(), output.end(), output.begin(), ::tolower);
	return output;
}

std::vector<uint8_t> UUE_WCModule::CreateCryptoString(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, std::string str)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);

	assert(key.size() == 32);  // AES256 key size
	assert(iv.size() == 16);   // IV is always the AES block size

	// If data isn't a multiple of 16, the default behavior is to pad with
	// n bytes of value n, where n is the number of padding bytes required
	// to make data a multiple of the block size.  This is PKCS7 padding.
	// The output then will be a multiple of the block size.
	
	std::vector<uint8_t> encrypted;
	size_t max_output_len = str.length() + 16 - (str.length() % 16);
	
	encrypted.resize(max_output_len);
	// Enc is 1 to encrypt, 0 to decrypt, or -1 (see documentation).
	EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), NULL, &key[0], &iv[0], 1);

	// EVP_CipherUpdate can encrypt all your data at once, or you can do
	// small chunks at a time.
	int actual_size = 0;
	EVP_CipherUpdate(ctx, &encrypted[0], &actual_size, reinterpret_cast<unsigned char*>(&str[0]), str.size());
		
	// EVP_CipherFinal_ex is what applies the padding.  If your data is
	// a multiple of the block size, you'll get an extra AES block filled
	// with nothing but padding.
	int final_size;
	EVP_CipherFinal_ex(ctx, &encrypted[actual_size], &final_size);
	
	actual_size += final_size;
	encrypted.resize(actual_size);

	EVP_CIPHER_CTX_cleanup(ctx);
	EVP_CIPHER_CTX_free(ctx);

	return encrypted;
}

std::string UUE_WCModule::GenerateCryptoJson()
{
	//1 AES 256
	//2 HASH 256
	//3 HMAC 
	std::vector<uint8_t> buffer;

	auto  aes_msg = CreateCryptoString(protocol_data._kewVector, protocol_data._iv, GetJsonRpcRequest());
	
	buffer.resize(aes_msg.size() + protocol_data._iv.size());

	for (size_t index_m = 0; index_m < aes_msg.size(); index_m++)
		buffer[index_m] = aes_msg[index_m];

	for (size_t index_iv = 0, index_b = aes_msg.size(); index_iv < buffer.size(); index_iv++, index_b++)
		buffer[index_b] = protocol_data._iv[index_iv];

	auto key = ConvertStringToHex(std::string(protocol_data._kewVector.begin(), protocol_data._kewVector.end()));

	auto hmac = ConvertStringToHex(CreateStringFromVectorUCharHMAC(protocol_data._kewVector, buffer));
	
	auto iv = ConvertStringToHex(std::string(protocol_data._iv.begin(), protocol_data._iv.end()));
	
	auto data = ConvertStringToHex(std::string(aes_msg.begin(), aes_msg.end()));

	JSON json_result =
	{
		{ "iv" ,  iv.c_str() },
		{ "hmac", hmac.c_str() },
		{ "data", data.c_str() }
	};

	UE_LOG(LogWalletConnect, Display, TEXT("%s%s"), *FString("GenerateCryptoJson[json_result]: "), *FString(json_result.dump().c_str()));

	return json_result.dump();
}

//void UUE_WCModule::Close()
//{
//	if (WebSocket) {
//		WebSocket->IsConnected() ? WebSocket->Close()
//			: GEngine != nullptr ? GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Socket not connected") : 0;
//	}
//}

void UUE_WCModule::InstantBridgeSocket()
{

	std::string temp;
	temp.push_back(Bridge);

	JsProtocol.UUID = GetGuid();

	JSON json_result =
	{
		{ "uuid", JsProtocol.UUID },
		{ "bridge" , temp }
	};

	WebSocket->Send(FString(json_result.dump().c_str()));
}

vector<uint8_t> UUE_WCModule::CovertHexStringToVectorUChar(const std::string& data)
{
	std::vector<uint8_t> payload;
	payload.resize(data.size() / 2);

	auto hex2uchar{ [](char data) -> unsigned char
	{
		if (data >= '0' && data <= '9')
			return data - '0';
		else if (data >= 'a' && data <= 'f')
			return 10 + data - 'a';
		else
			return 10 + data - 'A';
	} };

	for (int i = 0; i < data.size(); i += 2) 
		payload[i / 2] = hex2uchar(data[i]) * 16 + hex2uchar(data[i + 1]);	
	
	return payload;
}

std::string UUE_WCModule::CreateStringFromVectorUCharHMAC(const std::vector<uint8_t>& key, std::vector<uint8_t>& data)
{
	unsigned int hmac_size = 32;

	std::vector<uint8_t> result;
	result.resize(hmac_size);

	HMAC_CTX* hmac = HMAC_CTX_new();
	
	if (hmac != nullptr)
	{
		HMAC_CTX_reset(hmac);
		HMAC_Init(hmac, &key[0], key.size(), EVP_sha256());

		HMAC_Update(hmac, &data.front(), data.size());
		HMAC_Final(hmac, &result[0], &hmac_size);
		HMAC_CTX_free(hmac);
	}

	return std::string(result.begin(), result.end());
}

void UUE_WCModule::DebugFromCharpModule()
{
	std::vector<uint8_t> key = CovertHexStringToVectorUChar("f2cec9b1be75e1a480308449615ca4e14b41d4fa4322925888dd18b97bebb9e2");
	std::vector<uint8_t> iv  = CovertHexStringToVectorUChar("7e49cb9c3c889ab7f82a5dd16d03dc7f");
	std::vector<uint8_t> test_byte_message = CreateCryptoString(key, iv, "Congratulate");

	std::vector<uint8_t> msg_plus_iv;
	msg_plus_iv.resize(test_byte_message.size() + iv.size());


	for (size_t index = 0; index < test_byte_message.size(); index++)
		msg_plus_iv[index] = test_byte_message[index];
	
	for (size_t iv_index = test_byte_message.size(), i_iv = 0; iv_index < msg_plus_iv.size(); iv_index++, i_iv++)
		msg_plus_iv[iv_index] = iv[i_iv];


	auto what_are_you = CreateStringFromVectorUCharHMAC(key, msg_plus_iv);

	auto hmac = ConvertStringToHex(what_are_you);

	UE_LOG(LogWalletConnect, Error, TEXT("%s%s"), *FString("TEST[hmac]: "), *FString(hmac.c_str()));
}

void UUE_WCModule::ReadFileDebugJSON()
{
	FString result;
	FFileHelper::LoadFileToString(result, *(FPaths::GameSourceDir() + "test.txt"));

	auto json = JSON::parse(std::string(TCHAR_TO_UTF8(*result)));

	sharp.qr_code_uri     = json["qr_code_uri"].get<std::string>();
	sharp.wss_url         = json["wss_url"].get<std::string>();
	sharp.http_url		  = json["http_url"].get<std::string>();
	sharp.client_id		  = json["client_id"].get<std::string>();
	sharp.handshake_topic = json["handshake_topic"].get<std::string>();
	sharp.key_hex		  = json["key_hex"].get<std::string>();
	sharp.bridge		  = json["bridge"].get<std::string>()[0];
	sharp.iv			  = json["iv"].get<std::string>();
	sharp.hmac			  = json["hmac"].get<std::string>();
	sharp.data			  = json["data"].get<std::string>();

	
	UE_LOG(LogWalletConnect, Error, TEXT("%s%s"), *result);
}

void UUE_WCModule::TestMigration()
{
	//Success
	//auto json_rpc_request = models::json_rpc_request(
	//	new models::client_meta(
	//		std::string("An example that showcases how to use the WalletConnectSharp library"),
	//		std::string("https://app.warriders.com/"),
	//		std::vector<std::string>{ "https://app.warriders.com/favicon.ico" },
	//		std::string("WalletConnectSharp")
	//	),
	//	"9444e269-007b-4f62-a87e-5d7c9a8f9cb3").to_json().dump();

	////Success
	//auto encrypted_payload = models::encrypted_payload(
	//	"ef2cfa2d1797d80c1c3a839b8bac238f",
	//	"1ba9954554a3b8132159fa8ce21a9f75f2a81937fee465c3b56ad7478d21f27a",
	//	"3f90f49b22c6052e87332a4c3042055ec7ec8946e514f0a3d008f60f3a99390c26e15616d274115d0a240eca0ac68b36405776b5e14758765aa06d56c92d92d2c32876d0d4c60b5c6f21556243e8c9933ae5dff74606cd5662ccc2bc2dec72e3f53c55dbbaaa8f88e6b00021447969868fb3b5b287eab13428a40415ed81db3a1b6e2afe4ea98e6d78418aac30fd473c4f1b2ba9437a0aa41d0bc7104a8c7aa884c743d2a1d18f9986287e4b7edf5e1d0e59f3218ba0f65bea5bc9ab0534c097ec0d9661607fe385c20c573548bfed36ff814326b84cb70d60310d13ac7ab3e03fcb629ed6eeb017baf4632131971fccaa776af20dad57314d73c20f2114b7bb350365dcc5a7df050249a5f92f9528c38f5ccf9ea6030f312a84faf92b0f0e2875bbb36118b0644ebaa0291dcb5e57da67ee3a34adbd20d99109eb3cc74fb19d4ba59727eb88844143d157326f6a22b665e791e2489a50dcc424cda292e70b0993d66c4baa2a09f8b01518ef83231f3d"
	//).to_json().dump();

	////Success
	//auto network_message = models::network_message
	//(
	//	"b2e35c20-ac1d-41e7-a9a8-ccff8f66dfc1",
	//	models::network_message::etype::pub,
	//	encrypted_payload,
	//	false
	//).to_json().dump();

	//security::key = protocol_data._kewVector;

}

void UUE_WCModule::PostInitProperties()
{
	Super::PostInitProperties();

}