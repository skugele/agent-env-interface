#include "agent_comm.h"

void AgentComm::_register_methods()
{
	// Register methods
	register_method("send", &AgentComm::send);
	//register_method("recv", &AgentComm::recv);
	register_method("bind", &AgentComm::bind);

	// Register properties
	//register_property<AgentComm, string>("endpoint", &AgentComm::endpoint, "");
}

void AgentComm::_init()
{
	std::cout << "initializing AgentComm class" << endl;
}

//void AgentComm::_init_zeromq()
//{
//	std::cout << "initializing zeromq" << endl;
//
//
//
//}

bool AgentComm::bind() {
	context = new zmq::context_t(1);
	pub = new zmq::socket_t(*context, ZMQ_PUB);

	bool error = false;
	try {
		std::cout << "Opening connection to " << endpoint << "..." << endl;
		pub->bind(endpoint);
		std::cout << "Connection established!";
	}
	catch (std::exception& e) {
		std::cerr << "Caught exception: " << e.what() << std::endl;
		error = true;
	}
	return error;
}

void AgentComm::_convert_primitive_variant_to_bytes(godot::Variant v)
{
	stringstream ss;
	switch (v.get_type())
	{
	case Variant::NIL:
		std::cout << "NIL" << endl;
		break;
	case Variant::BOOL: {
		std::cout << "BOOL" << endl;
		ss << bool(v);
		break;
	}
	case Variant::INT:
		std::cout << "INT" << endl;
		ss << int64_t(v);
		break;
	case Variant::REAL:
		std::cout << "REAL" << endl;
		ss << float(v);
		break;
	case Variant::STRING:
	{
		std::cout << "STRING" << endl;
		ss << String(v).utf8().get_data();

		break;
	}
	default:
		std::cout << "Unsupported type: " << v.get_type() << std::endl;
		break;
	}
	std::cout << "value is: " << ss.str() << std::endl;
}

void AgentComm::send(godot::Variant v)
{
	static int method_invokes = 0;

	std::cout << "data type of variant is: ";
	switch (v.get_type())
	{
	case Variant::ARRAY:
	{
		std::cout << "ARRAY" << endl;

		Array a = Array(v);
		std::cout << "godot array length: " << a.size() << std::endl;
		for (int ndx = 0; ndx < a.size(); ndx++) {
			Variant v1 = a[ndx];
			_convert_primitive_variant_to_bytes(v1);
		}

		break;		
	}
	case Variant::DICTIONARY:
		std::cout << "DICTIONARY" << endl;
		break;

	default:
		_convert_primitive_variant_to_bytes(v);
		break;
	}
	std::cout << std::endl;

	/*
	godot_bool GDAPI godot_variant_as_bool(const godot_variant * p_self);
	uint64_t GDAPI godot_variant_as_uint(const godot_variant * p_self);
	int64_t GDAPI godot_variant_as_int(const godot_variant * p_self);
	double GDAPI godot_variant_as_real(const godot_variant * p_self);
	godot_string GDAPI godot_variant_as_string(const godot_variant * p_self);
	godot_dictionary GDAPI godot_variant_as_dictionary(const godot_variant * p_self);
	godot_array GDAPI godot_variant_as_array(const godot_variant * p_self);

	godot_int GDAPI godot_array_size(const godot_array * p_self);
	*/

	//zmq::message_t message(20);
	//snprintf((char*)message.data(), 20, "%d", i);
	//
	//method_invokes++;

	//cout << "Message content: " << i << endl;
	//cout << "Method invocation #: " << method_invokes << endl;

	//char buffer[21] = {0};
	//memcpy(buffer, message.data(), 20);
	//cout << "sending message: " << buffer << endl;

	//bool val = pub->send(message);
	//cout << "Return val: " << val << endl;
}

//int AgentComm::recv()
//{
//
//}
