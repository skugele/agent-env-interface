#include "godot_util.h"

using json = nlohmann::json;

void marshal_basic_variant(const godot::Variant& value, nlohmann::json& marshaler) {

	switch (value.get_type()) {
	case godot::Variant::NIL:
		marshaler = convert_nil(value);
		break;
	case godot::Variant::BOOL:
		marshaler = convert_bool(value);
		break;
	case godot::Variant::INT:
		marshaler = convert_int(value);
		break;
	case godot::Variant::REAL:
		marshaler = convert_real(value);
		break;
	case godot::Variant::STRING:
		marshaler = convert_string(value);
		break;
	default:
		std::cerr << "Ignoring unsupported variant type" << std::endl;
		break;
	}
}

// TODO: Might be able to combine this and previous method by passing a pointer to a function that does the update,
// in the first case it would be an assignment. In the 2nd case it would be a push_back. The signature of the 
// function would be func(json element, T value)
void marshal_basic_variant_in_array(const godot::Variant& value, nlohmann::json& marshaler) {

	switch (value.get_type()) {
	case godot::Variant::NIL:
		marshaler.push_back(convert_nil(value));
		break;
	case godot::Variant::BOOL:
		marshaler.push_back(convert_bool(value));
		break;
	case godot::Variant::INT:
		marshaler.push_back(convert_int(value));
		break;
	case godot::Variant::REAL:
		marshaler.push_back(convert_real(value));
		break;
	case godot::Variant::STRING:
		marshaler.push_back(convert_string(value));
		break;
	default:
		std::cerr << "Ignoring unsupported variant type" << std::endl;
		break;
	}
}

void marshal_array_variant(const godot::Array& array, nlohmann::json& marshaler) {
	for (int i = 0; i < array.size(); i++) {
		godot::Variant value = array[i];

		if (is_basic_variant(value)) {
			marshal_basic_variant_in_array(value, marshaler);
		}
		else if (is_array_variant(value)) {
			// adds empty array
			marshaler.push_back(json::array());

			// recusrive call
			marshal_array_variant(value, marshaler[marshaler.size() - 1]);
		}
		else if (is_dictionary_variant(value)) {
			// adds empty dictionary
			marshaler.push_back(json({}));
			
			marshal_dictionary_variant(value, marshaler[marshaler.size() - 1]);
		}
	}
}

void marshal_dictionary_variant(const godot::Dictionary& dict, nlohmann::json& marshaler) {

	godot::Array keys = dict.keys();

	for (int i = 0; i < keys.size(); i++)
	{
		godot::Variant key = keys[i];
		godot::Variant value = dict[key];

		json& element = marshaler[convert_string(key)];

		if (is_basic_variant(value)) {
			marshal_basic_variant(value, element);
		}
		else if (is_array_variant(value)) {
			marshal_array_variant(value, element);
		}
		else if (is_dictionary_variant(value)) {
			// recursive call
			marshal_dictionary_variant(value, element);
		}
	}
}

void marshal_variant(const godot::Variant& value, nlohmann::json& marshaler) {

	switch (value.get_type()) {
	case godot::Variant::DICTIONARY:
	{
		marshal_dictionary_variant(value, marshaler);
		break;
	}
	case godot::Variant::ARRAY:
	{
		marshal_array_variant(value, marshaler);
		break;
	}
	case godot::Variant::NIL:
	case godot::Variant::BOOL:
	case godot::Variant::INT:
	case godot::Variant::REAL:
	case godot::Variant::STRING:
	{
		marshal_basic_variant(value, marshaler);
		break;
	}
	default:
		break;
	}
}



