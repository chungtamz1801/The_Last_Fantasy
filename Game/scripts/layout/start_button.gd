extends Button

func _ready():
	self.pressed.connect(_on_pressed)
	
func _on_pressed():
	var root = get_tree().current_scene
	for player in root.main_team.players:
		if(player.player_avatar.is_empty()):
			root.show_alert("Please choose 4 avatar")
			return
	GameManager.main_team = root.main_team
	get_tree().change_scene_to_file("res://layout/team_management.tscn")
