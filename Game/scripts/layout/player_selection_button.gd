extends Button

func _ready():
	self.pressed.connect(set_player)

func set_player():
	var avatar_selection := get_tree().current_scene
	var player_id := int(self.name.substr(6,1))-1
	avatar_selection.set_player(player_id)
