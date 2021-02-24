#include "agent_comm.h"

void AgentComm::_register_methods()
{
	// Register methods
	register_method("send", &AgentComm::send);
	register_method("recv", &AgentComm::recv);

	// Register properties
	register_property("send_counter", &AgentComm::send_counter, 0);
}

void AgentComm::_init()
{
	cout << "initializing AgentComm class" << endl;

	//AgentComm::_init_ros();
}

void AgentComm::_init_zeromq()
{
	cout << "initializing zeromq" << endl;
}

void AgentComm::send(int i)
{
	cout << "sending state: " << send_counter++ << endl;

	//std_msgs::String msg;
	//std::stringstream ss;
	//ss << i;
	//msg.data = ss.str();

	//ROS_INFO("%s", msg.data.c_str());

	//AgentComm::pub.publish(msg);

	send_counter++;
}

int AgentComm::recv()
{
	cout << "receiving actions: " << send_counter << endl;

	return send_counter;
}
