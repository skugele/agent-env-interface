#include "agent_comm.h"

using namespace std;
using json = nlohmann::json;

const int AgentComm::PORT_MIN = 1024;
const int AgentComm::PORT_MAX = 65536;
const int AgentComm::PORT_DEFAULT = 9001;

const char* AgentComm::PROTOCOL_DEFAULT = "tcp";

const char* AgentComm::AGENT_MSG_HEADER = "agent";
const char* AgentComm::TOPIC_MSG_HEADER = "topic";
const char* AgentComm::SEQNO_MSG_HEADER = "seqno";

const char* AgentComm::MSG_HEADER_ELEMENT = "header";
const char* AgentComm::MSG_DATA_ELEMENT = "data";

AgentComm::AgentComm() : connection_registry() {

	// Godot's Variants are dependent on the Godot runtime, so these constants must be created dynamically (at runtime)
	AgentComm::PORT_OPTION = new godot::Variant("port");
	AgentComm::PROTOCOL_OPTION = new godot::Variant("protocol");
	AgentComm::AGENT_OPTION = new godot::Variant("agent");

	zmq_context = new zmq::context_t(1);
}

AgentComm::~AgentComm() {

	// allocated in constructor
	delete PORT_OPTION;
	delete PROTOCOL_OPTION;
	delete AGENT_OPTION;
}


void AgentComm::_register_methods() {
	// Register methods
	godot::register_method("send", &AgentComm::send);
	godot::register_method("connect", &AgentComm::connect);
}

void AgentComm::_init() {
	cout << "initializing AgentComm class" << endl;
}

AgentComm::context_id_t AgentComm::_get_unique_id() {
	static context_id_t next_id = 0;
	return ++next_id;
}

AgentComm::ConnectContext* AgentComm::_register_context(const godot::Dictionary& options, int type) {
	AgentComm::ConnectContext* c = new AgentComm::ConnectContext();

	c->id = _get_unique_id();
	c->connection = new zmq::socket_t(*zmq_context, type);
	c->type = type;
	c->seq_no = 0;

	if (options.has(*AGENT_OPTION)) {
		c->agent = convert_string(options[*AGENT_OPTION]);
	}
	else {
		c->agent = "default"; // TODO: externalize this to a constant
	}

	connection_registry[c->id] = c;

	return c;
}

int AgentComm::connect(godot::Variant options) {

	// TODO: Is there a better way to handle errors in Godot???
	if (options.get_type() != godot::Variant::DICTIONARY) {
		std::cerr << "Invalid bind options. Variant must be a dictionary." << std::endl;
		return -1; // TODO: Externalize to a constant
	}

	AgentComm::ConnectContext* context = _register_context(options, ZMQ_PUB);

	try
	{
		std::string endpoint;
		_construct_endpoint(options, endpoint);

		cout << "Opening connection to " << endpoint << "..." << endl;
		context->connection->bind(endpoint);
		cout << "Connection established!";
	}
	catch (exception& e)
	{
		cout << "Caught exception: " << e.what() << endl;
		return -1; // TODO: Externalize to a constant
	}

	return context->id;
}

void AgentComm::_construct_endpoint(const godot::Dictionary& options, std::string& endpoint) {

	std::string protocol = PROTOCOL_DEFAULT;
	int port = PORT_DEFAULT;

	if (options.has(*PROTOCOL_OPTION)) {
		std::string requested_protocol = convert_string(options[*PROTOCOL_OPTION]);

		// TODO: Change this to use a protocol accept list
		if (requested_protocol != "tcp") {
			std::cerr << "unsupported protocol. protocol currently limited to \"tcp\"" << std::endl;
		}
		else {
			std::cerr << "using protocol " << requested_protocol << std::endl;
			protocol = requested_protocol;
		}
	}

	if (options.has(*PORT_OPTION)) {
		int requested_port = (int)convert_int(options[*PORT_OPTION]);

		if (requested_port < PORT_MIN || requested_port > PORT_MAX) {
			std::cerr << "invalid reequested port assignment. port must be within range "
				<< "[" << PORT_MIN << "," << PORT_MAX << "]." << std::endl;
		}
		else {
			std::cout << "using port " << requested_port << std::endl;
			port = requested_port;
		}
	}

	stringstream ss;
	ss << protocol << "://*:" << port;

	endpoint = ss.str();
}

void AgentComm::_construct_message_header(AgentComm::ConnectContext* context, nlohmann::json& marshaler)
{
	marshaler[SEQNO_MSG_HEADER] = ++context->seq_no;
}

void AgentComm::send(const godot::Variant v_content, const godot::Variant v_context_id, const godot::Variant v_topic)
{
	try
	{
		ConnectContext* context = lookup_context(v_context_id);

		std::string topic = convert_string(v_topic);
		std::string content = serialize(v_content, context);

		// construct ZeroMq message
		zmq::message_t message(_get_message_length(topic, content));
		construct_message(message, topic, content);

		context->connection->send(message);
	}
	catch (exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
	}
}

std::string AgentComm::serialize(const godot::Variant payload, ConnectContext* context) {
	json marshaler;

	json& header = marshaler[MSG_HEADER_ELEMENT];
	json& data = marshaler[MSG_DATA_ELEMENT];

	_construct_message_header(context, header);
	marshal_variant(payload, data);

	return marshaler.dump();
}

size_t AgentComm::_get_message_length(std::string& topic, std::string& msg) {
	return topic.length() + msg.length() + 1; // additional character for space between topic and json
}

// Note: the resulting buffer contents must NOT be null terminated or MQ clients written in some languages 
// (e.g., Python) will break
void AgentComm::construct_message(zmq::message_t& msg, const std::string& topic, const std::string& payload) {
	char* p_buffer = (char*)msg.data();

	// add topic to buffer
	memcpy(p_buffer, topic.c_str(), topic.length());

	// add space
	p_buffer[topic.length()] = ' ';

	// add message payload to buffer
	memcpy(p_buffer + topic.length() + 1, payload.c_str(), payload.length());
}

AgentComm::ConnectContext* AgentComm::lookup_context(const godot::Variant& id) {
	ConnectContext* p_found = nullptr;

	try {
		p_found = connection_registry.at((int)convert_int(id));
	}
	catch (std::out_of_range& e) {
		std::cerr << "exception on context lookup: " << e.what() << std::endl;
	}

	return p_found;
}