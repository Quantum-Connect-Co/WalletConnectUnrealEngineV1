
class model_interface
{
	using json = nlohmann::ordered_json;

public:
	model_interface() = default;
	model_interface(model_interface& other) = delete;
	model_interface& operator=(model_interface& other) = delete;

	virtual const json to_json() const noexcept = 0;

	virtual ~model_interface() = default;
};