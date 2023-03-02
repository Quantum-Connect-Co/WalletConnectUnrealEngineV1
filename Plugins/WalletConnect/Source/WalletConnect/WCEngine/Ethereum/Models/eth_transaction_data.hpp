#pragma once

struct access_list
{
	//json property = "address"
	std::string address;
	//json property = "storageKeys"
	std::vector<std::string> storage_keys;
};

namespace eth {
	class eth_transaction_data : public params_interface
	{
		using json = nlohmann::ordered_json;

	public:

		explicit eth_transaction_data(
			const std::string& _from,
			const std::string& _to,
			const std::uint64_t& _value,
			const std::string& _data

		) noexcept :
			from(_from),
			to(_to),
			value(utils::uint64_to_hex(_value)),
			data(_data)
		{}
	public:
		const json get_params() const noexcept override {
			return json
			{
				{ "from",                 from.c_str()  },
				{ "to",	                  to.c_str()    },
				{ "data",                 data.c_str()  },
				{ "gas",                  "0x5248"      },
				{ "gasPrice",             nullptr       },
				{ "value",                value.c_str() },
				{ "nonce",                nullptr       },
				{ "chainId",              1             }
			};
		}


		const json get_json_eth_transaction() const noexcept {

			std::shared_ptr<params::universal_params> eth_trn_params(new params::universal_params(get_params()));

			return json
			{
				{ "id", int64(FDateTime::UtcNow().ToUnixTimestamp() * 1000000 + utils::generate_random_integer(111111, 999999)) },
				{ "jsonrpc", "2.0" },
				{ "method", "eth_sendTransaction"},
				{ "params", { eth_trn_params->get_params()} }
			};
		}


		//"nonce": null,
		//"accessList": null,
		//"from": "0x263d117CD910d01910aBB27a513842655ea58F89",
		//"to": "0x1C00B5048127FBDb594D8ADDC3F444713ecd6D01",
		//"gas": "0x5248",
		//"gasPrice": null,
		//"value": "0x2ce80355f6000",
		//"data": "0xd0e30db0",
		//"maxFeePerGas": null,
		//"maxPriorityFeePerGas": null,
		//"type": null,
		//"chainId": null,

	private:

		static std::uint64_t nonce;

		std::uint64_t chainId;
		std::string from;
		std::string to;
		std::string gas = "0x5248";
		std::string gasPrice;
		std::string value;
		std::string data;
		std::string maxFeePerGas;
		std::string maxPriorityFeePerGas;
		std::string type;
		std::vector<access_list> accessList;
	};

}
