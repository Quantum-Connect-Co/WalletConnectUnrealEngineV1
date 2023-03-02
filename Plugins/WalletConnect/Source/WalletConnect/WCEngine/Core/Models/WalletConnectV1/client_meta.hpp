
namespace models {

	class client_meta : public model_interface
	{
		using json = nlohmann::ordered_json;

	public:
		
		explicit client_meta(const std::string&	_description, const std::string& _url,
			const std::vector<std::string>& _icons, const std::string& _name) noexcept : 
				description	(std::move(_description)),
				url			(std::move(_url)		),
				icons		(std::move(_icons)		),
				name		(std::move(_name)		) {}
		
		explicit client_meta(const json& _json) noexcept :
			description	(_json["description"].empty() ? std::string()				: _json["description"	].get<std::string>()),
			url			(_json["url"		].empty() ? std::string()				: _json["url"			].get<std::string>()),
			icons		(_json["icons"		].empty() ? std::vector<std::string>()	: _json["icons"			].get<std::vector<std::string>>()),
			name		(_json["name"		].empty() ? std::string()				: _json["name"		].get<std::string>()) {}


		client_meta(const client_meta& other) noexcept :
			description	(other.description	),
			url			(other.url			),
			icons		(other.icons		),
			name		(other.name			) {}


		client_meta(const client_meta&& other) noexcept : 
			description	(std::move(other.description)	),
			url			(std::move(other.url)			),
			icons		(std::move(other.icons)			),
			name		(std::move(other.name)			) {}

	private:

		client_meta() = delete;
		client_meta& operator= (client_meta& other) = delete;

	public:

		virtual const json to_json() const noexcept override{
			return json
			{
				{"description", description.c_str() },
				{"url",			url.c_str() },
				{"icons", { icons[0].c_str() } },
				{"name", name.c_str()}
			};
		}


	public:
		const std::string&				get_url()			const noexcept { return url;			}
		const std::string&				get_description()	const noexcept { return description;	}
		const std::string&				get_name()			const noexcept { return name;			}
		const std::vector<std::string>& get_icons()			const noexcept { return icons;			}
		
	private:
		const std::string description;
		const std::string url;
		const std::vector<std::string> icons;
		const std::string name;
	};
}

