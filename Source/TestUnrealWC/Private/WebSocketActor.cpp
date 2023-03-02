// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketActor.h"
#include <Async/Async.h>

DEFINE_LOG_CATEGORY_STATIC(LogWebSocketActor, All, All);

// Sets default values
AWebSocketActor::AWebSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWebSocketActor::BeginPlay()
{
	AActor::BeginPlay();

	const char* ConnectionString = "wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self";

	UE_LOG(LogWebSocketActor, Warning, TEXT("wss://3.bridge.walletconnect.org"));


	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	

	TFunction<int()> Task = []()
	{
		FPlatformProcess::Sleep(4);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
		return 123;
	};

	auto Result = Async(EAsyncExecution::Thread, Task);



	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ConnectionString);

	WebSocket->OnConnected().AddLambda([]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Seccsess conected");
		});


	//WebSocket->OnClosed().AddLambda([](const FString& Error)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Error);
	//});


	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool WasClean)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, WasClean ? FColor::Green : FColor::Red, "ConnectionClosed" + Reason);
		});


	WebSocket->OnMessage().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Resived message: " + MessageString);
		});

	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
		});

	WebSocket->Connect();
}

void AWebSocketActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
}

// Called every frame
void AWebSocketActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

