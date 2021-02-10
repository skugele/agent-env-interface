#ifndef ROS_COMM_H
#define ROS_COMM_H

#include "common.h"
#include <Godot.hpp>
#include <Node.hpp>

class RosComm : public Node {
	GODOT_CLASS(RosComm, Node);

private:
	int counter = 0;

public:
	static void _register_methods();
	void _init();

	void send_state(int);
	int recv_actions();

};

#endif // !ROS_COMM_H