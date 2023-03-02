
namespace models {
	class network_message : public model_interface, public params_interface
	{
		using json = nlohmann::ordered_json;

	public:

		enum class etype { sub, pub, ack };
			
		NLOHMANN_JSON_SERIALIZE_ENUM(etype, 
		{
			{etype::sub, "sub"},
			{etype::pub, "pub"},
			{etype::ack, "ack"},
		})

		explicit network_message(const std::string& _topic, const network_message::etype _type, const std::string& _payload,
			const bool _silent) noexcept : topic(_topic), type(_type), payload(_payload), silent(_silent) {}

		explicit network_message(const json& _json) noexcept :

			topic   (_json["topic"  ].empty() ? std::string() : _json["topic"  ].get<std::string>()),
			type    (_json["type"   ].empty() ? etype::sub    : _json["type"   ].get<etype>()),
			payload (_json["payload"].empty() ? std::string() : _json["payload"].get<std::string>()),
			silent  (_json["silent" ].empty() ? false         : _json["silent" ].get<bool>()) {}
		
		network_message() = delete;
		network_message(network_message& other) = delete;
		network_message(network_message&& other) = delete;

		network_message& operator= (network_message& other) = delete;

	public:

		virtual const json to_json() const noexcept override {
			return json
			{
				{ "topic", topic.c_str() },
				{ "type", get_type().c_str() },
				{ "payload", payload.c_str() },
				{ "silent", silent }
			};
		}

		 virtual const json get_params() const noexcept override {
			return to_json();
		}

	public:

		const std::string get_type() const noexcept{
			switch (type)
			{
				case etype::sub: return "sub";
				case etype::pub: return "pub";
				case etype::ack: return "ack";
			}
			return std::string();
		}

		const std::string& get_topic()   const noexcept { return topic;   }
		const std::string& get_payload() const noexcept { return payload; }
		const bool&        get_silent()  const noexcept { return silent;  }

	private:

		const std::string topic;
		const network_message::etype type;
		const std::string payload;
		const bool silent;
	};
}

