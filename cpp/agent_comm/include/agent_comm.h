#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <sstream>
#include <zmq.hpp>
#include "Array.hpp"
#include "String.hpp"
#include <stdexcept>
#include <stdio.h>

#include "godot_util.h"



class AgentComm : public godot::Node {
	GODOT_CLASS(AgentComm, Node);

private:

	static const int PORT_MIN;
	static const int PORT_MAX;
	static const int PORT_DEFAULT;

	static const char* AGENT_MSG_HEADER;
	static const char* TOPIC_MSG_HEADER;
	static const char* SEQNO_MSG_HEADER;

	static const char* MSG_HEADER_ELEMENT;
	static const char* MSG_DATA_ELEMENT;

	static const char* PROTOCOL_DEFAULT;

	// client requestable connection options (these constants must be initialized at runtime)
	const godot::Variant* AGENT_OPTION;
	const godot::Variant* PORT_OPTION;
	const godot::Variant* PROTOCOL_OPTION;

	// zmq context used for all connections
	zmq::context_t* zmq_context;

	typedef unsigned int context_id_t;
	struct ConnectContext {
		context_id_t id;
		std::string agent;
		zmq::socket_t* connection;
		int type;
		int port;
		uint64_t seq_no;
	};

	std::map<context_id_t, ConnectContext*> connection_registry;

	void _construct_endpoint(const godot::Dictionary&, std::string&);
	void _construct_message_header(ConnectContext*, nlohmann::json&);
	void construct_message(zmq::message_t& msg, const std::string& topic, const std::string& payload);
	size_t _get_message_length(std::string& topic, std::string& msg);
	std::string serialize(const godot::Variant payload, ConnectContext* context);
	ConnectContext* lookup_context(const godot::Variant& id);
	ConnectContext* _register_context(const godot::Dictionary&, int type);
	AgentComm::context_id_t _get_unique_id();

public:

	AgentComm();
	~AgentComm();

	// GDNative required methods
	static void _register_methods();
	void _init();

	// GDNative exposed methods
	int connect(godot::Variant options);
	void send(const godot::Variant v, const godot::Variant context, const godot::Variant topic);

};