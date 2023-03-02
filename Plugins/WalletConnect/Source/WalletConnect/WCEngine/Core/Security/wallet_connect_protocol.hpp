
namespace std 
{
	typedef bool boolean;
	typedef int  integer;
}

namespace security 
{
	struct protocol_storage {
		std::string js_bridge_uuid;
		std::string client_id;
		std::string hanshake_topic;

		std::vector<std::uint8_t> key;
		std::vector<std::uint8_t> iv;
		char wc_bridge;
	};

	class wallet_session {

		using json = nlohmann::ordered_json;

	public:

		explicit wallet_session(const std::vector<std::string>& _accounts, const std::boolean _approved, const std::uint32_t _chain_id,
			const std::string& _peer_id, const models::client_meta& _peer_meta) noexcept : accounts(_accounts), approved(_approved),
			chain_id(_chain_id), peer_id(_peer_id), peer_meta(_peer_meta) {}

		explicit wallet_session(const json& result) noexcept :
			accounts(result["accounts"].empty() ? std::vector<std::string>() : result["accounts"].get<std::vector<std::string>>()),
			approved(result["approved"].empty() ? false                      : result["approved"].get<bool>()),
			chain_id(result["chainId" ].empty() ? 0                          : result["chainId" ].get<std::uint32_t>()),
			peer_id (result["peerId"  ].empty() ? std::string()              : result["peerId"  ].get<std::string>()),

			peer_meta(models::client_meta(
				result["peerMeta"]["description"].empty() ? std::string()              : result["peerMeta"]["description"].get<std::string>(),
				result["peerMeta"]["url"        ].empty() ? std::string()              : result["peerMeta"]["url"        ].get<std::string>(),
				result["peerMeta"]["icons"      ].empty() ? std::vector<std::string>() : result["peerMeta"]["icons"      ].get<std::vector<std::string>>(),
				result["peerMeta"]["name"       ].empty() ? std::string()              : result["peerMeta"]["name"       ].get<std::string>()
			)) {}
		
		 const bool save_session_data() {
			std::ofstream outfile("test.txt");

			outfile << "my text here!" << std::endl;

			outfile.close();
			return true;
		}

	public:
		 const std::vector<std::string>& get_accounts()  const { return accounts;  }
		 const std::boolean&             get_approved()  const { return approved;  }
		 const std::uint32_t&            get_chain_id()  const { return chain_id;  }
		 const std::string&              get_peer_id()   const { return peer_id;   }
		 const models::client_meta&      get_peer_meta() const { return peer_meta; }


	private:
		const std::vector<std::string> accounts;
		const std::boolean             approved;
		const std::uint32_t            chain_id;
		const std::string              peer_id;
		const models::client_meta      peer_meta;
	};
}

