#include "ros_comm.h"

void RosComm::_register_methods()
{
	// Register methods
	register_method("send_state", &RosComm::send_state);
	register_method("recv_actions", &RosComm::recv_actions);

	// Register properties
	register_property("counter", &RosComm::counter, 0);
}

void RosComm::_init()
{
	cout << "initializing RosComm class" << endl;
}

void RosComm::send_state(int i)
{
	cout << "sending state: " << i << endl;
}

int RosComm::recv_actions()
{
	counter++;
	cout << "receiving actions: " << counter << endl;

	return counter;
}
