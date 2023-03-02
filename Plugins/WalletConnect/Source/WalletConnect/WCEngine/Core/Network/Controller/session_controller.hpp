

class session_controller {
	
	using json = nlohmann::ordered_json;
	using action = bool (session_controller::*)(const std::string& msg);

public:

	enum class connection_status : uint8_t {
		disconnected,
		connected
	};

	explicit session_controller(const FString& Description,const FString& URL,const TArray<FString>& Icons,
		const FString& Name,IWebSocket* webSocket, FWalletConnectSignature* fWalletConnectSignature) noexcept {
		
		assert(webSocket.IsValid());
		assert(!Description.IsEmpty());
		assert(!URL.IsEmpty());
		assert(!Name.IsEmpty());

		protocol_storage = new security::protocol_storage();

		protocol_storage->key = crypto::generate_key();
		protocol_storage->iv = crypto::generate_iv();
		
		protocol_storage->client_id = crypto::generate_guid();
		protocol_storage->hanshake_topic = crypto::generate_guid();

		web_socket_ptr = webSocket;
		wc_bridge = network::get_random_bridge();
		protocol_storage->wc_bridge = wc_bridge;
		init_session_actions();

		client_meta = new models::client_meta(std::string(TCHAR_TO_UTF8(*Description)),std::string(TCHAR_TO_UTF8(*URL)),
			std::vector<std::string>{ TCHAR_TO_UTF8(*Icons[0]) },std::string(TCHAR_TO_UTF8(*Name)));

		communication_controller_ptr = std::make_shared<network::communication_controller>
			(protocol_storage, client_meta, web_socket_ptr, fWalletConnectSignature);
	}

	 bool ParseMessage(FString message) noexcept {
		return communication_controller_ptr->parse_string(TCHAR_TO_UTF8(*message));;
	}

	 FString GetQrCodeURI() noexcept {
		return network::get_qr_code_uri(protocol_storage->hanshake_topic, network::get_https_url(wc_bridge),
			utils::convert_key_to_hex(protocol_storage->key)).c_str();
	}
	
	 void EthPersonalSign(const FString& _Address, const FString& _Message) {

		auto result = SaveSession();

		if (communication_controller_ptr->wallet_session_ptr.get() != nullptr) {

			auto address = communication_controller_ptr->wallet_session_ptr->get_accounts()[0];
			//auto message = std::string("0x" + utils::convert_string_to_hex(TCHAR_TO_UTF8(*_Message)));


			//auto message = std::string("0x4a9fb9150a2d441a8cde6989e9eeac6d3e1240536d746fda319b8c40812104b5");
			auto message = std::string("Check");

			std::shared_ptr<params::universal_params > params(new params::universal_params(json{  address, message }));

			auto json_rpc_show = models::json_rpc_request(params.get(), "eth_sign").get_eth_trans().dump();
			

			auto show_key = utils::convert_key_to_hex(protocol_storage->key);
			auto eth_iv = crypto::generate_iv();

			auto crypto_json_rpc = crypto::generate_encrypted_payload(
				protocol_storage->key, eth_iv,
				json_rpc_show).to_json().dump();

			auto network_msg_sub = models::network_message(
				communication_controller_ptr->wallet_session_ptr->get_peer_id(),
				models::network_message::etype::pub,
				crypto_json_rpc,
				false

			).to_json();

			std::shared_ptr<params::universal_params> params_two(new params::universal_params(network_msg_sub));

			auto final_message = models::json_rpc_request(
				params_two.get(),
				"eth_send_transaction",
				protocol_storage->js_bridge_uuid
			).to_json().dump();

			if (web_socket_ptr != nullptr) {
				web_socket_ptr->Send(models::json_rpc_request(
					params_two.get(),
					"eth_send_transaction",
					protocol_storage->js_bridge_uuid
				).to_json().dump().c_str());
			}
		}
	}

	 void EthSendTransaction(
		const std::string& _from,
		const std::string& _to,
		const std::string& _value,
		const std::string& _data) {

		if (communication_controller_ptr->wallet_session_ptr.get() != nullptr) {

			auto transaction_data_show = eth::eth_transaction_data(_from, _to, utils::string_to_uint64(_value), _data).get_params().dump();

			auto transaction_data = eth::eth_transaction_data(_from, _to, utils::string_to_uint64(_value), _data).get_json_eth_transaction().dump();

			auto eth_iv = crypto::generate_iv();

			auto crypto_json_rpc = crypto::generate_encrypted_payload(
				protocol_storage->key, eth_iv,
				transaction_data).to_json().dump();

			auto network_msg_sub = models::network_message(
				communication_controller_ptr->wallet_session_ptr->get_peer_id(),
				models::network_message::etype::pub,
				crypto_json_rpc,
				false

			).to_json();

			std::shared_ptr<params::universal_params> params_two(new params::universal_params(network_msg_sub));

			auto final_message = models::json_rpc_request(
				params_two.get(),
				"eth_send_transaction",
				protocol_storage->js_bridge_uuid
			).to_json().dump();

			if (web_socket_ptr != nullptr) {
				web_socket_ptr->Send(final_message.c_str());
			}
		}
	}


	 void SendEthTransaction(const FString& _From, const FString& _To, const FString& _Value, const FString& _Data) noexcept {

		if (communication_controller_ptr->wallet_session_ptr.get() != nullptr) 
		{

			EthPersonalSign("0x7A0762b62436EE5F266e422DF547F15592ce3c86", "Hey Dolbaeb Zdarova");

			EthSendTransaction(
				TCHAR_TO_UTF8(*_From),
				TCHAR_TO_UTF8(*_To),
				TCHAR_TO_UTF8(*_Value),
				communication_controller_ptr->sign_hash);

			//auto params = std::shared_ptr<eth_transaction_data>(new eth_transaction_data(
			//	communication_controller_ptr->wallet_session_ptr->get_accounts()[0],//TCHAR_TO_UTF8(*_From),
			//	TCHAR_TO_UTF8(*_To),
			//	utils::string_to_uint64(TCHAR_TO_UTF8(*_Value)),
			//	utils::string_to_uint64(TCHAR_TO_UTF8(*_Data))));

			//auto json_rpc_show = models::json_rpc_request(params.get(), "eth_sendTransaction").to_json().dump();


			//auto show_msg = params->get_json_eth_transaction().dump();


			//auto eth_iv = crypto::generate_iv();

			//auto json_rpc = crypto::generate_encrypted_payload(
			//	protocol_storage->key, eth_iv,
			//	params->get_json_eth_transaction().dump()).to_json().dump();
			//
			//auto network_msg_sub = models::network_message(
			//	communication_controller_ptr->wallet_session_ptr->get_peer_id(),
			//	models::network_message::etype::pub,
			//	json_rpc,
			//	false

			//).to_json().dump();

			//std::shared_ptr<params::universal_params> params_two(new params::universal_params(network_msg_sub));

			//auto final_message = models::json_rpc_request(
			//	params_two.get(),
			//	"eth_send_transaction",
			//	protocol_storage->js_bridge_uuid
			//).to_json().dump();

			//if (web_socket_ptr != nullptr) {
			//	web_socket_ptr->Send(final_message.c_str());
			//}
		}
	}

	~session_controller() noexcept {
		if (client_meta != nullptr) { delete client_meta; }
		if (protocol_storage != nullptr) { delete protocol_storage; }
	}

public:
	 const TArray<FString> GetAccounts() const {
		auto accounts = wallet_session->get_accounts();
		auto unreal_array = TArray<FString>();

		for (auto account : accounts) 
			unreal_array.Add(account.c_str());

		return unreal_array;
	}

	 const bool GetApproved() const {
		return wallet_session->get_approved();
	}

	 const uint8 GetChainId() const {
		return wallet_session->get_chain_id();
	}

	 const FString GetPeerId() const {
		return wallet_session->get_peer_id().c_str();
	}

	 const FString GetPeerMetaDescription() const {
		return wallet_session->get_peer_meta().get_description().c_str();
	}

	 const TArray<FString> GetPeerMetaIcons() const {
		TArray<FString> icons;

		for (auto icon : wallet_session->get_peer_meta().get_icons()) 
			icons.Add(icon.c_str());
		
		return icons;
	}

	 const FString GetPeerMetaURL() const {
		return wallet_session->get_peer_meta().get_url().c_str();
	}

	 const FString GetPeerMetaName() const {
		return wallet_session->get_peer_meta().get_name().c_str();
	}

	 const bool SaveSession() {

		auto accounts		= communication_controller_ptr->wallet_session_ptr->get_accounts();
		auto key_byte		= protocol_storage->key;
		auto key_hex		= utils::convert_key_to_hex	(protocol_storage->key);
		auto bridge_url		= network::get_https_url(protocol_storage->wc_bridge);
		auto peer_id		= communication_controller_ptr->wallet_session_ptr->get_peer_id();
		auto client_id		= protocol_storage->client_id;
		auto chain_id		= communication_controller_ptr->wallet_session_ptr->get_chain_id();
		auto network_id		= 0;
		auto handshake_id	= protocol_storage->hanshake_topic;
		auto d_app_meta		= *client_meta;
		auto peer_meta		= *client_meta;
		auto approved		= communication_controller_ptr->wallet_session_ptr->get_approved();

		saved_session_ptr = std::make_shared<security::saved_session>(
			accounts,
			key_byte,
			key_hex,
			bridge_url,
			peer_id,
			client_id,
			chain_id,
			network_id,
			handshake_id,
			d_app_meta,
			peer_meta,
			approved );
		
		auto result = saved_session_ptr->save_to_encrypted_file();

		return false;
	}

private:


	 bool eth_send_transaction(const std::string& msg) noexcept {
		 return true;
	}

	 bool bridge_socket_open(const std::string& msg) noexcept {
		std::shared_ptr<params::universal_params> params(new params::universal_params(json{ "bridge" , std::string(1, wc_bridge) }));
		
		protocol_storage->js_bridge_uuid = json::parse(msg)["id"].get<std::string>();
		auto allmsg = models::json_rpc_request(
			params.get(),
			"create_wc_client",
			protocol_storage->js_bridge_uuid
		).to_json().dump();

		if (web_socket_ptr != nullptr) {
			web_socket_ptr->Send(models::json_rpc_request(
				params.get(),
				"create_wc_client",
				protocol_storage->js_bridge_uuid
			).to_json().dump().c_str());
			return true;
		}
		return false;
	}

	 bool wallet_connect_socket_open(const std::string& msg) noexcept {


		auto network_msg_sub = models::network_message(protocol_storage->client_id, models::network_message::etype::sub,
			std::string(), true).to_json();

		std::shared_ptr<params::universal_params> params(new params::universal_params(network_msg_sub));

		auto json_rpc_request_sub = models::json_rpc_request
			(params.get(), "wallet_connect_session_request", protocol_storage->js_bridge_uuid).to_json().dump();

		web_socket_ptr->Send(json_rpc_request_sub.c_str());


		std::shared_ptr<params::wc_session_request_params> params_ptr
			(new params::wc_session_request_params(protocol_storage->client_id, client_meta));

		auto network_msg_pub = models::network_message(
			protocol_storage->hanshake_topic,
			models::network_message::etype::pub,
			crypto::generate_encrypted_payload(
				protocol_storage->key, protocol_storage->iv,
				models::json_rpc_request(
					params_ptr.get(),
					std::string("wc_sessionRequest")
				).to_json().dump()
			).to_json().dump(),
			true).to_json();
				

		std::shared_ptr<params::universal_params> params_two(new params::universal_params(network_msg_pub));

		auto json_rpc_request_pub = models::json_rpc_request
			(params_two.get(), "wallet_connect_session_request", protocol_storage->js_bridge_uuid).to_json().dump();

		web_socket_ptr->Send(json_rpc_request_pub.c_str());
		return true;
	}

	 bool wallet_connect_response(const std::string& msg) noexcept {

		//try {
			auto bridge_data = json::parse(msg)["result"]["data"].get<std::string>();

			auto payload = json::parse(bridge_data)["payload"].get<std::string>();

			auto data = json::parse(payload)["data"].get<std::string>();

			auto iv = utils::convert_hex_string_to_vector_uchar(json::parse(payload)["iv"].get<std::string>());

			auto hmac = json::parse(payload)["hmac"].get<std::string>();

			auto key = utils::convert_string_to_hex(std::string(protocol_storage->key.begin(), protocol_storage->key.end()));

			auto decrypt_result = crypto::decrypt_aes_256_cbc_PKCS7(data, protocol_storage->key, iv);

			auto result = json::parse(decrypt_result.c_str())["result"];

			wallet_session = std::shared_ptr<security::wallet_session>(new security::wallet_session(result));
				
		//}

		//catch(std::exception error){
		//	return false;
		//}

		return true;
	}

	 bool start_session(const std::string& msg) noexcept { return false; }
		

private:
	char wc_bridge;
	
	std::unordered_map<std::string, action> session_actions;
	std::shared_ptr<security::wallet_session> wallet_session;
	std::shared_ptr<security::saved_session> saved_session_ptr;
	std::shared_ptr<network::communication_controller> communication_controller_ptr;
	security::protocol_storage* protocol_storage;
	connection_status connection_status;
	models::client_meta* client_meta;
	IWebSocket* web_socket_ptr;

private:
	void init_session_actions() noexcept  {
		session_actions.insert({ "start_session",			   &session_controller::start_session				});
		session_actions.insert({ "bridge_socket_open",		   &session_controller::bridge_socket_open			});
		session_actions.insert({ "wallet_connect_socket_open", &session_controller::wallet_connect_socket_open	});
		session_actions.insert({ "wallet_connect_response",    &session_controller::wallet_connect_response		});
	}

private:
	session_controller(session_controller& other) = delete;
	session_controller(session_controller&& other) = delete;
	session_controller& operator= (session_controller&) = delete;
	session_controller& operator= (session_controller&&) = delete;
};