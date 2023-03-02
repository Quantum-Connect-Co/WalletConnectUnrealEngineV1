// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/WClient.h"

// Sets default values for this component's properties
UWClient::UWClient()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	FClientMeta fClientMeta;

	fClientMeta.Description = "An example that showcases how to use the WalletConnectSharp library";
	fClientMeta.URL = "https://app.warriders.com/";
	fClientMeta.Icons.Add("https://app.warriders.com/favicon.ico");
	fClientMeta.Name = "WalletConnectUnreal";

	PrimaryComponentTick.bCanEverTick = true;

}

void UWClient::BeginPlay()
{
	UActorComponent::BeginPlay();
}

void UWClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWClient::Deallocate();
	//w
	UActorComponent::EndPlay(EndPlayReason);
}


void UWClient::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FString UWClient::CreateNewSession(FClientMeta InClientMeta, FString Adress)
{
	UWClient::ClientMeta = InClientMeta;

	//auto test = security::saved_session(std::move(security::saved_session::get_saved_session_from_file()));
	
	UWClient::WebSocketInit(Adress);

	return SessionHandler != nullptr ? SessionHandler->GetQrCodeURI() : FString();
}

FSession UWClient::GetSessionData()
{
	if (SessionHandler != nullptr)
	{
		Session.Accounts = SessionHandler->GetAccounts();
		Session.Approved = SessionHandler->GetApproved();
		Session.ChainId  = SessionHandler->GetChainId();
		Session.PeerId   = SessionHandler->GetPeerId();

		Session.ClientMeta.Description = SessionHandler->GetPeerMetaDescription();
		Session.ClientMeta.Icons       = SessionHandler->GetPeerMetaIcons();
		Session.ClientMeta.Name        = SessionHandler->GetPeerMetaName();
		Session.ClientMeta.URL         = SessionHandler->GetPeerMetaURL();
	}

	return Session;
}

void UWClient::SendEthTransaction(FString _From, FString To, FString Value, FString Data)
{
	if (SessionHandler != nullptr) 
	{
		SessionHandler->SendEthTransaction(FString(), To, Value, Data);
	}
}

void UWClient::WebSocketClose()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected()) {
		WebSocket->Close();
	}
}

void UWClient::WebSocketInit(FString Adress)
{

	auto check = FModuleManager::Get().LoadModule("WebSockets");

	if (check != nullptr)
	{
		WebSocket = FWebSocketsModule::Get().CreateWebSocket(Adress);

		SessionHandler = new session_controller(ClientMeta.Description, ClientMeta.URL,
			ClientMeta.Icons, ClientMeta.Name, WebSocket.Get(), &GetWalletSessionData);

		if (WebSocket != nullptr)
		{
			WebSocket->OnConnected().AddUFunction(this, FName("OnWebSocketConnected"));

			WebSocket->OnClosed().AddUFunction(this, FName("OnWebSocketClosed"));

			WebSocket->OnConnectionError().AddUFunction(this, FName("OnWebSocketError"));

			WebSocket->OnMessage().AddUFunction(this, FName("OnWebSocketGetMessage"));

			WebSocket->Connect();
		}
	}
}

void UWClient::OnWebSocketConnected()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Succsess Connect!");
	}
}


void UWClient::OnWebSocketGetMessage(const FString& MessageString)
{

	if (SessionHandler->ParseMessage(MessageString)) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, MessageString);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, MessageString);
	}
}

void UWClient::OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	if (GEngine != nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString("Code: " + Reason + " [WasClean: " + (bWasClean ? "true ]" : "false ]")));
	}
}
void UWClient::OnWebSocketError(const FString& Error)
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Error code: " + Error));
	}
}

void UWClient::Deallocate()
{
	if (SessionHandler != nullptr) { delete SessionHandler; }
}