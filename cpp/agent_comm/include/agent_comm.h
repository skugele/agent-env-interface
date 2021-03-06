#pragma once

#include "share.h"
#include <Node.hpp>
#include <sstream>
#include <zmq.hpp>
#include "Array.hpp"
#include "String.hpp"
#include <nlohmann/json.hpp>

class AgentComm : public Node {
	GODOT_CLASS(AgentComm, Node);

private:
	std::string endpoint = "tcp://*:9001";

	zmq::context_t* context;
	zmq::socket_t* pub;

	void _convert_variant_to_bytes(godot::Variant&, stringstream&);
	void _convert_primitive_variant_to_bytes(godot::Variant&, stringstream&);
	void _convert_array_variant_to_bytes(godot::Variant&, stringstream&);
	void _convert_dict_variant_to_bytes(godot::Variant&, stringstream&);

public:

	// GDNative required methods
	static void _register_methods();
	void _init();

	// GDNative exposed methods
	bool bind();
	void send(godot::Variant);
};