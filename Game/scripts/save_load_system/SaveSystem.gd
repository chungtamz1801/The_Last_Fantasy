extends Node

signal data_readed(readed_data: Variant)

var readed_data: Variant = null

func load_game():
	if not FileAccess.file_exists("user://savegame.save"):
		return
	
	var save_file = FileAccess.open("user://savegame.save", FileAccess.READ)
	while save_file.get_position() < save_file.get_length():
		var json_string = save_file.get_line() 
		var json = JSON.new()

		var parse_result = json.parse(json_string)
		if not parse_result == OK:
			print("JSON Parse Error: ", json.get_error_message(), " in ", json_string, " at line ", json.get_error_line())
			continue
		
		var node_data = json.data

		# Firstly, we need to create the object and add it to the tree and set its position.
		var new_object = load(node_data["filename"]).instantiate()
		get_tree().current_scene.add_child(new_object)
		#get_node(node_data["parent"]).add_child(new_object)
		new_object.position = Vector2(node_data["pos_x"], node_data["pos_y"])
#
		# Now we set the remaining variables.
		var player = Player.new()
		for i in node_data.keys():
			if i == "filename" or i == "parent" or i == "pos_x" or i == "pos_y":
				continue
			if i == "skills":
				player.set(i, Array(node_data[i], TYPE_STRING, &"", null)) 
				continue
			player.set(i,node_data[i])
		new_object.texture = load("res://sprites/avatars/%s/%s" % [node_data["player_name"],node_data["player_avatar"]+".png"])
		data_readed.emit(player)
		
