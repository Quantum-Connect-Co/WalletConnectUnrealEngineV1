
namespace models {

	class json_rpc_request : public model_interface
	{
		using json = nlohmann::ordered_json;
		using params = params_interface;

	public:

		explicit json_rpc_request(const params* _params_interface_ptr, const std::string& _method)
			noexcept : params_ptr(_params_interface_ptr), method(_method) {}

		explicit json_rpc_request(const params* _params_interface_ptr, const std::string& _method, const std::string& _id)
			noexcept : params_ptr(_params_interface_ptr), method(_method), id(_id) {}
			
		json_rpc_request() = delete;
		json_rpc_request(json_rpc_request& other) = delete;
		json_rpc_request(json_rpc_request&& other) = delete;

		json_rpc_request& operator= (json_rpc_request& other) = delete;

	public:

		 virtual const json to_json() const noexcept override {
			assert(peer_meta != nullptr);
			return id.empty() ?
			json
			{
				{ "params", { params_ptr->get_params()}},
				{ "id", int64(FDateTime::UtcNow().ToUnixTimestamp() * 1000000 + utils::generate_random_integer(111111, 999999)) },
				{ "jsonrpc", "2.0" },
				{ "method", method}
			}:
			json
			{
				{ "params", { params_ptr->get_params()}},
				{ "id", this->id },
				{ "jsonrpc", "2.0" },
				{ "method", method}
			};
		}

	public:

		 const json get_eth_trans() const noexcept {
			return json
			{
				{ "id", int64(FDateTime::UtcNow().ToUnixTimestamp() * 1000000 + utils::generate_random_integer(111111, 999999)) },
				{ "jsonrpc", "2.0" },
				{ "method", method },
				{ "params", params_ptr->get_params() }
			};
		}

		 const params* get_params() const noexcept { return params_ptr; }

	private:

		const params* params_ptr;
		const std::string method;
		const std::string id;
	};
}


namespace params {

	class universal_params : public params_interface
	{
		using json = nlohmann::ordered_json;

	public:
		explicit universal_params(const json& _params) : params(_params) {}

	public:
		virtual const json get_params() const noexcept {
			return params;
		}
	private:
		json params;
	};
}
