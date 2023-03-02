

namespace models {

	enum class method : uint8_t {
		start_session,
		close_session
	};

	NLOHMANN_JSON_SERIALIZE_ENUM(method, {
		{method::start_session, "start_session"},
		{method::close_session, "close_session"},
	})

	class json_server_repeater : public model_interface
	{
		using json = nlohmann::ordered_json;

	public:

		explicit json_server_repeater(const std::string& uuid,const std::string& message, const method method)
			: uuid(uuid), message(message), method(method) {}
		
		explicit json_server_repeater(const std::string& uuid, const json& message, const method method)
			: uuid(uuid), message(message.dump()), method(method) {}

		json_server_repeater() = delete;
		json_server_repeater(json_rpc_request& other) = delete;
		json_server_repeater(json_rpc_request&& other) = delete;

		json_rpc_request& operator= (json_rpc_request& other) = delete;

	public:

		virtual const json to_json() const noexcept override {
			return json
			{
				{ "uuid", uuid },
				{ "method", enums.at(method) },
				{ "message", message}
			};
		}

	public:
		const std::string& get_uuid() const noexcept { return uuid; }
		const std::string& get_message() const noexcept { return message; }
		const method& get_method() const noexcept { return method; }

	private:
		
		const std::string uuid;
		const std::string message;
		const method method;

	private:
		std::unordered_map<models::method, std::string> enums
		{
			{ method::start_session, "start_session"},
			{ method::close_session, "close_session"}
		};
	};
}

