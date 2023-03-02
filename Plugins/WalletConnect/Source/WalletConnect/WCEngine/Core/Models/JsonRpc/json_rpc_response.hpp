
namespace models {
	class json_rpc_response : public model_interface {
		using json = nlohmann::ordered_json;

	public:
		explicit json_rpc_response(json& _json) noexcept : id(_json["id"].get<std::uint64_t>()), result(_json) {}
		
	public:
		 virtual const json to_json() const noexcept override {
			return json
			{
				{ "id", int64(FDateTime::UtcNow().ToUnixTimestamp() * 1000000 + utils::generate_random_integer(111111,999999)) },
				{ "jsonrpc", "2.0" },
				{ "result", result }
			};
		}

	public:
		
		const std::uint64_t id;
		const json result;
	};
}