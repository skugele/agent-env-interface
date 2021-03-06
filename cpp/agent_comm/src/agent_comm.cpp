#include "agent_comm.h"

void AgentComm::_register_methods()
{
	// Register methods
	register_method("send", &AgentComm::send);
	register_method("bind", &AgentComm::bind);

	// Register properties
	//register_property<AgentComm, string>("endpoint", &AgentComm::endpoint, "");
}

void AgentComm::_init()
{
	std::cout << "initializing AgentComm class" << endl;
}

bool AgentComm::bind() {
	context = new zmq::context_t(1);
	pub = new zmq::socket_t(*context, ZMQ_PUB);

	bool error = false;
	try 
	{
		std::cout << "Opening connection to " << endpoint << "..." << endl;
		pub->bind(endpoint);
		std::cout << "Connection established!";
	}
	catch (std::exception& e) 
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
		error = true;
	}
	return error;
}

void AgentComm::send(godot::Variant v)
{
	static int method_invokes = 0;

	stringstream ss;
	_convert_variant_to_bytes(v, ss);

	std::cout << "content to serialize: " << ss.str() << std::endl;

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

void AgentComm::_convert_variant_to_bytes(godot::Variant& v, stringstream& ss)
{
	switch (v.get_type())
	{
		case Variant::ARRAY:
		{
			_convert_array_variant_to_bytes(v, ss);
			break;
		}
		case Variant::DICTIONARY:
		{
			_convert_dict_variant_to_bytes(v, ss);
			break;
		}
		default:
		{
			_convert_primitive_variant_to_bytes(v, ss);
			break;
		}
	}
}

void AgentComm::_convert_primitive_variant_to_bytes(godot::Variant& v, stringstream& ss)
{
	switch (v.get_type())
	{
		case Variant::NIL:
		{
			ss << "null";
			break;
		}
		case Variant::BOOL: 
		{
			ss << bool(v);
			break;
		}
		case Variant::INT:
		{
			ss << int64_t(v);
			break;
		}
		case Variant::REAL:
		{
			ss << float(v);
			break;
		}
		case Variant::STRING:
		{
			ss << String(v).utf8().get_data();
			break;
		}
		default:
		{
			std::cerr << "Unsupported type: " << v.get_type() << std::endl;
			break;
		}
	}
}

void AgentComm::_convert_array_variant_to_bytes(godot::Variant& v, stringstream& ss)
{
	Array a = Array(v);
	for (int ndx = 0; ndx < a.size(); ndx++) {
		Variant element = a[ndx];
		_convert_primitive_variant_to_bytes(element, ss);
	}
}

void AgentComm::_convert_dict_variant_to_bytes(godot::Variant& v, stringstream& ss)
{


}

//int AgentComm::recv()
//{
//
//}
