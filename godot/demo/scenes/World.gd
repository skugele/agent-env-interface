extends Node2D


onready var ros_comm = $RosCommNode

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var state = ros_comm.counter
	print("Sending state: ", state)
	ros_comm.send_state(state)
	
	var actions = -1
	actions = ros_comm.recv_actions()
	print("New Actions Received: ", actions)
