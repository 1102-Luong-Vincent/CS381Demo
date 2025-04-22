extends Control

@onready var host_button = $VBoxContainer/HostButton
@onready var join_button = $VBoxContainer/JoinButton
@onready var address_input = $VBoxContainer/AddressInput
@onready var status = $VBoxContainer/StatusLabel

func _ready():
	host_button.pressed.connect(_on_host_pressed) #connects the host button
	join_button.pressed.connect(_on_join_pressed) #connects join button
	
func _on_host_pressed() -> void:
	var localServer = ENetMultiplayerPeer.new()
	var createServer = localServer.create_server(9999, 3) #1234 is the ip address, with 2 people that can join
	
	if createServer != OK:
		status.text = "Failed to host server. Error code: %s" % createServer
		return;
		
	multiplayer.multiplayer_peer = localServer	 	
	status.text = "Hosting on port 9999"
		
	var game_scene = preload("res://main game.tscn")
	get_tree().change_scene_to_packed(game_scene)
	
func _on_join_pressed() -> void:
	var ipAddress =  address_input.text.strip_edges()
	
	if ipAddress == "":
		status.text = "IP address is empty"
		return;	
		
	var localClient = ENetMultiplayerPeer.new()
	var	joinServer = localClient.create_client(ipAddress, 9999)
	
	if joinServer != OK:
		status.text = "Failed to connect to %s" % ipAddress
		return
		
	multiplayer.multiplayer_peer = localClient
	status.text = "Connected to server at %s" % ipAddress
	 
	var game_scene = preload("res://main game.tscn")
	get_tree().change_scene_to_packed(game_scene)
