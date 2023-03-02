
namespace params {

	class wc_session_request_params : public params_interface
	{
		using json = nlohmann::ordered_json;
		using params = wc_session_request_params;
		using client_meta = models::client_meta;

	public:

		explicit wc_session_request_params(const std::string& _peer_id, const client_meta* _client_meta)
			 noexcept : peer_id(_peer_id), peer_meta(_client_meta) {}
			
		wc_session_request_params() = delete;
		wc_session_request_params(wc_session_request_params& other) = delete;
		wc_session_request_params(wc_session_request_params&& other) = delete;

		params& operator= (params& other) = delete;

	public:

		 virtual const json get_params() const noexcept override {
			return json
			{
				{ "peerId", peer_id.c_str() },
				{ "peerMeta", peer_meta->to_json() },
				{ "chainId", 1 }
			};
		}

	public:

		const std::string& get_peer_id() const noexcept { return peer_id; }
		const client_meta* get_peer_meta() const noexcept { return peer_meta; }
		const int get_chain_id() { return 1; }

	private:
		const std::string peer_id;
		const client_meta* peer_meta;
	};
}