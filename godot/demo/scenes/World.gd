extends Node2D

onready var agent_comm = $AgentCommNode
onready var sensor_state = $SensorStateNode

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

onready var counter = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	
	sensor_state.state_var_1
#	var val = agent_comm.bind()

	# test basic types
	agent_comm.send(null)
	agent_comm.send(false)
	agent_comm.send(1.0)
	agent_comm.send(-6)
	agent_comm.send("bullshit")	
	
	
	# test array types	
	var nested_array = [
		[1],
		[1,2],
		[1,2,"3"]
	]

	agent_comm.send([1,2,3,4])
	agent_comm.send(nested_array)
		
	var dict = {
		'sensor_1': 5,
		'sensor_2': [1,2,3,4],
		'sensor_3': {'key1': 'value1',
					 'key2': 'value2'},
		'sensor_4': null,
		'sensor_5': [false, true, true, false],
		'sensor_6':{1: {'inner_key1':'inner_value1'}}
	}

	agent_comm.send(dict)

	var crazy_example = {
		'sensor_1': {'key1':[1,2,3,4, {'inner_key1': 4.0, 'inner_key2': [1,2,false]}]},
		'sensor_2':null,
		'sensor_3':[5,6,[1,2,3,false],null,{1:2,3:4}]
	}

	agent_comm.send(crazy_example)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	# publish agent state
	
	# receive actions
	
	# execute actions
	
	
	pass
