extends Node2D

onready var agent_comm = $AgentCommNode

# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	agent_comm.send(1)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	agent_comm.send(agent_comm.send_counter)
	var v = agent_comm.recv()
	print(v)
