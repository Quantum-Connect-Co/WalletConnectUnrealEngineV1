#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>  

#include "Kismet/KismetSystemLibrary.h"


#include "WalletConnectModule.h"
#include <WalletConnect/Public/WebSo.h>

#pragma region json
#include <WalletConnect/WCEngine/Core/JSON/json_lib.hpp>
#pragma endregion json


#pragma region utils
#include <WalletConnect/WCEngine/Core/Utils/Methods/utils_methods.hpp>
#pragma endregion utils


#pragma region models

	#pragma region interface
	#include <WalletConnect/WCEngine/Core/Models/Interface/model_interface.hpp>
	#include <WalletConnect/WCEngine/Core/Models/Interface/params_interface.hpp>
	#pragma endregion interface

	#pragma region json rpc
	#include <WalletConnect/WCEngine/Core/Models/JsonRpc/json_rpc_request.hpp>
	#include <WalletConnect/WCEngine/Core/Models/JsonRpc/json_rpc_response.hpp>
	#pragma endregion json rpc

	#pragma region wallet connect V1
	#include <WalletConnect/WCEngine/Core/Models/WalletConnectV1/client_meta.hpp>
	#include <WalletConnect/WCEngine/Core/Models/WalletConnectV1/encrypted_payload.hpp>
	#include <WalletConnect/WCEngine/Core/Models/WalletConnectV1/network_message.hpp>
	#include <WalletConnect/WCEngine/Core/Models/WalletConnectV1/wc_session_request_params.hpp>
	#pragma endregion wallet connect V1

#pragma endregion models


#pragma region crypto
#include <WalletConnect/WCEngine/Core/Crypto/crypto_methods.hpp>
#pragma endregion crypto


#pragma region security
#include <WalletConnect/WCEngine/Core/Security/wallet_connect_protocol.hpp>
#include <WalletConnect/WCEngine/Core/Security/wallet_connect_session.hpp>
#include <WalletConnect/WCEngine/Core/Security/wallet_connect_saved_session.hpp>
#pragma endregion security


#pragma region ethereum
#include <WalletConnect/WCEngine/Ethereum/Models/eth_transaction_data.hpp>
#pragma endregion ethereum


#pragma region network
	
	#pragma region methods 
	#include <WalletConnect/WCEngine/Core/Network/Methods/network_methods.hpp>
	#pragma endregion methods 
	
	#pragma region controller 
	#include <WalletConnect/WCEngine/Core/Network/Controller/generation_controller.hpp>
	#include <WalletConnect/WCEngine/Core/Network/Controller/communication_controller.hpp>
	#include <WalletConnect/WCEngine/Core/Network/Controller/session_controller.hpp>
	#pragma endregion controller 
	
#pragma endregion network
