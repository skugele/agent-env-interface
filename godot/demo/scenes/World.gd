extends Node2D

onready var agent_comm = $AgentCommNode
onready var sensor_state = $SensorStateNode

onready var pub_options = {
	'agent':12345, 
	'port':9001, 
	'protocol':'tcp'
}

onready var server_options = {
	'port':9002, 
	'protocol':'tcp'
}

onready var pub_context = null
onready var request_counter = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	
#	sensor_state.state_var_1
	pub_context = agent_comm.connect(pub_options)
	
	agent_comm.start_listener(server_options)	
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	var msg = {
		'sensor_1': 5,
		'sensor_2': [1,2,3,4],
		'sensor_3': {'key1': 'value1',	
					 'key2': 'value2'},
		'sensor_4': null,
		'sensor_5': [false, true, true, false],
		'sensor_6': {1: {'inner_key1':'inner_value1'}}
	}
	
	agent_comm.send(msg, pub_context, "/agent/123/sensors/antennae/1/smell")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/antennae/2/smell")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/antennae/1/taste")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/antennae/2/taste")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/antennae/1/touch")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/antennae/2/touch")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/hairs")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/pain")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/velocity")
	agent_comm.send(msg, pub_context, "/agent/123/sensors/health")
	
	# receive actions
	
	# execute actions
#	agent_comm.stop_listener()
	
func _on_action_received(action_details):
	request_counter += 1
	
	if (request_counter % 100 == 0):
		print("# requests: ", request_counter)
