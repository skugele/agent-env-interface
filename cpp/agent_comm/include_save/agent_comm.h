#pragma once

#include "share.h"
#include <Node.hpp>
#include <sstream>
#include <cstdlib>

//#include "crap.h"

#include <zmqpp/zmqpp.hpp>

class AgentComm : public Node {
	GODOT_CLASS(AgentComm, Node);

private:
	int send_counter = 0;

	const std::string endpoint = "tcp://localhost:4242";

	zmqpp::context context;
	zmqpp::socket* pub;
	zmqpp::socket* sub;

public:
	static void _register_methods();
	void _init_zeromq();
	void _init();

	void send(int);
	int recv();
};