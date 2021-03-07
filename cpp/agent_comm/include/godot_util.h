#pragma once

#include <Godot.hpp>
#include <string.h>
#include <nlohmann/json.hpp>

inline std::string convert_string(const godot::Variant& v) {
	return godot::String(v).utf8().get_data();
}

inline int64_t convert_int(const godot::Variant& v) {
	return int64_t(v);
}

inline double convert_real(const godot::Variant& v) {
	return double(v);
}

inline bool convert_bool(const godot::Variant& v) {
	return bool(v);
}

inline std::nullptr_t convert_nil(const godot::Variant& v) {
	return nullptr;
}

inline bool is_basic_variant(const godot::Variant& v) {
	switch (v.get_type()) {
		case godot::Variant::NIL:
		case godot::Variant::BOOL:
		case godot::Variant::INT:
		case godot::Variant::REAL:
		case godot::Variant::STRING:
			return true;
			break;
		default:
			return false;
	}
}

inline bool is_array_variant(const godot::Variant& v) {
	return v.get_type() == godot::Variant::ARRAY;
}

inline void marshal_basic_variant(const godot::Variant& value, nlohmann::json& marshaler) {

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
inline void marshal_basic_variant_in_array(const godot::Variant& value, nlohmann::json& marshaler) {

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