// Fill out your copyright notice in the Description page of Project Settings.


#include "WalletConnectModule.h"


void UWalletConnectModule::CreateNewSession(UWebSocketAdapter* UWebSocketAdapter)
{

	//SessionHandler = new ISessionHandler(
	//	FClientMeta.Description,
	//	FClientMeta.URL,
	//	FClientMeta.Icons,
	//	FClientMeta.Name,
	//	UWebSocketAdapter->WebSocket.Get()
	//);
}


FString UWalletConnectModule::GetQRCodeURI()
{
	//return FString(network::get_qr_code_uri(
	//	SessionHandler->protocol_storage->hanshake_topic,
	//	network::get_https_url(SessionHandler->wc_bridge),
	//	utils::convert_key_to_hex(SessionHandler->protocol_storage->key)
	//).c_str());
	return FString();
}

void UWalletConnectModule::OnMessageEvent(const FString& MessageString) {
	//auto yes = std::string(TCHAR_TO_UTF8(*FString(MessageString)));

	//if (yes == "Hello Dolbaeb") {
	//	if (SessionHandler != nullptr)
	//	{
	//		SessionHandler->subscribe();
	//		SessionHandler->publish();
	//	}
	//}

	//if (GEngine != nullptr) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, MessageString);
}

void UWalletConnectModule::WebSocketInit()
{
	 // = MakeShared<IWebSocket>(FWebSocketsModule::Get().CreateWebSocket(FString("ws://localhost:8080")));

}


Status UWalletConnectModule::GetConnectionStatus()
{
	return StatusWalletConnection;
}

void UWalletConnectModule::CallWhenEndPlay()
{

		
	StatusWalletConnection = Status::Disconnected;
}
