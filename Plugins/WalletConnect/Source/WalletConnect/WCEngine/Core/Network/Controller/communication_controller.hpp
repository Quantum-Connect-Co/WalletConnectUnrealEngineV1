
namespace network {

	class communication_controller {

		using json = nlohmann::ordered_json;
		using action = bool (communication_controller::*)(const std::string& msg);
		using unreal_web_socket =  IWebSocket;

	public:
		explicit communication_controller(
			security::protocol_storage* _protocol_storage_ptr,
			models::client_meta* _client_meta_ptr,
			unreal_web_socket* _unreal_web_socket_ptr,
			FWalletConnectSignature* fWalletConnectSignature) noexcept :

				protocol_storage_ptr(_protocol_storage_ptr),
				client_meta_ptr(_client_meta_ptr),
				unreal_web_socket_ptr(_unreal_web_socket_ptr),
				get_wallet_session_event(fWalletConnectSignature),
				generation_controller_ptr(std::make_shared<network::generation_controller>(_protocol_storage_ptr, _client_meta_ptr)){};

	public:
		 bool parse_string(const std::string& input_str) noexcept {
			auto method = utils::safety_get_from_json<std::string>(input_str, std::string("result"), std::string("method"));
			return session_actions.count(method) > 0 ? (this->*session_actions.at(method))(json(input_str)) : false;
		}

	private:

		 bool bridge_socket_open(const std::string& msg) {
			if (protocol_storage_ptr != nullptr)
			{
				unreal_web_socket_ptr->Send(generation_controller_ptr->create_wc_client(msg).c_str());
				return true;
			}
			return false;
		}

		 bool wallet_connect_socket_open(const std::string& msg) {
			if (protocol_storage_ptr != nullptr) 
			{
				unreal_web_socket_ptr->Send(generation_controller_ptr->wc_session_request_params_sub().c_str());
				unreal_web_socket_ptr->Send(generation_controller_ptr->wc_session_request_params_pub().c_str());

				auto network = models::network_message(protocol_storage_ptr->client_id, models::network_message::etype::ack, "", true).to_json();
				auto show = network.dump();

				std::shared_ptr<params::universal_params> params_two(new params::universal_params(network));

				auto final_message = models::json_rpc_request(
					params_two.get(),
					"wallet_connect_ack",
					protocol_storage_ptr->js_bridge_uuid
				).to_json().dump();

				unreal_web_socket_ptr->Send(final_message.c_str());

				return true;
			}
			return false;
		}

		 bool wallet_connect_save_sign(const std::string& msg) {
			sign_hash = msg;
			return true;
		}

		 bool wallet_connect_save_session(const std::string& msg) {
			wallet_session_ptr = std::make_shared<security::wallet_session>
				(std::get<1>(generation_controller_ptr->get_decrypt_result(msg)));

			FClientMeta f_client_meta;
			f_client_meta.Description = wallet_session_ptr->get_peer_meta().get_description().c_str();
			f_client_meta.URL         = wallet_session_ptr->get_peer_meta().get_url().c_str();
			f_client_meta.Name        = wallet_session_ptr->get_peer_meta().get_name().c_str();

			for (auto icon : wallet_session_ptr->get_peer_meta().get_icons()) 
				f_client_meta.Icons.Add(icon.c_str());
			
			FSession f_session;
			f_session.ClientMeta = f_client_meta;
			f_session.Approved   = wallet_session_ptr->get_approved();
			f_session.ChainId    = wallet_session_ptr->get_chain_id();
			f_session.PeerId     = wallet_session_ptr->get_peer_id().c_str();
				
			for (auto account : wallet_session_ptr->get_accounts()) {
				f_session.Accounts.Add(account.c_str());
			}
			
			if (unreal_web_socket_ptr != nullptr) {

				auto network = models::network_message(protocol_storage_ptr->client_id, models::network_message::etype::ack, "", true).to_json();
				auto show = network.dump();

				std::shared_ptr<params::universal_params> params_two(new params::universal_params(network));

				auto final_message = models::json_rpc_request(
					params_two.get(),
					"wallet_connect_ack",
					protocol_storage_ptr->js_bridge_uuid
				).to_json().dump();

				unreal_web_socket_ptr->Send(final_message.c_str());
			}

			get_wallet_session_event->Broadcast(f_session);
			return true;
		}

		 bool wallet_connect_response(const std::string& msg) 
		{
			//try 
			//{

			auto method = generation_controller_ptr->select_response_action2(msg);

			auto show = json::parse(msg)["result"].dump();

			return response_actions.count(method) > 0 ? (this->*response_actions.at(method))(json::parse(msg).dump()) : false;


			//switch (generation_controller_ptr->select_response_action(msg)) 
			//{
			//	case response_action::save_session:			  return wallet_connect_save_session(msg);
			//	case response_action::save_transactrion_hash: return wallet_connect_save_sign(msg);

			//	default: false;
			//}
			// 
			//}
			//catch (std::exception error)
			//{
			//	return false;
			//}d
		}

		 bool parse_error(const std::string& msg) { return false; }
			
	public:
		std::shared_ptr<security::wallet_session> wallet_session_ptr;
		std::string sign_hash;
	private:

		security::protocol_storage* protocol_storage_ptr;
		models::client_meta* client_meta_ptr;
		unreal_web_socket* unreal_web_socket_ptr;
		

	private:

		FWalletConnectSignature* get_wallet_session_event;

		std::shared_ptr<network::generation_controller> generation_controller_ptr;

	private:

		std::unordered_map<std::string, action> session_actions
		{
			{ "bridge_socket_open",			&communication_controller::bridge_socket_open         },
			{ "wallet_connect_socket_open", &communication_controller::wallet_connect_socket_open },
			{ "wallet_connect_response",    &communication_controller::wallet_connect_response    },
			{ "parse_error",                &communication_controller::parse_error                }
		};

		std::unordered_map<std::string, action> response_actions
		{
			{ "wallet_connect_save_session",&communication_controller::wallet_connect_save_session	},
			{ "wallet_connect_save_sign",	&communication_controller::wallet_connect_save_sign		}
		};

	};
}