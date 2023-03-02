namespace utils {

	using json = nlohmann::ordered_json;

	template<typename Type>
	 static const Type safety_get_from_json(const std::string& msg, const std::string& key) {
		return json::parse(msg)[key].empty() ? Type() : json::parse(msg)[key].get<Type>();
	}

	template<typename Type>
	 static const Type safety_get_from_json(const std::string& msg, const std::string& key1, const std::string& key2) {
		return json::parse(msg)[key1][key2].empty() ? Type() : json::parse(msg)[key1][key2].get<Type>();
	}

	 static const int generate_random_integer(int min, int max) noexcept {
		std::random_device rd;
		
		// Generates pseudo-random numbers, seeded by random_device
		std::default_random_engine generator{ rd() };
		
		// Transforms output of generator into desired range
		// In this case, between 1 and 10, inclusive
		std::uniform_int_distribution<int> dist{ min, max};

		return dist(generator);
	}

	 static const std::string convert_string_to_hex(const std::string& input) noexcept {
		static const char hex_digits[] = "0123456789ABCDEF";
		std::string output;
		output.reserve(input.length() * 2);//
		for (unsigned char c : input)
		{
			output.push_back(hex_digits[c >> 4]);
			output.push_back(hex_digits[c & 15]);
		}
		transform(output.begin(), output.end(), output.begin(), ::tolower);
		return output;
	}

	 static const std::string convert_key_to_hex(const std::vector<uint8_t>& key) noexcept {
		return convert_string_to_hex(std::string(key.begin(), key.end()));
	}

	 static const std::vector<uint8_t> convert_hex_string_to_vector_uchar(const std::string& data) noexcept {
		std::vector<uint8_t> payload;
		payload.resize(data.size() / 2);
		auto hex2uchar{ [](char data) -> unsigned char {
			if (data >= '0' && data <= '9') return data - '0';
			else if (data >= 'a' && data <= 'f') return 10 + data - 'a';
			else return 10 + data - 'A';} };

		for (int i = 0; i < data.size(); i += 2)
			payload[i / 2] = hex2uchar(data[i]) * 16 + hex2uchar(data[i + 1]);

		return payload;
	}

	 static const std::string uint64_to_hex(const std::uint64_t& i) noexcept {
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0')
			<< std::setw(sizeof(int) * 2)
			<< std::hex << i;

		return stream.str();
	}

	 static const std::string uint64_to_string(const std::uint64_t& value) noexcept {
		std::ostringstream os;
		os << value;
		return os.str();
	}

	 static const int64_t string_to_uint64(const std::string& str) noexcept {
		uint64_t value;
		std::istringstream iss(str.c_str());
		iss >> value;
		return value;
	}
}