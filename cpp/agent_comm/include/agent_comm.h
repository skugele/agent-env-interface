#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <sstream>
#include <zmq.hpp>
#include "Array.hpp"
#include "String.hpp"

#include "godot_util.h"

class AgentComm : public godot::Node {
	GODOT_CLASS(AgentComm, Node);

private:

	// TODO: Can we do this with the protocol as an enumerated list in a property and the port a bounded integer value???
	std::string endpoint = "tcp://*:9001";

	zmq::context_t* context;
	zmq::socket_t* pub;

	//template <typename T>
	//void _convert_variant_array(godot::Array&, std::vector<T>&);
	//std::map <const std::string, const std::string> construct_message_header();

public:

	// GDNative required methods
	static void _register_methods();
	void _init();

	// GDNative exposed methods
	bool bind();
	void send(godot::Variant);
};