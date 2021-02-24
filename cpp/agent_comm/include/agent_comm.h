#pragma once

#include "share.h"
#include <Node.hpp>
#include <sstream>

class AgentComm : public Node {
	GODOT_CLASS(AgentComm, Node);

private:
	int send_counter = 0;

public:
	static void _register_methods();
	void _init_zeromq();
	void _init();

	void send(int);
	int recv();
};