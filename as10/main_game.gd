extends Control

var value = 0

@onready var label = $Label
@onready var increase_button = $Increment
@onready var decrease_button = $Decrement
@onready var player_list_label = $PlayerListLabel

func _ready():
	increase_button.pressed.connect(_on_increase_pressed)
	decrease_button.pressed.connect(_on_decrease_pressed)
	update_label()
	
	await get_tree().create_timer(0.5).timeout  # <== Give time for peers to fully connect
	update_player_list()

	await get_tree().create_timer(1.0).timeout
	update_player_list()
	
	multiplayer.peer_connected.connect(update_player_list)
	multiplayer.peer_disconnected.connect(update_player_list)

func update_label():
	label.text = "Value: " + str(value)

# Called locally by anyone
func _on_increase_pressed():
	if multiplayer.is_server():
		request_update(1)  # call directly if host
	else:
		request_update.rpc_id(1, 1)  # send to host if client

func _on_decrease_pressed():
	if multiplayer.is_server():
		request_update(-1)
	else:
		request_update.rpc_id(1, -1)

# Only the host receives this call
@rpc("any_peer")
func request_update(delta: int):
	if multiplayer.is_server():
		value += delta
		broadcast_value.rpc(value)   # Send to all the clients
		broadcast_value.call(value)  # Run locally on host

# Sync the value on all peers
@rpc("authority")
func broadcast_value(new_value: int):
	value = new_value
	update_label()

func update_player_list():
	var ids = multiplayer.get_peers()
	ids.insert(0, multiplayer.get_unique_id())  # Include self at top
		
	player_list_label.text = "Connected Player IDs: \n"
	for id in ids:
		if id == multiplayer.get_unique_id():
			player_list_label.text += "- ID: %s (You)\n" % id
		else:	
			player_list_label.text += "- ID: %s\n" % id
