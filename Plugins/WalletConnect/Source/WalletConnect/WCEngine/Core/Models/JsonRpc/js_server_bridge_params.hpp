
namespace params {

	class js_server_bridge_request_params : public params_interface {

		using json = nlohmann::ordered_json;
		using params = js_server_bridge_request_params;

	public:
		explicit params(std::string& _uuid, std::string _bridge) 
			noexcept : uuid(_uuid), bridge(_bridge) {}

	public:
		 virtual const json get_params() const noexcept override {
			return json
			{
				{ "uuid",    uuid   },
				{ "bridge",  bridge }
			};
		}

	private:
		const std::string uuid;
		const std::string bridge;
	};
}