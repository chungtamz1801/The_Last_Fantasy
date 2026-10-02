extends Button

func _pressed() -> void:
	var command_list := $"../../CommandList"
	var separation = command_list.get_theme_constant("h_separation")
	
	for child in command_list.get_children():
		child.queue_free()
	
	var new_button = Button.new()
		
	new_button.text = "Normal Attack"
	#size
	new_button.custom_minimum_size.x = (command_list.size.x - separation) / 2
	new_button.pressed.connect(func(): print("Normal Attack"))
	command_list.add_child(new_button)
	
	new_button = Button.new()
	new_button.text = "Move"
	#size
	new_button.custom_minimum_size.x = (command_list.size.x - separation) / 2
	new_button.pressed.connect(func(): print("Move"))
	command_list.add_child(new_button)
	
	new_button = Button.new()
	new_button.text = "Rank Up"
	#size
	new_button.custom_minimum_size.x = (command_list.size.x - separation) / 2
	new_button.pressed.connect(func(): print("Rank Up"))
	command_list.add_child(new_button)
	
	new_button = Button.new()
	new_button.text = "Surrender"
	#size
	new_button.custom_minimum_size.x = (command_list.size.x - separation) / 2
	new_button.pressed.connect(func(): print("Surrender"))
	#var style_box:StyleBox = new_button.get_theme_stylebox("normal").duplicate()
	#style_box.content_margin_left = 15
	#new_button.add_theme_stylebox_override("normal",style_box)
	
	command_list.add_child(new_button)
