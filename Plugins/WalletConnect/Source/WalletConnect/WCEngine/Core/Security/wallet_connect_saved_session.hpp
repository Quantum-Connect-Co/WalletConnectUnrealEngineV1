namespace security {

	const static std::vector<uint8_t> saved_key =
		utils::convert_hex_string_to_vector_uchar("657af180f5415dc3aa4600ab173aba2a");;
	const static std::vector<uint8_t> saved_iv =
		utils::convert_hex_string_to_vector_uchar("7d05cf266984f4ecd7b24a9b8b9fab44650a2df326b266a0f3c63f4a461f51d2");

	class saved_session {

		using json = nlohmann::ordered_json;

	public:
		explicit saved_session(
			const std::vector<std::string>&		_accounts,
			const std::vector<std::uint8_t>&	_key_byte,
			const std::string&					_key_hex,
			const std::string&					_bridge_url,
			const std::string&					_peer_id,
			const std::string&					_client_id,
			const std::uint32_t					_chain_id,
			const std::uint32_t					_network_id,
			const std::string&					_handshake_id,
			const models::client_meta&			_d_app_meta,
			const models::client_meta&			_peer_meta,
			const std::boolean					_approved

		) noexcept :

			accounts		(_accounts		),
			key_byte		(_key_byte		),
			key_hex			(_key_hex		),
			bridge_url		(_bridge_url	),
			peer_id			(_peer_id		),
			client_id		(_client_id		),
			chain_id		(_chain_id		),
			network_id		(_network_id	),
			handshake_id	(_handshake_id	),
			d_app_meta		(_d_app_meta	),
			peer_meta		(_peer_meta		),
			approved		(_approved		) {}
			

		saved_session(saved_session&& other) noexcept :
			accounts		(std::move(other.accounts)		),
			key_byte		(std::move(other.key_byte)		),
			key_hex			(std::move(other.key_hex)		),
			bridge_url		(std::move(other.bridge_url)	),
			peer_id			(std::move(other.peer_id)		),
			client_id		(std::move(other.client_id)		),
			chain_id		(std::move(other.chain_id)		),
			network_id		(std::move(other.network_id)	),
			handshake_id	(std::move(other.handshake_id)	),
			d_app_meta		(std::move(other.d_app_meta)	),
			peer_meta		(std::move(other.peer_meta)		),
			approved		(std::move(other.approved)		) {}

		
	public:
		 const bool save_to_encrypted_file() {
			auto game_path = std::string(TCHAR_TO_UTF8(*UKismetSystemLibrary::GetProjectDirectory()));
			auto target_folder = "\\Plugins\\WalletConnect\\Cache\\file_generate.txt";

			//try {
				auto outfile = std::ofstream(game_path + target_folder);

				if (outfile.is_open()) {

					auto encrypt_msg = crypto::encrypt_aes_256_cbc_PKCS7(saved_key, saved_iv, get_data_json_fotmat());
					auto data = utils::convert_string_to_hex(std::string(encrypt_msg.begin(), encrypt_msg.end()));
					outfile << data;
					outfile.close();
				}
				//return true;
			//}
			//catch (std::exception error) {
			//	//print(error)
			//}
			return false;
		}



		 const std::string get_data_json_fotmat() {
			auto result = json
			{
				{ "accounts",		accounts				},
				{ "key_byte",		key_byte				},
				{ "key_hex",		key_hex					},
				{ "bridge_url",		bridge_url				},
				{ "peer_id",		peer_id					},
				{ "client_id",		client_id				},
				{ "chain_id",		chain_id				},
				{ "network_id",		network_id				},
				{ "handshake_id",	handshake_id			},
				{ "d_app_meta",		d_app_meta.to_json()	},
				{ "peer_meta",		peer_meta.to_json()		},
				{ "approved",		approved				}
			};
			return result.dump();
		}


		// static saved_session get_saved_session_from_file() {
		//	using json = nlohmann::ordered_json;

		//	auto game_path = std::string(TCHAR_TO_UTF8(*UKismetSystemLibrary::GetProjectDirectory()));
		//	auto target_folder = "\\Plugins\\WalletConnect\\Cache\\file_generate.txt";

		//	std::ifstream infile(game_path + target_folder);
		//	std::string line;
		//	std::getline(infile, line);

		//	auto data = crypto::decrypt_aes_256_cbc_PKCS7(line, saved_key, saved_iv);
		//	auto parse_result = json::parse(data);

		//	auto accounts		= parse_result["accounts"].get<std::vector<std::string>>();
		//	auto key_byte		= parse_result["key_byte"].get<std::vector<std::uint8_t>>();
		//	auto key_hex		= parse_result["key_hex"].get<std::string>();
		//	auto bridge_url		= parse_result["bridge_url"].get<std::string>();
		//	auto peer_id		= parse_result["peer_id"].get<std::string>();
		//	auto client_id		= parse_result["client_id"].get<std::string>();
		//	auto chain_id		= parse_result["chain_id"].get<std::uint32_t>();
		//	auto network_id		= parse_result["network_id"].get<std::uint32_t>();
		//	auto handshake_id	= parse_result["handshake_id"].get<std::string>();
		//	auto d_app_meta		= models::client_meta(parse_result["d_app_meta"]);
		//	auto peer_meta		= models::client_meta(parse_result["peer_meta"]);
		//	auto approved		= parse_result["approved"].get<std::boolean>();

		//	return saved_session(accounts, key_byte, key_hex, bridge_url, peer_id, client_id, chain_id,
		//		network_id, handshake_id, d_app_meta, peer_meta, approved);
		//}
	private:

		const std::vector<std::string>	accounts;
		const std::vector<std::uint8_t>	key_byte;
		const std::string				key_hex;
		const std::string				bridge_url;
		const std::string				peer_id;
		const std::string				client_id;
		const std::uint32_t				chain_id;
		const std::uint32_t				network_id;
		const std::string				handshake_id;
		const models::client_meta		d_app_meta;
		const models::client_meta		peer_meta;
		const std::boolean				approved;

	};

	
	
}

