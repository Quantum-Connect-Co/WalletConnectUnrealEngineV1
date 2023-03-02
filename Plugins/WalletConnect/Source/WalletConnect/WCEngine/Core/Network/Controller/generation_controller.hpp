namespace network {

	enum class response_action : uint8_t 
	{
		read_error,
		save_session,
		save_transactrion_hash
	};


	class generation_controller {

		using json = nlohmann::ordered_json;

	public:
		explicit generation_controller(
			security::protocol_storage* _protocol_storage_ptr,
			models::client_meta* _client_meta_ptr) noexcept :

				protocol_storage_ptr(_protocol_storage_ptr),
				client_meta_ptr(_client_meta_ptr) {}

	public:

		 const std::string create_wc_client(const std::string& msg) {
			std::shared_ptr<params::universal_params> params
				(new params::universal_params
					(json{ "bridge" , std::string(1, protocol_storage_ptr->wc_bridge) }));

			protocol_storage_ptr->js_bridge_uuid = json::parse(msg)["id"].empty() ?
				std::string() : json::parse(msg)["id"].get<std::string>();

			return models::json_rpc_request
				(params.get(), "create_wc_client", protocol_storage_ptr->js_bridge_uuid).to_json().dump();
		}

		 const std::string wc_session_request_params_sub() {
			auto network_msg_sub = models::network_message
			(protocol_storage_ptr->client_id, models::network_message::etype::sub, std::string(), true).to_json();

			std::shared_ptr<params::universal_params> params(new params::universal_params(network_msg_sub));

			return models::json_rpc_request
			(params.get(), "wallet_connect_session_request", protocol_storage_ptr->js_bridge_uuid).to_json().dump();
		}

		 const std::string wc_session_request_params_pub() {
			std::shared_ptr<params::wc_session_request_params> params_ptr
				(new params::wc_session_request_params(protocol_storage_ptr->client_id, client_meta_ptr));

			auto json_session_request = models::json_rpc_request
				(params_ptr.get(), std::string("wc_sessionRequest")).to_json().dump();

			auto encrypted_payload = crypto::generate_encrypted_payload
				(protocol_storage_ptr->key, protocol_storage_ptr->iv, json_session_request).to_json().dump();

			auto network_msg_pub = models::network_message
				(protocol_storage_ptr->hanshake_topic, models::network_message::etype::pub, encrypted_payload, true).to_json();

			std::shared_ptr<params::universal_params> params_two(new params::universal_params(network_msg_pub));

			return models::json_rpc_request
				(params_two.get(), "wallet_connect_session_request", protocol_storage_ptr->js_bridge_uuid).to_json().dump();
		}

		 const response_action select_response_action(const std::string& msg) {
			//auto res = get_decrypt_result(msg)["peerId"];

			//return res.empty() ?
			//	response_action::save_transactrion_hash : response_action::save_session;
			 return response_action::read_error;
		}



		 const std::string select_response_action2(const std::string& msg) {

			auto test = get_decrypt_result(msg);

			auto flag = std::get<0>(test);
			auto result = std::get<1>(test);

			if (flag == response_action::save_session) {
				return std::string("wallet_connect_save_session");

			}
			if (flag == response_action::save_transactrion_hash){
				return std::string("wallet_connect_save_sign");
			}
			else {
				return std::string();
			}
		}


		 std::tuple<response_action, json> get_decrypt_result(const std::string& msg) {

			auto bridge_data	= json::parse(msg)["result"]["data"].get<std::string>();
			auto payload		= json::parse(bridge_data)["payload"].get<std::string>();
			auto data			= json::parse(payload)["data"].get<std::string>();
			auto iv				= utils::convert_hex_string_to_vector_uchar(json::parse(payload)["iv"].get<std::string>());
			auto hmac			= json::parse(payload)["hmac"].get<std::string>();//Create validate hmac later
			auto decrypt		= crypto::decrypt_aes_256_cbc_PKCS7(data, protocol_storage_ptr->key, iv);

			auto test = utils::safety_get_from_json<json>(decrypt, std::string("result"));

			response_action status = response_action::save_session;

			if (test.empty()) {
				status = response_action::save_transactrion_hash;
			}
			
			if (!test["error"].empty()) {
				status = response_action::read_error;
			}

			return std::tuple<response_action, json> (status, test);
		}

	private:
		security::protocol_storage* protocol_storage_ptr;
		models::client_meta* client_meta_ptr;
		
	};

}