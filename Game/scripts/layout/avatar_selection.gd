extends Control

var main_team := Team.new()
var current_player_id:int = 0
var count: int = 0

signal avatar_changed(avatar_id: String)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	main_team.name = "Main Team"
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if(count==10):
		print(current_player_id)
		print(main_team.players[current_player_id].player_avatar)
		count = 0
	else:
		count = count + 1
		
func set_player(player_id:int):
	current_player_id = player_id
	avatar_changed.emit(main_team.players[current_player_id].player_avatar)
	
func set_avatar(avatar_id: String):
	main_team.players[current_player_id].player_avatar = avatar_id
	avatar_changed.emit(main_team.players[current_player_id].player_avatar)
	
