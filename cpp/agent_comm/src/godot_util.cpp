#include "godot_util.h"

using json = nlohmann::json;

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