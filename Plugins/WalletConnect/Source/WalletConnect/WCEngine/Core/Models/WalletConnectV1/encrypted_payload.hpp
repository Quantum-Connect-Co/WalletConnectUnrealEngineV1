
namespace models {
	class encrypted_payload : public model_interface
	{
		using json = nlohmann::ordered_json;

	public:

		explicit encrypted_payload(const std::string& _iv,const std::string& _hmac,
			const std::string& _data) noexcept : iv(_iv), hmac(_hmac), data(_data) {}
			
		encrypted_payload() = delete;
		encrypted_payload(encrypted_payload& other) = delete;
		encrypted_payload(encrypted_payload&& other) = delete;

		encrypted_payload& operator= (encrypted_payload& other) = delete;

	public:

		virtual const json to_json() const noexcept override {
			return json
			{
				{ "iv" ,  iv.c_str() },
				{ "hmac", hmac.c_str() },
				{ "data", data.c_str() }
			};
		}

	public:
		 const std::string& get_iv() const noexcept { return iv; }
		 const std::string& get_hmac() const noexcept { return hmac; }
		 const std::string& get_data() const noexcept { return data; }

	private:

		const std::string iv;
		const std::string hmac;
		const std::string data;

	};
}

