#include "godot_util.h"

using json = nlohmann::json;

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
			godot::Array value_array(value);
			for (int i = 0; i < value_array.size(); i++) {
				marshal_basic_variant_in_array(value_array[i], element);
			}
		}
		else if (is_dictionary_variant(value)) {
			// recursive call
			marshal_dictionary_variant(value, element);
		}
	}
}