
namespace network {

	 static const char get_random_bridge() noexcept {
		std::string alpha_numeric = "abcdefghijklmnopqrstuvwxyz0123456789";
		return alpha_numeric[utils::generate_random_integer(0, alpha_numeric.length() - 1)];
	}

	 static const std::string get_https_url(char wc_bridge) noexcept {
		std::string local_https = "https://";
		std::string local_url = ".bridge.walletconnect.org";
		return std::string(local_https + wc_bridge + local_url).c_str();
	}

	 static const std::string get_websocket_url(char wc_bridge) noexcept {
		std::string local_wss = "wss://";
		std::string local_url = ".bridge.walletconnect.org";
		return std::string(local_wss + wc_bridge + local_url).c_str();
	}

	 static const std::string url_encode(const std::string& value) noexcept
	{
		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::hex;

		for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
			std::string::value_type c = (*i);

			// Keep alphanumeric and other accepted characters intact
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			// Any other characters are percent-encoded
			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int((unsigned char)c);
			escaped << std::nouppercase;
		}
		return escaped.str();
	}
	
	 static const std::string get_qr_code_uri (
		const std::string& handshake_topic,
		const std::string& https_url,
		const std::string& key_hex ) noexcept {
	
		auto versionEncode = url_encode("1");
		auto bridgeUrlEncode = url_encode(https_url);
		auto keyEncoded = url_encode(key_hex);

		auto part1 = std::string("wc:" + handshake_topic);

		auto part2 = std::string("@" + versionEncode + "?bridge=" + bridgeUrlEncode + "&key=" + keyEncoded);

		return std::string(part1 + part2);
	}

	 static const std::string generate_guid() noexcept {
		return std::string(TCHAR_TO_UTF8(*FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)));
	};
}