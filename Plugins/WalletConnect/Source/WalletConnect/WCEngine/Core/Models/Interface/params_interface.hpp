

class params_interface
{
	using json = nlohmann::ordered_json;

public:
	params_interface() = default;
	params_interface(params_interface& other) = delete;

	params_interface& operator=(params_interface& other) = delete;

	virtual const json get_params() const noexcept = 0;

	virtual ~params_interface() = default;

};