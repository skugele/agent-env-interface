#include "agent_comm.h"

using namespace std;

// for convenience
using json = nlohmann::json;

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
	cout << "initializing AgentComm class" << endl;
}

bool AgentComm::bind() {
	context = new zmq::context_t(1);
	pub = new zmq::socket_t(*context, ZMQ_PUB);

	bool error = false;
	try 
	{
		cout << "Opening connection to " << endpoint << "..." << endl;
		pub->bind(endpoint);
		cout << "Connection established!";
	}
	catch (exception& e) 
	{
		cout << "Caught exception: " << e.what() << endl;
		error = true;
	}
	return error;
}

//std::map<const std::string, const std::string> AgentComm::construct_message_header()
//{
//	map <const string, const string> header;
//
//	return header;
//}

// Marshals and sends a dictionary of values
void AgentComm::send(godot::Variant v)
{
	try
	{
		json marshaler;

		json& header = marshaler["header"];
		json& data = marshaler["data"];

		switch (v.get_type()) {
		case godot::Variant::DICTIONARY:
		{
			godot::Dictionary d = godot::Dictionary(v);
			godot::Array keys = d.keys();

			for (int i = 0; i < keys.size(); i++)
			{
				godot::Variant key = keys[i];
				godot::Variant value = d[key];

				json& element = data[convert_string(key)];

				if (is_basic_variant(value)) {
					marshal_basic_variant(value, element);
				}
				else if (is_array_variant(value)) {
					godot::Array value_array(value);
					for (int i = 0; i < value_array.size(); i++) {
						marshal_basic_variant_in_array(value_array[i], element);
					}
				}
			}

			std::cout << "JSON: " << marshaler.dump() << std::endl;

			break;
		}
		default:
			break;
		}	

		//marshaler["header"] = header;
		//marshaler["data"] = data;

		//cout << "JSON: " << marshaler.dump() << endl;
	}
	catch (exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
	}

	//cout << "content to serialize: " << marshaler.dump() << endl;

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