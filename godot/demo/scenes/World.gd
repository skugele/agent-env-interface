extends Node2D

onready var agent_comm = $AgentCommNode

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

onready var counter = 0

# Called when the node enters the scene tree for the first time.
func _ready():
#	var val = agent_comm.bind()
	
	var n = null
	
	var b = false
	var s = "string"
	var i = 10
	var f = 10.4
	
	var b_array = [false, true, false]
	var s_array = ["Hippies", "2", "3", "Nooch"]
	var i_array = [1,2,3,4]
	var f_array = [1.1, 1.2, 1.3, 1.4]
	
	var dict = {'blah':5}
	
	agent_comm.send(n)
	agent_comm.send(b)
	agent_comm.send(s)
	agent_comm.send(i)
	agent_comm.send(f)
	agent_comm.send(b_array)
	agent_comm.send(s_array)
	agent_comm.send(i_array)
	agent_comm.send(f_array)
	
	agent_comm.send(dict)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

