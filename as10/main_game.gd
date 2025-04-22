extends Control

var value = 0

@onready var label = $Label
@onready var increase_button = $Increment
@onready var decrease_button = $Decrement
@onready var player_list_label = $PlayerListLabel

func _ready():
	increase_button.pressed.connect(_on_increase_pressed)
	decrease_button.pressed.connect(_on_decrease_pressed)
	update_label() # Set initial label text
	
	update_player_list()

	multiplayer.peer_connected.connect(update_player_list)
	multiplayer.peer_disconnected.connect(update_player_list)

func update_label():
	label.text = "Value: " + str(value)	
	
# Function which updates the above value
func updateValue(newValue: int, from_owner = false):	
	# If we don't own the node and the message is not from the owner
	if !(is_multiplayer_authority() or from_owner):
		# Ask the owner to update the value
		rpc_id(get_multiplayer_authority(), "update_counter", newValue, false)
		return
	
	value = newValue
	update_label()
	
	# Make sure the value change gets propagated to all of the clients! 
	if is_multiplayer_authority() and !from_owner:
		rpc("update_counter", newValue, true)

# RPC which calls update value if called by the object's owner or the server
@rpc("any_peer")
func update_counter(new_value: int, from_owner: bool = false) -> void:
	if is_multiplayer_authority() or from_owner:
		updateValue(new_value, from_owner)

func _on_increase_pressed():
	update_counter(value + 1)

func _on_decrease_pressed():
	update_counter(value - 1)

func update_player_list():
	var ids = multiplayer.get_peers()
	player_list_label.text = "Connected Player IDs:\n"
	for id in ids:
		player_list_label.text += "- ID: %s\n" % id
